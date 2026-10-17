use std::collections::BTreeSet;
#[derive(Clone, Debug, Default)]
pub struct Scope {
    names: BTreeSet<String>,
    next: usize,
}
impl Scope {
    pub fn reserve_source(&mut self, source: &str) {
        for word in source.split(|c: char| !(c.is_alphanumeric() || c == '_')) {
            if !word.is_empty() {
                self.names.insert(word.into());
            }
        }
    }
    pub fn fresh_name(&mut self) -> String {
        loop {
            let name = format!("albat_tmp_{}", self.next);
            self.next += 1;
            if self.names.insert(name.clone()) {
                return name;
            }
        }
    }
}
