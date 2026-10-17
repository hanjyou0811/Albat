use crate::{Diagnostic, LibraryOptions};
use std::collections::{BTreeMap, BTreeSet};
mod catalog;
pub use catalog::CATALOG;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Position {
    Head,
    Main,
}
#[derive(Clone, Debug)]
pub struct Entry {
    pub name: &'static str,
    pub body: &'static str,
    pub position: Position,
    pub dependencies: &'static [&'static str],
}
#[derive(Clone, Debug)]
pub struct LibraryManager {
    options: LibraryOptions,
    requested: BTreeSet<String>,
    excluded: BTreeSet<String>,
    enabled: BTreeSet<String>,
    dynamic: BTreeMap<String, (String, Position, Vec<String>)>,
    dynamic_order: Vec<String>,
}
impl LibraryManager {
    pub fn new(options: &LibraryOptions) -> Self {
        let mut s = Self {
            options: options.clone(),
            requested: BTreeSet::new(),
            excluded: BTreeSet::new(),
            enabled: BTreeSet::new(),
            dynamic: BTreeMap::new(),
            dynamic_order: Vec::new(),
        };
        if options.pragma {
            s.request("pragma", false);
        }
        if options.stdcpp {
            s.request("stdc++", false);
        }
        if options.namespace_std {
            s.request("namespace_std", false);
        }
        s
    }
    pub fn request(&mut self, name: &str, already: bool) {
        if already {
            self.excluded.insert(name.into());
        } else {
            self.requested.insert(name.into());
        }
    }
    pub fn set_enabled(&mut self, name: &str, enabled: bool) {
        if enabled {
            self.enabled.insert(name.into());
        } else {
            self.enabled.remove(name);
        }
    }
    pub fn is_enabled(&self, name: &str) -> bool {
        self.enabled.contains(name)
    }
    pub fn register(
        &mut self,
        name: &str,
        body: String,
        position: Position,
        dependencies: Vec<String>,
    ) {
        if !self.dynamic.contains_key(name) {
            self.dynamic_order.push(name.into());
        }
        self.dynamic
            .insert(name.into(), (body, position, dependencies));
        self.request(name, false);
    }
    pub fn body(&self, name: &str) -> &str {
        if let Some((body, _, _)) = self.dynamic.get(name) {
            return body;
        }
        CATALOG
            .iter()
            .find(|e| e.name == name)
            .map(|e| e.body)
            .unwrap_or("")
    }
    pub fn emit(&self, position: Position) -> Result<String, Diagnostic> {
        let mut needed = self.requested.clone();
        loop {
            let before = needed.len();
            for entry in CATALOG {
                if needed.contains(entry.name) {
                    for &dep in entry.dependencies {
                        if entry.name == "ACL"
                            && dep == "namespace_acl"
                            && !self.options.namespace_acl
                        {
                            continue;
                        }
                        needed.insert(dep.into());
                    }
                }
            }
            for (name, (_, _, deps)) in &self.dynamic {
                if needed.contains(name) {
                    needed.extend(deps.iter().cloned());
                }
            }
            if before == needed.len() {
                break;
            }
        }
        // Unknown legacy requests are intentionally inert (e.g. the disabled sum library).
        let mut out = String::new();
        for e in CATALOG {
            let pos = if e.name == "DEFINE_INT_LL" && self.options.define_int_ll != 1 {
                Position::Head
            } else {
                e.position
            };
            if pos == position && needed.contains(e.name) && !self.excluded.contains(e.name) {
                out.push_str(e.body);
            }
        }
        for name in &self.dynamic_order {
            let (body, pos, _) = &self.dynamic[name];
            if *pos == position && needed.contains(name) && !self.excluded.contains(name) {
                out.push_str(body);
            }
        }
        Ok(out.trim().into())
    }
}
