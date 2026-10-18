//! Byte offsets always refer to boundaries in the original UTF-8 source.
use crate::Diagnostic;

/// Mark source bytes that are code, excluding quoted literals and comments.
/// A newline terminating a line comment remains code for source tracking.
pub fn code_mask(source: &str) -> Result<Vec<bool>, Diagnostic> {
    let b = source.as_bytes();
    let mut mask = vec![true; b.len()];
    let mut i = 0;
    while i < b.len() {
        let start = i;
        if b[i..].starts_with(b"//") {
            while i < b.len() && b[i] != b'\n' {
                mask[i] = false;
                i += 1;
            }
        } else if b[i..].starts_with(b"/*") {
            i += 2;
            while i + 1 < b.len() && &b[i..i + 2] != b"*/" {
                i += 1;
            }
            if i + 1 >= b.len() {
                return Err(Diagnostic::at(source, start, "unterminated comment"));
            }
            i += 2;
            mask[start..i].fill(false);
        } else if b[i] == b'"' || b[i] == b'\'' {
            let quote = b[i];
            i += 1;
            let mut closed = false;
            while i < b.len() {
                if b[i] == b'\\' {
                    i = (i + 2).min(b.len());
                } else if b[i] == quote {
                    i += 1;
                    closed = true;
                    break;
                } else {
                    i += 1;
                }
            }
            if !closed {
                return Err(Diagnostic::at(source, start, "unterminated literal"));
            }
            mask[start..i].fill(false);
        } else {
            i += 1;
        }
    }
    Ok(mask)
}
fn closing(open: u8) -> Option<u8> {
    match open {
        b'(' => Some(b')'),
        b'[' => Some(b']'),
        b'{' => Some(b'}'),
        _ => None,
    }
}

pub fn matching_close(source: &str, open: usize) -> Result<usize, Diagnostic> {
    if !source.is_char_boundary(open) {
        return Err(Diagnostic::at(source, open, "invalid character boundary"));
    }
    let b = source.as_bytes();
    let Some(&first) = b.get(open) else {
        return Err(Diagnostic::at(source, open, "expected opening bracket"));
    };
    if closing(first).is_none() {
        return Err(Diagnostic::at(source, open, "expected opening bracket"));
    }
    let mask = code_mask(source)?;
    if !mask[open] {
        return Err(Diagnostic::at(
            source,
            open,
            "bracket is inside a literal or comment",
        ));
    }
    let mut stack = Vec::new();
    for i in open..b.len() {
        if !mask[i] {
            continue;
        }
        if let Some(close) = closing(b[i]) {
            stack.push(close);
        } else if b")] }".contains(&b[i]) && b[i] != b' ' {
            if stack.pop() != Some(b[i]) {
                return Err(Diagnostic::at(source, i, "mismatched bracket"));
            }
            if stack.is_empty() {
                return Ok(i);
            }
        }
    }
    Err(Diagnostic::at(source, open, "unclosed bracket"))
}

pub fn split_top_level(source: &str, delimiter: char) -> Result<Vec<&str>, Diagnostic> {
    let mask = code_mask(source)?;
    let mut stack = Vec::new();
    let mut result = Vec::new();
    let mut start = 0;
    for (i, ch) in source.char_indices() {
        if !mask[i] {
            continue;
        }
        if ch.is_ascii() {
            if let Some(close) = closing(ch as u8) {
                stack.push((close, i));
            } else if matches!(ch, ')' | ']' | '}') && stack.pop().map(|x| x.0) != Some(ch as u8) {
                return Err(Diagnostic::at(source, i, "mismatched bracket"));
            }
        }
        if ch == delimiter && stack.is_empty() {
            result.push(&source[start..i]);
            start = i + ch.len_utf8();
        }
    }
    if let Some((_, i)) = stack.last() {
        return Err(Diagnostic::at(source, *i, "unclosed bracket"));
    }
    result.push(&source[start..]);
    Ok(result)
}

pub fn is_identifier_byte(b: u8) -> bool {
    b.is_ascii_alphanumeric() || b == b'_' || b >= 128
}

/// Replace whole code identifiers while preserving comments and literals.
pub fn replace_identifier(source: &str, from: &str, to: &str) -> Result<String, Diagnostic> {
    let mask = code_mask(source)?;
    let mut out = String::new();
    let mut cursor = 0;
    for (i, _) in source.match_indices(from) {
        let end = i + from.len();
        if mask[i..end].iter().all(|&v| v)
            && (i == 0 || !is_identifier_byte(source.as_bytes()[i - 1]))
            && (end == source.len() || !is_identifier_byte(source.as_bytes()[end]))
        {
            out.push_str(&source[cursor..i]);
            out.push_str(to);
            cursor = end;
        }
    }
    out.push_str(&source[cursor..]);
    Ok(out)
}

/// Split macro arguments, also keeping template-ids together. Unlike ordinary
/// delimiters, angle brackets need lookahead: `<` and `>` can be comparisons.
pub fn split_arguments(source: &str) -> Result<Vec<&str>, Diagnostic> {
    let mask = code_mask(source)?;
    let bytes = source.as_bytes();
    let mut result = Vec::new();
    let mut start = 0;
    let mut i = 0;
    while i < bytes.len() {
        if !mask[i] {
            i += 1;
            continue;
        }
        match bytes[i] {
            b'(' | b'[' | b'{' => i = matching_close(source, i)? + 1,
            b'<' => {
                if let Some(close) = template_close(source, &mask, i)? {
                    i = close + 1;
                } else {
                    i += 1;
                }
            }
            b',' => {
                result.push(&source[start..i]);
                i += 1;
                start = i;
            }
            b')' | b']' | b'}' => {
                return Err(Diagnostic::at(source, i, "mismatched bracket"));
            }
            _ => i += 1,
        }
    }
    result.push(&source[start..]);
    Ok(result)
}

fn template_close(source: &str, mask: &[bool], open: usize) -> Result<Option<usize>, Diagnostic> {
    let b = source.as_bytes();
    let previous = (0..open)
        .rev()
        .find(|&i| mask[i] && !b[i].is_ascii_whitespace());
    if !previous.is_some_and(|i| is_identifier_byte(b[i]))
        || matches!(b.get(open + 1), Some(b'=' | b'<'))
        || (open > 0 && b[open - 1] == b'<')
    {
        return Ok(None);
    }
    let mut depth = 1;
    let mut i = open + 1;
    while i < b.len() {
        if !mask[i] {
            i += 1;
            continue;
        }
        match b[i] {
            b'(' | b'[' | b'{' => {
                i = matching_close(source, i)? + 1;
                continue;
            }
            b'<' => depth += 1,
            b'>' => {
                depth -= 1;
                if depth == 0 {
                    let next = (i + 1..b.len()).find(|&p| mask[p] && !b[p].is_ascii_whitespace());
                    // A following operand means this was a comparison, e.g.
                    // `a < b, c > d`, rather than a template-id.
                    return Ok(
                        if next.is_none_or(|p| {
                            matches!(
                                b[p],
                                b'(' | b'{' | b':' | b',' | b'>' | b')' | b']' | b'}' | b'.'
                            )
                        }) {
                            Some(i)
                        } else {
                            None
                        },
                    );
                }
            }
            b';' | b')' | b']' | b'}' => return Ok(None),
            _ => {}
        }
        i += 1;
    }
    Ok(None)
}
