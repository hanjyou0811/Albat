use crate::{
    library::LibraryManager,
    scan::{code_mask, is_identifier_byte, matching_close},
    Diagnostic,
};
fn left_start(s: &str, end: usize) -> Result<usize, Diagnostic> {
    let b = s.as_bytes();
    let mut p = end;
    while p > 0 && b[p - 1].is_ascii_whitespace() {
        p -= 1;
    }
    if p > 0 && matches!(b[p - 1], b')' | b']') {
        let close = p - 1;
        let mask = code_mask(s)?;
        let mut stack = Vec::new();
        let mut matched = None;
        for i in 0..=close {
            if !mask[i] {
                continue;
            }
            if matches!(b[i], b'(' | b'[' | b'{') {
                stack.push(i);
            } else if matches!(b[i], b')' | b']' | b'}') {
                let open = stack.pop();
                if i == close {
                    matched = open;
                }
            }
        }
        p = matched.ok_or_else(|| Diagnostic::at(s, close, "unmatched operand bracket"))?;
    }
    while p > 0 && (is_identifier_byte(b[p - 1]) || b[p - 1] == b'.') {
        p -= 1;
    }
    Ok(p)
}
fn right_end(s: &str, start: usize) -> Result<usize, Diagnostic> {
    let b = s.as_bytes();
    let mut p = start;
    while p < b.len() && b[p].is_ascii_whitespace() {
        p += 1;
    }
    while p < b.len() && matches!(b[p], b'+' | b'-' | b'!' | b'~' | b'*' | b'&') {
        p += 1;
    }
    if p < b.len() && b[p] == b'(' {
        p = matching_close(s, p)? + 1;
    } else {
        while p < b.len() && (is_identifier_byte(b[p]) || b[p] == b'.') {
            p += 1;
        }
    }
    loop {
        let before = p;
        while p < b.len() && b[p].is_ascii_whitespace() {
            p += 1;
        }
        if p < b.len() && matches!(b[p], b'(' | b'[') {
            p = matching_close(s, p)? + 1;
        } else {
            return Ok(before);
        }
    }
}
pub fn rewrite(source: &str, libs: &mut LibraryManager) -> Result<String, Diagnostic> {
    let mut s = source.to_owned();
    for (symbol, function) in [
        ("**=", "_pow"),
        ("/+=", "_div"),
        ("%+=", "_mod"),
        ("**", "_pow"),
        ("/+", "_div"),
        ("%+", "_mod"),
    ] {
        let mut search = 0;
        loop {
            let mask = code_mask(&s)?;
            let found = s[search..]
                .match_indices(symbol)
                .map(|(i, _)| search + i)
                .find(|&i| mask[i..i + symbol.len()].iter().all(|&v| v));
            let Some(pos) = found else {
                break;
            };
            let left = left_start(&s, pos)?;
            if left == pos {
                search = pos + symbol.len();
                continue;
            }
            let operand = s[left..pos].trim();
            if matches!(
                operand,
                "int" | "long" | "char" | "float" | "double" | "void" | "auto"
            ) {
                search = pos + symbol.len();
                continue;
            }
            let start = pos + symbol.len();
            let mut end = right_end(&s, start)?;
            if symbol.ends_with('=') {
                let mut i = start;
                while i < s.len() {
                    if !mask[i] {
                        i += 1;
                        continue;
                    }
                    if matches!(s.as_bytes()[i], b'(' | b'[' | b'{') {
                        i = matching_close(&s, i)? + 1;
                        continue;
                    }
                    if matches!(s.as_bytes()[i], b';' | b',' | b')' | b']' | b'}') {
                        break;
                    }
                    i += 1;
                }
                end = i;
            }
            if end <= start {
                return Err(Diagnostic::at(&s, start, "missing operator operand"));
            }
            let right = s[start..end].trim();
            let replacement = if symbol.ends_with('=') {
                format!("{operand} = {function}({operand}, {right})")
            } else {
                format!("({function}({operand}, {right}))")
            };
            s.replace_range(left..end, &replacement);
            libs.request(function, false);
            search = 0;
        }
    }
    Ok(s)
}
