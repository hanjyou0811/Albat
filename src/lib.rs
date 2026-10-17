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
            if node.text.starts_with("do_perm") {
                let open = node.text.find('(').unwrap();
                let close = scan::matching_close(&node.text, open)?;
                let args = transform::macros::rewrite(&node.text[open + 1..close])?;
                node.text = "do".into();
                node.suffix = format!(" while(next_permutation({args}));");
            }
            if node.children.is_none() && node.text.contains('[') && node.text.contains('{') {
                let mask = scan::code_mask(&node.text)?;
                let mut offset = 0;
                while let Some(open) = (offset..node.text.len())
                    .find(|&i| mask.get(i) == Some(&true) && node.text.as_bytes()[i] == b'{')
                {
                    let close = scan::matching_close(&node.text, open)?;
                    let mut body = parser::parse_fragment(
                        &node.text[open + 1..close],
                        &node.source,
                        loader,
                        libs,
                    )?;
                    rewrite_nodes(&mut body, scope, libs, loader)?;
                    let replacement = format!("{{\n{}\n}}", emit::nodes(&body, libs)?);
                    node.text.replace_range(open..close + 1, &replacement);
                    offset = open + replacement.len();
                }
            }
            if !node.text.starts_with('#') {
                node.text = transform::rewrite(&node.text, scope, libs)?;
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
