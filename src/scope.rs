use std::collections::BTreeSet;
#[derive(Clone, Debug, Default)]
pub struct Scope {
    names: BTreeSet<String>,
    next: usize,
    types: BTreeSet<String>,
}
impl Scope {
    pub fn reserve_source(&mut self, source: &str) {
        if let Ok(mask) = crate::scan::code_mask(source) {
            let mut previous = "";
            let mut i = 0;
            while i < source.len() {
                if mask[i] && crate::scan::is_identifier_byte(source.as_bytes()[i]) {
                    let start = i;
                    while i < source.len()
                        && mask[i]
                        && crate::scan::is_identifier_byte(source.as_bytes()[i])
                    {
                        i += 1;
                    }
                    let word = &source[start..i];
                    if ["struct", "class", "typename", "using"].contains(&previous) {
                        self.types.insert(word.into());
                    }
                    previous = word;
                } else {
                    i += 1;
                }
            }
        }

        for word in source.split(|c: char| !(c.is_alphanumeric() || c == '_')) {
            if !word.is_empty() {
                self.names.insert(word.into());
            }
        }
    }
    pub fn is_type(&self, name: &str) -> bool {
        self.types.contains(name)
            || [
                "void", "char", "int", "long", "short", "float", "double", "bool", "auto",
                "string", "unsigned", "signed", "size_t", "ll", "ull", "mint", "modint", "mod_int",
            ]
            .contains(&name)
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
