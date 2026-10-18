use crate::{
    ir::{Node, Program},
    library::LibraryManager,
    scan::{code_mask, is_identifier_byte, matching_close},
    Diagnostic, Options, SourceLoader,
};

pub fn parse(
    source: &str,
    options: &Options,
    loader: &dyn SourceLoader,
    libraries: &mut LibraryManager,
) -> Result<Program, Diagnostic> {
    let mut parser = Parser {
        source,
        file: &options.source_name,
        cursor: 0,
        loader,
        libraries,
        includes: vec![options.source_name.clone()],
    };
    let mut nodes = parser.nodes(false).map_err(|mut error| {
        if error.source == "<stdin>" {
            error.source = options.source_name.clone();
        }
        error
    })?;
    let has_main = nodes.iter().any(|n| {
        n.children.is_some()
            && n.suffix != "@include"
            && (n.text.is_empty() || contains_main(&n.text))
    });
    if !has_main {
        nodes = vec![Node {
            text: options.main_signature.clone(),
            children: Some(nodes),
            suffix: String::new(),
            source: options.source_name.clone(),
            line: 1,
            function: true,
        }];
    } else {
        let mut created = false;
        for n in &mut nodes {
            if n.text.is_empty() && n.children.is_some() && n.suffix != "@include" && !created {
                n.text = options.main_signature.clone();
                n.function = true;
                created = true;
            }
        }
    }
    Ok(Program { nodes })
}
pub fn contains_main(s: &str) -> bool {
    let Ok(mask) = code_mask(s) else {
        return false;
    };
    s.match_indices("main").any(|(i, _)| {
        mask[i..i + 4].iter().all(|&code| code)
            && (i == 0 || !is_identifier_byte(s.as_bytes()[i - 1]))
            && s[i + 4..].trim_start().starts_with('(')
    })
}
struct Parser<'a> {
    source: &'a str,
    file: &'a str,
    cursor: usize,
    loader: &'a dyn SourceLoader,
    libraries: &'a mut LibraryManager,
    includes: Vec<String>,
}
impl Parser<'_> {
    fn error(&self, pos: usize, message: &str) -> Diagnostic {
        let mut e = Diagnostic::at(self.source, pos, message);
        e.source = self.file.into();
        e
    }
    fn skip(&mut self) -> Result<(), Diagnostic> {
        loop {
            while self.cursor < self.source.len()
                && self.source.as_bytes()[self.cursor].is_ascii_whitespace()
            {
                self.cursor += 1;
            }
            let s = &self.source[self.cursor..];
            if s.starts_with("//") {
                let end = s.find('\n').unwrap_or(s.len());
                if s[..end].trim() == "//interactive" {
                    self.libraries.set_enabled("FastIO_flush", true);
                }
                self.cursor += end;
            } else if s.starts_with("/*") {
                let end = s
                    .find("*/")
                    .ok_or_else(|| self.error(self.cursor, "unterminated comment"))?;
                self.cursor += end + 2;
            } else {
                break;
            }
        }
        Ok(())
    }
    fn nodes(&mut self, stop_at_close: bool) -> Result<Vec<Node>, Diagnostic> {
        let mut out = Vec::new();
        loop {
            self.skip()?;
            if self.cursor == self.source.len() {
                if stop_at_close {
                    return Err(self.error(self.cursor, "unclosed block"));
                }
                return Ok(out);
            }
            if self.source.as_bytes()[self.cursor] == b'}' {
                if !stop_at_close {
                    return Err(self.error(self.cursor, "unexpected closing brace"));
                }
                self.cursor += 1;
                return Ok(out);
            }
            let before = self.cursor;
            out.push(self.node()?);
            if self.cursor <= before {
                return Err(self.error(before, "parser made no progress"));
            }
        }
    }
    fn node(&mut self) -> Result<Node, Diagnostic> {
        self.skip()?;
        let start = self.cursor;
        let line = self.source[..start].bytes().filter(|&b| b == b'\n').count() + 1;
        let mut node = Node {
            text: String::new(),
            children: None,
            suffix: String::new(),
            source: self.file.into(),
            line,
            function: false,
        };
        let s = &self.source[start..];
        if s.starts_with('#') {
            let end = s.find('\n').unwrap_or(s.len());
            node.text = s[..end].into();
            self.cursor += end;
            let directive = s[1..end].trim();
            if let Some(name) = directive.strip_prefix("include ") {
                let name = name.trim();
                if !name.starts_with('<') && !name.starts_with('"') {
                    if self.includes.iter().any(|p| p == name) {
                        return Err(self.error(start, "cyclic include"));
                    }
                    let loaded = self.loader.load(name, self.file)?;
                    let mut chain = self.includes.clone();
                    chain.push(name.into());
                    let mut child = Parser {
                        source: &loaded,
                        file: name,
                        cursor: 0,
                        loader: self.loader,
                        libraries: self.libraries,
                        includes: chain,
                    };
                    node.text = String::new();
                    node.children = Some(child.nodes(false).map_err(|mut error| {
                        if error.source == "<stdin>" {
                            error.source = name.into();
                        }
                        error
                    })?);
                    // An include is a transparent collection, not a C++ scope.
                    node.suffix = "@include".into();
                }
            }
            return Ok(node);
        }
        if s.starts_with('{') {
            self.cursor += 1;
            node.children = Some(self.nodes(true)?);
            return Ok(node);
        }
        let word_end = s.bytes().take_while(|&b| is_identifier_byte(b)).count();
        let word = &s[..word_end];
        let controls = [
            "if", "for", "while", "switch", "rep", "REP", "rrep", "RREP", "fore", "do_perm",
        ];
        if controls.contains(&word) {
            let open = start + word_end + s[word_end..].len() - s[word_end..].trim_start().len();
            if self.source.as_bytes().get(open) != Some(&b'(') {
                return Err(self.error(open, "expected control condition"));
            }
            let close = matching_close(self.source, open)?;
            node.text = self.source[start..=close].into();
            self.cursor = close + 1;
            self.skip()?;
            if self.source.as_bytes().get(self.cursor) == Some(&b'{') {
                self.cursor += 1;
                node.children = Some(self.nodes(true)?);
            } else {
                node.children = Some(vec![self.node()?]);
            }
            return Ok(node);
        }
        if word == "else" || word == "do" {
            node.text = word.into();
            self.cursor += word_end;
            self.skip()?;
            // Keep an else-if header at the same level as the preceding if.
            // Wrapping it in an else block would detach a subsequent else.
            let rest = &self.source[self.cursor..];
            if word == "else"
                && rest.strip_prefix("if").is_some_and(|tail| {
                    tail.as_bytes()
                        .first()
                        .is_none_or(|&b| !is_identifier_byte(b))
                })
            {
                let mut branch = self.node()?;
                branch.text = format!("else {}", branch.text);
                branch.line = line;
                return Ok(branch);
            }
            if self.source.as_bytes().get(self.cursor) == Some(&b'{') {
                self.cursor += 1;
                node.children = Some(self.nodes(true)?);
            } else {
                node.children = Some(vec![self.node()?]);
            }
            if word == "do" {
                self.skip()?;
                if !self.source[self.cursor..].starts_with("while") {
                    return Err(self.error(self.cursor, "expected while after do"));
                }
                let tail = self.cursor;
                self.cursor += 5;
                self.skip()?;
                let close = matching_close(self.source, self.cursor)?;
                self.cursor = close + 1;
                self.skip()?;
                if self.source.as_bytes().get(self.cursor) != Some(&b';') {
                    return Err(self.error(self.cursor, "expected semicolon after do-while"));
                }
                self.cursor += 1;
                node.suffix = format!(" {}", &self.source[tail..self.cursor]);
            }
            return Ok(node);
        }
        let mask = code_mask(self.source)?;
        let bytes = self.source.as_bytes();
        let mut i = start;
        while i < bytes.len() {
            if !mask[i] {
                i += 1;
                continue;
            }
            match bytes[i] {
                b'(' | b'[' => {
                    i = matching_close(self.source, i)? + 1;
                    continue;
                }
                b'{' => {
                    let head = self.source[start..i].trim();
                    let structural = matches!(
                        leading_keyword(head),
                        "struct" | "class" | "namespace" | "enum"
                    );
                    let function = head.contains(')')
                        && (crate::scan::split_top_level(head, '=')?.len() == 1
                            || head.contains("operator"));
                    if structural || function {
                        node.text = head.into();
                        node.function = function && !structural;
                        self.cursor = i + 1;
                        node.children = Some(self.nodes(true)?);
                        if structural && word != "namespace" {
                            self.skip()?;
                            if self.source.as_bytes().get(self.cursor) == Some(&b';') {
                                self.cursor += 1;
                                node.suffix = ";".into();
                            }
                        }
                        return Ok(node);
                    }
                    i = matching_close(self.source, i)? + 1;
                    continue;
                }
                b';' => {
                    node.text = self.source[start..=i].trim().into();
                    self.cursor = i + 1;
                    return Ok(node);
                }
                b')' | b']' => {
                    return Err(self.error(i, "unexpected closing delimiter"));
                }
                b'}' => {
                    return Err(self.error(i, "expected semicolon before closing brace"));
                }
                _ => {}
            }
            i += 1;
        }
        Err(self.error(start, "expected semicolon or block"))
    }
}

pub fn parse_fragment(
    source: &str,
    file: &str,
    loader: &dyn SourceLoader,
    libraries: &mut LibraryManager,
) -> Result<Vec<Node>, Diagnostic> {
    Parser {
        source,
        file,
        cursor: 0,
        loader,
        libraries,
        includes: vec![file.into()],
    }
    .nodes(false)
}

fn leading_keyword(mut source: &str) -> &str {
    source = source.trim_start();
    while let Some(rest) = source.strip_prefix("template") {
        let rest = rest.trim_start();
        if !rest.starts_with('<') {
            break;
        }
        let mut depth = 0;
        let mut end = None;
        for (i, ch) in rest.char_indices() {
            if ch == '<' {
                depth += 1;
            }
            if ch == '>' {
                depth -= 1;
                if depth == 0 {
                    end = Some(i + 1);
                    break;
                }
            }
        }
        let Some(end) = end else {
            break;
        };
        source = rest[end..].trim_start();
    }
    source
        .split(|c: char| !(c.is_alphanumeric() || c == '_'))
        .next()
        .unwrap_or("")
}
