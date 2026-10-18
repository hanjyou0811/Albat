use crate::{transpile, Diagnostic, Options, SourceLoader};
use std::{
    ffi::OsString,
    fs,
    io::{self, Read, Write},
    path::{Path, PathBuf},
    process::{Command, ExitStatus, Stdio},
};

pub struct FsSources {
    pub root: PathBuf,
}
impl SourceLoader for FsSources {
    fn load(&self, requested: &str, from: &str) -> Result<String, Diagnostic> {
        fs::read_to_string(self.root.join(requested)).map_err(|e| Diagnostic {
            source: from.into(),
            line: 1,
            column: 1,
            message: format!("cannot read {requested}: {e}"),
        })
    }
}
fn status_code(status: ExitStatus) -> i32 {
    if let Some(code) = status.code() {
        return code;
    }
    #[cfg(unix)]
    {
        use std::os::unix::process::ExitStatusExt;
        if let Some(signal) = status.signal() {
            return 128 + signal;
        }
    }
    1
}
fn export_signature(value: &str) -> Result<String, String> {
    let inner = value
        .trim()
        .strip_prefix('[')
        .and_then(|v| v.strip_suffix(']'))
        .ok_or("--export expects a list of quoted strings")?;
    let parts = crate::scan::split_top_level(inner, ',').map_err(|e| e.to_string())?;
    let parts: Vec<&str> = parts
        .into_iter()
        .map(|p| {
            p.trim()
                .strip_prefix('"')
                .and_then(|p| p.strip_suffix('"'))
                .ok_or("--export expects quoted strings")
        })
        .collect::<Result<_, _>>()?;
    if parts.len() < 2 {
        return Err("--export requires a return type and function name".into());
    }
    Ok(format!(
        "{} {}({})",
        parts[0],
        parts[1],
        parts[2..].join("")
    ))
}
fn source_name(path: &Path) -> String {
    if path.is_absolute() {
        if let Ok(cwd) = std::env::current_dir() {
            if let Ok(relative) = path.strip_prefix(cwd) {
                return relative.to_string_lossy().into();
            }
        }
    }
    path.to_string_lossy().into()
}
fn compile_run(path: &Path, input: Option<&Path>, output: Option<&Path>) -> Result<i32, String> {
    let source = fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display()))?;
    let options = Options {
        source_name: source_name(path),
        ..Default::default()
    };
    let loader = FsSources {
        root: std::env::current_dir().map_err(|e| e.to_string())?,
    };
    let cpp = transpile(&source, &options, &loader).map_err(|e| e.to_string())?;
    let temp = tempfile::Builder::new()
        .prefix("albat-")
        .tempdir()
        .map_err(|e| e.to_string())?;
    let source = temp.path().join("program.cpp");
    let binary = temp.path().join("program");
    fs::write(&source, cpp).map_err(|e| e.to_string())?;
    let status = Command::new("g++")
        .args(["-O2", "-std=gnu++20", "-o"])
        .arg(&binary)
        .arg(&source)
        .status()
        .map_err(|e| format!("cannot execute g++: {e}"))?;
    if !status.success() {
        return Ok(status_code(status));
    }
    let mut cmd = Command::new(&binary);
    if let Some(input) = input {
        cmd.stdin(Stdio::from(
            fs::File::open(input).map_err(|e| e.to_string())?,
        ));
    }
    if let Some(output) = output {
        cmd.stdout(Stdio::from(
            fs::File::create(output).map_err(|e| e.to_string())?,
        ));
    }
    Ok(status_code(cmd.status().map_err(|e| e.to_string())?))
}
pub fn run(args: impl Iterator<Item = OsString>) -> i32 {
    match execute(args.collect()) {
        Ok(code) => code,
        Err(error) => {
            eprintln!("albat: {error}");
            1
        }
    }
}
fn execute(args: Vec<OsString>) -> Result<i32, String> {
    let mut options = Options::default();
    match args.first().and_then(|s| s.to_str()) {
        Some("run") if args.len() == 2 => return compile_run(Path::new(&args[1]), None, None),
        Some("exec") if args.len() == 4 => {
            return compile_run(
                Path::new(&args[1]),
                Some(Path::new(&args[2])),
                Some(Path::new(&args[3])),
            )
        }
        Some("--export") if args.len() == 2 => {
            options.main_signature = export_signature(&args[1].to_string_lossy())?;
        }
        Some(arg) if arg.starts_with("--export=") && args.len() == 1 => {
            options.main_signature = export_signature(&arg[9..])?;
        }
        Some("--help") | Some("-h") => {
            println!("Usage: albat [--export '[\"type\",\"name\",\"args\"]'] < source.al\n       albat run <source.al>\n       albat exec <source.al> <input> <output>");
            return Ok(0);
        }
        None => {}
        _ => return Err(
            "Usage: albat [--export value] | run <source.al> | exec <source.al> <input> <output>"
                .into(),
        ),
    }
    #[cfg(target_os = "linux")]
    if let Ok(path) = fs::read_link("/proc/self/fd/0") {
        if path.is_absolute() {
            options.source_name = source_name(&path);
        }
    }
    let mut source = String::new();
    io::stdin()
        .read_to_string(&mut source)
        .map_err(|e| e.to_string())?;
    let loader = FsSources {
        root: std::env::current_dir().map_err(|e| e.to_string())?,
    };
    let cpp = transpile(&source, &options, &loader).map_err(|e| e.to_string())?;
    io::stdout()
        .write_all(cpp.as_bytes())
        .map_err(|e| e.to_string())?;
    Ok(0)
}
