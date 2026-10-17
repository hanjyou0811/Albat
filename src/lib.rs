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

pub mod library;
