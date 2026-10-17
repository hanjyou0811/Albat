//! Albat to C++ translation.
pub mod diagnostic;
pub mod options;
pub mod scan;
pub use diagnostic::Diagnostic;
pub use options::{LibraryOptions, Options};

pub trait SourceLoader {
    fn load(&self, requested: &str, from: &str) -> Result<String, Diagnostic>;
}
pub struct NoSources;
impl SourceLoader for NoSources {
    fn load(&self, requested: &str, from: &str) -> Result<String, Diagnostic> {
        Err(Diagnostic {
            source: from.into(),
            line: 1,
            column: 1,
            message: format!("file loading is unavailable: {requested}"),
        })
    }
}

pub mod emit;
pub mod ir;
pub mod library;
pub mod parser;
pub mod scope;
pub mod transform;

pub fn transpile(
    source: &str,
    options: &Options,
    loader: &dyn SourceLoader,
) -> Result<String, Diagnostic> {
    let mut libraries = library::LibraryManager::new(&options.libraries);
    let mut program = parser::parse(source, options, loader, &mut libraries)?;
    let mut scope = scope::Scope::default();
    scope.reserve_source(source);
    fn reserve_nodes(nodes: &[ir::Node], scope: &mut scope::Scope) {
        for node in nodes {
            scope.reserve_source(&node.text);
            scope.reserve_source(&node.suffix);
            if let Some(children) = &node.children {
                reserve_nodes(children, scope);
            }
        }
    }
    reserve_nodes(&program.nodes, &mut scope);
    fn rewrite_nodes(
        nodes: &mut Vec<ir::Node>,
        scope: &mut scope::Scope,
        libs: &mut library::LibraryManager,
        loader: &dyn SourceLoader,
    ) -> Result<(), Diagnostic> {
        let mut result = Vec::new();
        for mut node in std::mem::take(nodes) {
            if node.children.is_some() && node.text.contains('@') {
                let (prelude, header) = transform::io::condition(&node.text, scope, libs)?;
                node.text = header;
                if !prelude.is_empty() {
                    result.push(ir::Node {
                        text: prelude,
                        children: None,
                        suffix: String::new(),
                        source: node.source.clone(),
                        line: node.line,
                        function: false,
                    });
                }
            }
            if node.children.is_some()
                && node
                    .text
                    .strip_prefix("do_perm")
                    .is_some_and(|rest| rest.trim_start().starts_with('('))
            {
                let open = node.text.find('(').unwrap();
                let close = scan::matching_close(&node.text, open)?;
                let args = transform::macros::rewrite(&node.text[open + 1..close])?;
                node.text = "do".into();
                node.suffix = format!(" while(next_permutation({args}));");
            }
            if node.children.is_none() && node.text.contains('[') {
                let mut offset = 0;
                loop {
                    let mask = scan::code_mask(&node.text)?;
                    let bytes = node.text.as_bytes();
                    let capture = (offset..node.text.len()).find(|&i| {
                        if !mask[i] || bytes[i] != b'[' {
                            return false;
                        }
                        let previous = node.text[..i].trim_end().as_bytes().last().copied();
                        previous.is_none()
                            || previous
                                .is_some_and(|b| matches!(b, b'=' | b'(' | b',' | b'{' | b';'))
                    });
                    let Some(capture) = capture else {
                        break;
                    };
                    let capture_end = scan::matching_close(&node.text, capture)?;
                    let mut open = capture_end + 1;
                    while open < bytes.len() && bytes[open].is_ascii_whitespace() {
                        open += 1;
                    }
                    if bytes.get(open) == Some(&b'(') {
                        open = scan::matching_close(&node.text, open)? + 1;
                    }
                    while open < bytes.len() && bytes[open] != b'{' && bytes[open] != b';' {
                        open += 1;
                    }
                    if bytes.get(open) != Some(&b'{') {
                        offset = capture_end + 1;
                        continue;
                    }
                    let close = scan::matching_close(&node.text, open)?;
                    let mut body = parser::parse_fragment(
                        &node.text[open + 1..close],
                        &node.source,
                        loader,
                        libs,
                    )?;
                    fn adjust_lines(nodes: &mut [ir::Node], offset: usize) {
                        for node in nodes {
                            node.line += offset;
                            if let Some(children) = &mut node.children {
                                adjust_lines(children, offset);
                            }
                        }
                    }
                    let line_offset = node.line.saturating_sub(1)
                        + node.text[..open + 1]
                            .bytes()
                            .filter(|&b| b == b'\n')
                            .count();
                    adjust_lines(&mut body, line_offset);
                    rewrite_nodes(&mut body, scope, libs, loader)?;
                    let replacement = format!("{{\n{}\n}}", emit::nodes(&body, libs)?);
                    node.text.replace_range(open..close + 1, &replacement);
                    offset = open + replacement.len();
                }
            }
            if !node.text.starts_with('#') {
                node.text = transform::rewrite(&node.text, scope, libs).map_err(|mut error| {
                    error.source = node.source.clone();
                    error.line += node.line.saturating_sub(1);
                    error
                })?;
            }
            if node.suffix.trim_start().starts_with("while") {
                node.suffix = transform::rewrite(&node.suffix, scope, libs)?;
            }
            if let Some(children) = &mut node.children {
                rewrite_nodes(children, scope, libs, loader)?;
            }
            result.push(node);
        }
        *nodes = result;
        Ok(())
    }
    rewrite_nodes(&mut program.nodes, &mut scope, &mut libraries, loader)?;
    emit::emit(&program, &libraries)
}
#[cfg(not(target_arch = "wasm32"))]
pub mod cli;
#[cfg(feature = "wasm-bindgen")]
pub mod wasm;
