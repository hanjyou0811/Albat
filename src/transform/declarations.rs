use crate::{
    scan::{matching_close, split_top_level},
    scope::Scope,
    Diagnostic,
};
pub fn tuple(s: &str, scope: &mut Scope) -> Result<Option<String>, Diagnostic> {
    let s = s.trim();
    if !s.starts_with('(') {
        return Ok(None);
    }
    let left_end = matching_close(s, 0)?;
    let rest = s[left_end + 1..].trim_start();
    let Some(rest) = rest.strip_prefix('=') else {
        return Ok(None);
    };
    let rest = rest.trim_start();
    if !rest.starts_with('(') {
        return Ok(None);
    }
    let right_end = matching_close(rest, 0)?;
    let left = split_top_level(&s[1..left_end], ',')?;
    let right = split_top_level(&rest[1..right_end], ',')?;
    if left.len() != right.len() {
        return Err(Diagnostic::at(s, 0, "tuple assignment arity mismatch"));
    }
    if left.len() < 2 {
        return Ok(None);
    }
    let temps: Vec<String> = left.iter().map(|_| scope.fresh_name()).collect();
    let mut out = String::new();
    for (t, r) in temps.iter().zip(right) {
        out.push_str(&format!("auto {t} = {r};"));
    }
    for (l, t) in left.iter().zip(temps) {
        out.push_str(&format!("{l} = {t};"));
    }
    Ok(Some(out))
}

pub fn grouped(s: &str) -> Result<String, Diagnostic> {
    let Some(open) = s.find('(') else {
        return Ok(s.into());
    };
    let tp = s[..open].trim();
    if ![
        "int",
        "long long",
        "double",
        "float",
        "char",
        "string",
        "auto",
        "unsigned",
        "bool",
    ]
    .contains(&tp)
    {
        return Ok(s.into());
    }
    let close = matching_close(s, open)?;
    if s[close + 1..].trim() == ";" {
        Ok(format!("{} {};", tp, &s[open + 1..close]))
    } else {
        Ok(s.into())
    }
}
