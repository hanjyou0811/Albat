pub mod declarations;
pub mod io;
pub mod macros;
pub mod operators;
use crate::{
    library::LibraryManager,
    scan::{code_mask, is_identifier_byte, replace_identifier},
    scope::Scope,
    Diagnostic,
};
pub fn rewrite(
    statement: &str,
    scope: &mut Scope,
    libraries: &mut LibraryManager,
) -> Result<String, Diagnostic> {
    let mut s = declarations::grouped(&aliases(&macros::rewrite(statement)?)?)?;
    s = slices(&s, libraries)?;
    s = operators::rewrite_typed(&s, libraries, scope)?;
    if let Some(tuple) = declarations::tuple(&s, scope)? {
        s = tuple;
    }
    if let Some(io) = io::rewrite(&s, scope, libraries)? {
        s = io;
    }
    detect_libraries(&s, libraries)?;
    Ok(s)
}
pub fn detect_libraries(s: &str, libraries: &mut LibraryManager) -> Result<(), Diagnostic> {
    let mask = code_mask(s)?;
    let mut tokens = Vec::new();
    let mut calls = std::collections::BTreeSet::new();
    let mut i = 0;
    while i < s.len() {
        if mask[i] && is_identifier_byte(s.as_bytes()[i]) {
            let start = i;
            while i < s.len() && mask[i] && is_identifier_byte(s.as_bytes()[i]) {
                i += 1;
            }
            let token = &s[start..i];
            tokens.push(token);
            if s[i..].trim_start().starts_with('(') {
                calls.insert(token);
            }
        } else {
            i += 1;
        }
    }
    let typed = [
        "UnionFind",
        "WeightedUnionFind",
        "_SegTree",
        "SegTree_pt_max",
        "SegTree_pt_min",
        "SegTree_pt_sum",
        "Fenwick_tree",
        "mint",
        "modint",
        "mod_int",
        "CycleLib",
        "MexSet",
    ];
    let functions = [
        ("biseek", "biseek"),
        ("chmax", "chmax"),
        ("chmin", "chmin"),
        ("runLengthEncoding", "runLengthEncoding"),
        ("yesno", "yesno"),
        ("GCD_L", "gcd"),
        ("lcm", "lcm"),
        ("inversionNumber", "inversionNumber"),
        ("divNum", "divnum"),
        ("makePrimes", "makePrimes"),
        ("popc", "popcount"),
        ("popcl", "popcountl"),
        ("bit_enumerate", "bit_enumerate"),
        ("hasbit", "hasbit"),
        ("bitlen", "bitlen"),
        ("trim", "trim"),
        ("ltrim", "ltrim"),
        ("rtrim", "rtrim"),
    ];
    for token in &tokens {
        if typed.contains(token) {
            libraries.request(token, false);
        }
    }
    for (call, name) in functions {
        if calls.contains(call) {
            libraries.request(name, false);
        }
    }
    for name in ["cin", "cout", "scanf", "printf"] {
        if tokens.contains(&name) {
            libraries.set_enabled(name, true);
        }
    }
    if libraries.is_enabled("cin") {
        libraries.request("istream", false);
    }
    if libraries.is_enabled("cout") {
        libraries.request("ostream", false);
    }
    let mixed = libraries.is_enabled("FastIO")
        || libraries.is_enabled("FastIO_flush")
        || (libraries.is_enabled("cin") && libraries.is_enabled("scanf"))
        || (libraries.is_enabled("cout") && libraries.is_enabled("printf"));
    for name in ["SYNC_WITH_STDIO", "CIN_TIE"] {
        if mixed {
            libraries.request(name, true);
        } else if libraries.is_enabled("cin") || libraries.is_enabled("cout") {
            libraries.request(name, false);
        }
    }
    Ok(())
}
pub fn aliases(s: &str) -> Result<String, Diagnostic> {
    let mut s = s.to_owned();
    for (a, b) in [
        ("ll", "long long"),
        ("ull", "unsigned long long"),
        ("ld", "long double"),
        ("LD", "long double"),
        ("vi", "vector<int>"),
        ("VI", "vector<int>"),
        ("vll", "vector<long long>"),
        ("VLL", "vector<long long>"),
        ("vl", "vector<long long>"),
        ("VL", "vector<long long>"),
        ("vs", "vector<string>"),
        ("VS", "vector<string>"),
        ("vvi", "vector<vector<int>>"),
        ("VVI", "vector<vector<int>>"),
        ("vvll", "vector<vector<long long>>"),
        ("VVLL", "vector<vector<long long>>"),
        ("vvl", "vector<vector<long long>>"),
        ("VVL", "vector<vector<long long>>"),
        ("vvvi", "vector<vector<vector<int>>>"),
        ("VVVI", "vector<vector<vector<int>>>"),
        ("PII", "pair<int, int>"),
        ("Pll", "pair<long long, long long>"),
        ("PLL", "pair<long long, long long>"),
        ("infi", "1073709056"),
        ("INFI", "1073709056"),
        ("infl", "4611686016279904256LL"),
        ("INFL", "4611686016279904256LL"),
        ("inff", "1.2e153"),
        ("gcd", "GCD_L"),
        ("caesar", "ci.Caesar"),
    ] {
        s = replace_identifier(&s, a, b)?;
    }
    Ok(s)
}

fn slices(source: &str, libs: &mut LibraryManager) -> Result<String, Diagnostic> {
    let mut s = source.to_owned();
    let mut search = 0;
    loop {
        let mask = code_mask(&s)?;
        let Some(open) = (search..s.len()).find(|&i| mask[i] && s.as_bytes()[i] == b'[') else {
            break;
        };
        let close = crate::scan::matching_close(&s, open)?;
        let parts = crate::scan::split_top_level(&s[open + 1..close], ':')?;
        if !(2..=3).contains(&parts.len()) {
            search = close + 1;
            continue;
        }
        let mut start = open;
        while start > 0 && is_identifier_byte(s.as_bytes()[start - 1]) {
            start -= 1;
        }
        if start == open {
            search = close + 1;
            continue;
        }
        let replacement = format!(
            "Slice({}, {}, {}, {})",
            &s[start..open],
            parts[0],
            parts[1],
            parts.get(2).copied().unwrap_or("1")
        );
        s.replace_range(start..close + 1, &replacement);
        libs.request("Slice", false);
        search = start + replacement.len();
    }
    Ok(s)
}
