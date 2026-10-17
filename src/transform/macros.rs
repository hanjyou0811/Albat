use crate::{
    scan::{code_mask, is_identifier_byte, matching_close, split_top_level},
    Diagnostic,
};
pub fn rewrite(source: &str) -> Result<String, Diagnostic> {
    let mut s = source.to_owned();
    let names = [
        "rep",
        "REP",
        "rrep",
        "RREP",
        "fore",
        "all",
        "len",
        "Count",
        "_upper_bound",
        "_lower_bound",
    ];
    loop {
        let mask = code_mask(&s)?;
        let mut found = None;
        for (i, _) in s.char_indices() {
            if !mask[i] || (i > 0 && is_identifier_byte(s.as_bytes()[i - 1])) {
                continue;
            }
            for name in names {
                if !s[i..].starts_with(name) {
                    continue;
                }
                let end = i + name.len();
                if s.as_bytes()
                    .get(end)
                    .is_some_and(|&b| is_identifier_byte(b))
                {
                    continue;
                }
                let rest = &s[end..];
                let open = end + rest.len() - rest.trim_start().len();
                if s.as_bytes().get(open) != Some(&b'(') {
                    continue;
                }
                let close = matching_close(&s, open)?;
                let args = split_top_level(&s[open + 1..close], ',')?;
                let mut a: Vec<String> = args
                    .iter()
                    .map(|a| rewrite(a.trim()))
                    .collect::<Result<_, _>>()?;
                let result = match name {
                    "fore" if a.len() == 2 => format!("for(auto &{} : {})", a[0], a[1]),
                    "rep" | "REP" | "rrep" | "RREP" if (2..=4).contains(&a.len()) => {
                        if a.len() == 2 {
                            a.insert(1, "0".into());
                        }
                        let step = if a.len() == 4 {
                            a[3].clone()
                        } else {
                            "1".into()
                        };
                        if name.eq_ignore_ascii_case("rep") {
                            format!(
                                "for(int {} = {}; {} < {}; {} += {})",
                                a[0], a[1], a[0], a[2], a[0], step
                            )
                        } else {
                            format!(
                                "for(int {} = {}-1; {} >= {}; {} -= {})",
                                a[0], a[2], a[0], a[1], a[0], step
                            )
                        }
                    }
                    "all" if a.len() == 1 => format!("({0}).begin(), ({0}).end()", a[0]),
                    "len" if a.len() == 1 => format!("(long long)({}).size()", a[0]),
                    "Count" if a.len() == 2 => {
                        format!("count({0}.begin(), {0}.end(), {1})", a[0], a[1])
                    }
                    "_upper_bound" | "_lower_bound" => {
                        let function = &name[1..];
                        match a.len() {
                            1 => format!("{}({})", function, a[0]),
                            2 => format!("{}.{}({})", a[0], function, a[1]),
                            3 => format!(
                                "{}.{}({})",
                                a[0].split('.').next().unwrap_or(&a[0]),
                                function,
                                a[2]
                            ),
                            _ => return Err(Diagnostic::at(&s, i, "invalid bound macro arity")),
                        }
                    }
                    _ => return Err(Diagnostic::at(&s, i, "invalid macro arity")),
                };
                found = Some((i, close + 1, result));
                break;
            }
            if found.is_some() {
                break;
            }
        }
        if let Some((start, end, replacement)) = found {
            s.replace_range(start..end, &replacement);
        } else {
            break;
        }
    }
    Ok(s)
}
