use crate::{
    library::LibraryManager,
    scan::{code_mask, is_identifier_byte, matching_close, split_top_level},
    scope::Scope,
    Diagnostic,
};
fn unwrap_parens(s: &str) -> Result<&str, Diagnostic> {
    let s = s.trim();
    if s.starts_with('(') && matching_close(s, 0)? == s.len() - 1 {
        Ok(&s[1..s.len() - 1])
    } else {
        Ok(s)
    }
}
pub fn rewrite(
    s: &str,
    scope: &mut Scope,
    libs: &mut LibraryManager,
) -> Result<Option<String>, Diagnostic> {
    let s = s.trim();
    let body = s.trim_end_matches(';').trim();
    if body.starts_with('$') {
        let n = body.bytes().take_while(|&b| b == b'$').count();
        let argument = body[n..].trim();
        let grouped =
            argument.starts_with('(') && matching_close(argument, 0)? == argument.len() - 1;
        let mut args = split_top_level(unwrap_parens(argument)?, ',')?;
        // Legacy parses bare comma tails as ordinary statements; parentheses opt
        // into a list of output values.
        let tail = if !grouped && args.len() > 1 {
            args.split_off(1)
        } else {
            Vec::new()
        };
        let mut sep = None;
        let mut values = Vec::new();
        for arg in args {
            let arg = arg.trim();
            if let Some(rest) = arg
                .strip_prefix("sep")
                .and_then(|s| s.trim_start().strip_prefix('='))
            {
                sep = Some(rest.trim());
            } else if !arg.is_empty() {
                values.push(arg);
            }
        }
        libs.set_enabled("FastIO", true);
        let call = if n == 1 { "wt" } else { "wtln" };
        let mut out = String::new();
        for v in values {
            out.push_str(&format!(
                "{}({}{});",
                call,
                v,
                sep.map(|s| format!(", {s}")).unwrap_or_default()
            ));
        }
        for statement in tail {
            out.push_str(statement);
            out.push(';');
        }
        return Ok(Some(out));
    }
    let mask = code_mask(body)?;
    let Some(at) = body
        .bytes()
        .enumerate()
        .find(|(i, b)| *b == b'@' && mask[*i])
        .map(|x| x.0)
    else {
        return Ok(None);
    };
    let tp = body[..at].trim();
    if tp.contains('(') {
        return Ok(None);
    }
    let mut vars = body[at + 1..].trim();
    let mut count = None;
    if vars.ends_with(']') {
        if let Some(start) = vars.rfind("[size") {
            let option = &vars[start + 1..vars.len() - 1];
            if let Some((_, value)) = option.split_once('=') {
                count = Some(value.trim());
                vars = vars[..start].trim();
            }
        }
    }
    vars = unwrap_parens(vars)?;
    let parts = split_top_level(vars, ',')?;
    let mut definitions = Vec::new();
    let mut reads = String::new();
    let index = count.map(|_| scope.fresh_name());
    for v in parts {
        let v = v.trim();
        let skip = v.starts_with('!');
        let v = v.trim_start_matches('!').trim();
        let mut end = 0;
        while end < v.len() && is_identifier_byte(v.as_bytes()[end]) {
            end += 1;
        }
        let name = if end == 0 {
            scope.fresh_name()
        } else {
            v[..end].into()
        };
        let has_change = v.contains("++") || v.contains("--");
        if !tp.is_empty() {
            let decl = if end == 0 {
                name.clone()
            } else {
                v.replace("++", "").replace("--", "")
            };
            definitions.push(decl);
        }
        if !skip {
            let target = if let Some(idx) = &index {
                format!("{name}[{idx}]")
            } else {
                name.clone()
            };
            reads.push_str(&format!("rd({target});"));
            if has_change {
                // Characterize the old setup_Line behavior: both ++ and -- decrement.
                if tp.starts_with("vector")
                    || matches!(tp, "vi" | "VI" | "vll" | "VLL" | "vl" | "VL")
                {
                    let item = scope.fresh_name();
                    reads.push_str(&format!("for(auto &{item} : {name}){{{item}--;}}"));
                } else {
                    reads.push_str(&format!("{target}--;"));
                }
            }
        }
    }
    if let (Some(count), Some(index)) = (count, index) {
        reads = format!("for(int {index}=0;{index}<{count};{index}++){{{reads}}}");
    }
    libs.set_enabled("FastIO", true);
    Ok(Some(format!(
        "{}{}",
        if definitions.is_empty() {
            String::new()
        } else {
            format!("{tp} {};", definitions.join(", "))
        },
        reads
    )))
}
/// Lift control-header reads ahead of the loop, preserving ++/-- in its condition.
pub fn condition(
    s: &str,
    scope: &mut Scope,
    libs: &mut LibraryManager,
) -> Result<(String, String), Diagnostic> {
    let mut header = s.to_owned();
    let mut prelude = String::new();
    loop {
        let mask = code_mask(&header)?;
        let Some(at) = header
            .bytes()
            .enumerate()
            .find(|(i, b)| *b == b'@' && mask[*i])
            .map(|x| x.0)
        else {
            break;
        };
        let mut start = at;
        while start > 0 && is_identifier_byte(header.as_bytes()[start - 1]) {
            start -= 1;
        }
        let tp = &header[start..at];
        let mut end = at + 1;
        while end < header.len() && is_identifier_byte(header.as_bytes()[end]) {
            end += 1;
        }
        let name = if end == at + 1 {
            scope.fresh_name()
        } else {
            header[at + 1..end].into()
        };
        if !tp.is_empty() {
            prelude.push_str(&format!("{tp} {name};"));
        }
        prelude.push_str(&format!("rd({name});"));
        header.replace_range(start..end, &name);
        libs.set_enabled("FastIO", true);
    }
    Ok((prelude, header))
}
