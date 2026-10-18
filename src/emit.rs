use crate::{
    ir::{Node, Program},
    library::{LibraryManager, Position},
    parser::contains_main,
    transform, Diagnostic,
};
fn filename(s: &str) -> String {
    s.replace('\\', "\\\\")
        .replace('"', "\\\"")
        .replace('\n', "\\n")
        .replace('\r', "\\r")
}
pub(crate) fn nodes(nodes: &[Node], libs: &LibraryManager) -> Result<String, Diagnostic> {
    let mut out = String::new();
    for n in nodes {
        if n.line > 0 {
            out.push_str(&format!("#line {} \"{}\"\n", n.line, filename(&n.source)));
        }
        if n.suffix == "@include" {
            if let Some(children) = &n.children {
                out.push_str(&self::nodes(children, libs)?);
            }
            continue;
        }
        out.push_str(&n.text);
        if let Some(children) = &n.children {
            out.push_str(" {\n");
            if contains_main(&n.text) {
                out.push_str("#line 1 \"<generated>\"\n");
                out.push_str(&libs.emit(Position::Main)?);
                out.push('\n');
            }
            out.push_str(&self::nodes(children, libs)?);
            out.push_str("#line 1 \"<generated>\"\n}");
            out.push_str(&n.suffix);
        }
        out.push('\n');
    }
    Ok(out)
}
pub fn emit(program: &Program, libs: &LibraryManager) -> Result<String, Diagnostic> {
    let mut scratch = libs.clone();
    let mut out = transform::operators::rewrite(
        &transform::aliases(&transform::macros::rewrite(&libs.emit(Position::Head)?)?)?,
        &mut scratch,
    )?;
    if let Some(pos) = out.find("using namespace std;") {
        let mut declarations = String::new();
        for entry in crate::library::CATALOG {
            if entry.body.is_empty() || !libs.selected_names().contains(entry.name) {
                continue;
            }
            // Class definitions and their free operators must follow the class declaration.
            if entry.body.contains("struct ") || entry.body.contains("class FastIO") {
                continue;
            }
            let body = transform::operators::rewrite(
                &transform::macros::rewrite(entry.body)?,
                &mut scratch,
            )?;
            let parsed = crate::parser::parse_fragment(
                &body,
                "<generated>",
                &crate::NoSources,
                &mut scratch,
            )?;
            for node in parsed {
                if node.function && node.children.is_some() {
                    declarations.push_str(&prototype(&node.text)?);
                    declarations.push_str(";\n");
                }
            }
        }
        out.insert_str(
            pos + "using namespace std;".len(),
            &format!("\n{declarations}"),
        );
    }
    out.push('\n');
    // Stream helpers are parsed from frozen Albat-compatible assets above.
    if libs.is_enabled("FastIO") || libs.is_enabled("FastIO_flush") {
        let flush = libs.is_enabled("FastIO_flush")
            || libs.is_enabled("cin")
            || libs.is_enabled("cout")
            || libs.is_enabled("printf")
            || libs.is_enabled("scanf");
        out.push_str(libs.body(if flush { "FastIO_flush" } else { "FastIO" }));
        out.push('\n');
    }
    collect_prototypes(&program.nodes, &mut out)?;
    out.push_str(&nodes(&program.nodes, libs)?);
    Ok(out)
}

// Includes are transparent, but classes, namespaces and function bodies are
// real scopes: only descend through include wrappers at this scope.
fn collect_prototypes(nodes: &[Node], out: &mut String) -> Result<(), Diagnostic> {
    for node in nodes {
        if node.suffix == "@include" {
            if let Some(children) = &node.children {
                collect_prototypes(children, out)?;
            }
        } else if node.function && !contains_main(&node.text) && node.children.is_some() {
            out.push_str(&prototype(&node.text)?);
            out.push_str(";\n");
        }
    }
    Ok(())
}

fn prototype(header: &str) -> Result<String, Diagnostic> {
    let Some(open) = header.find('(') else {
        return Ok(header.into());
    };
    let close = crate::scan::matching_close(header, open)?;
    let args = crate::scan::split_top_level(&header[open + 1..close], ',')?;
    let args: Vec<&str> = args
        .into_iter()
        .map(|s| s.split('=').next().unwrap_or(s).trim())
        .collect();
    Ok(format!(
        "{}({}){}",
        &header[..open],
        args.join(", "),
        &header[close + 1..]
    ))
}
