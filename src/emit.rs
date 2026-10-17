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
    for n in &program.nodes {
        if n.function && !contains_main(&n.text) && n.children.is_some() {
            out.push_str(&prototype(&n.text)?);
            out.push_str(";\n");
        }
    }
    out.push_str(&nodes(&program.nodes, libs)?);
    Ok(out)
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
