use std::fmt;

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Diagnostic {
    pub source: String,
    pub line: usize,
    pub column: usize,
    pub message: String,
}
impl Diagnostic {
    pub fn at(text: &str, offset: usize, message: impl Into<String>) -> Self {
        let mut end = offset.min(text.len());
        while !text.is_char_boundary(end) {
            end -= 1;
        }
        let prefix = &text[..end];
        Self {
            source: "<stdin>".into(),
            line: prefix.bytes().filter(|&b| b == b'\n').count() + 1,
            column: prefix.rsplit('\n').next().unwrap_or("").chars().count() + 1,
            message: message.into(),
        }
    }
}
impl fmt::Display for Diagnostic {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "{}:{}:{}: {}",
            self.source, self.line, self.column, self.message
        )
    }
}
impl std::error::Error for Diagnostic {}
