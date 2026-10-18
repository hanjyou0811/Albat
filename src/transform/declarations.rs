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

pub fn grouped(s: &str, scope: &Scope) -> Result<String, Diagnostic> {
    let Some(open) = s.find('(') else {
        return Ok(s.into());
    };
    let tp = s[..open].trim();
    if !scope.is_type(tp) {
        return Ok(s.into());
    }
    let close = matching_close(s, open)?;
    let contents = &s[open + 1..close];
    let declarators = split_top_level(contents, ',')?;
    if s[close + 1..].trim() == ";"
        && declarators
            .iter()
            .all(|part| is_declarator(part).unwrap_or(false))
    {
        Ok(format!("{} {};", tp, &s[open + 1..close]))
    } else {
        Ok(s.into())
    }
}

// A type followed by parentheses can also construct a temporary. Only unwrap
// a list of named declarators; literals and general expressions stay intact.
fn is_declarator(part: &str) -> Result<bool, Diagnostic> {
    let mut part = part.trim().trim_start_matches(['*', '&']).trim_start();
    let Some(first) = part.chars().next() else {
        return Ok(false);
    };
    if !(first.is_alphabetic() || first == '_') {
        return Ok(false);
    }
    let end = part
        .bytes()
        .take_while(|&b| crate::scan::is_identifier_byte(b))
        .count();
    if ["true", "false", "nullptr", "this"].contains(&&part[..end]) {
        return Ok(false);
    }
    part = part[end..].trim_start();
    loop {
        if part.is_empty() {
            return Ok(true);
        }
        if part.starts_with('=') {
            return Ok(!part.starts_with("=="));
        }
        if part.starts_with(['[', '(', '{']) {
            let close = matching_close(part, 0)?;
            part = part[close + 1..].trim_start();
        } else {
            return Ok(false);
        }
    }
}
