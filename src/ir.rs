#[derive(Clone, Debug)]
pub struct Node {
    pub text: String,
    pub children: Option<Vec<Node>>,
    pub suffix: String,
    pub source: String,
    pub line: usize,
    pub function: bool,
}
#[derive(Clone, Debug)]
pub struct Program {
    pub nodes: Vec<Node>,
}
