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
        let name = name
            .trim()
            .trim_start_matches("const ")
            .trim_start_matches("std::");
        if let Some((base, _)) = name.split_once('<') {
            return name.ends_with('>')
                && (self.types.contains(base.trim())
                    || [
                        "pair",
                        "tuple",
                        "vector",
                        "stack",
                        "queue",
                        "deque",
                        "priority_queue",
                        "set",
                        "multiset",
                        "map",
                        "bitset",
                        "unordered_set",
                        "unordered_map",
                        "Fenwick_tree",
                        "_SegTree",
                        "SegTree_pt_max",
                        "SegTree_pt_min",
                        "SegTree_pt_sum",
                        "WeightedUnionFind",
                        "CycleLib",
                    ]
                    .contains(&base.trim()));
        }
        self.types.contains(name)
            || [
                "void",
                "char",
                "signed char",
                "unsigned char",
                "int",
                "signed int",
                "unsigned int",
                "signed",
                "unsigned",
                "uint",
                "long",
                "signed long",
                "unsigned long",
                "ulong",
                "long long",
                "signed long long",
                "unsigned long long",
                "short",
                "float",
                "double",
                "long double",
                "bool",
                "auto",
                "string",
                "size_t",
                "ll",
                "ull",
                "mint",
                "modint",
                "mod_int",
                "MexSet",
                "UnionFind",
                "istream",
                "ostream",
                "cpp_int",
                "int8_t",
                "int16_t",
                "int32_t",
                "int64_t",
                "uint8_t",
                "uint16_t",
                "uint32_t",
                "uint64_t",
                "__int8_t",
                "__int16_t",
                "__int32_t",
                "__int64_t",
                "__int128_t",
                "__uint8_t",
                "__uint16_t",
                "__uint32_t",
                "__uint64_t",
                "__uint128_t",
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
