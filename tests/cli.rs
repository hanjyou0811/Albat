use std::{
    io::Write,
    process::{Command, Stdio},
};
fn binary() -> Command {
    Command::new(env!("CARGO_BIN_EXE_albat"))
}
#[test]
fn stdin_translates_and_invalid_source_fails() {
    let mut p = binary()
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .unwrap();
    p.stdin.take().unwrap().write_all(b"cout<<3;").unwrap();
    let out = p.wait_with_output().unwrap();
    assert!(out.status.success());
    assert!(String::from_utf8(out.stdout)
        .unwrap()
        .contains("int main()"));
    let mut p = binary()
        .stdin(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .unwrap();
    p.stdin.take().unwrap().write_all(b"int main(){").unwrap();
    let out = p.wait_with_output().unwrap();
    assert!(!out.status.success());
}
#[test]
fn run_propagates_exit_status_and_handles_spaces() {
    let dir = tempfile::tempdir().unwrap();
    let file = dir.path().join("space ' name.al");
    std::fs::write(&file, "int main(){return 7;}").unwrap();
    let out = binary().arg("run").arg(&file).output().unwrap();
    assert_eq!(
        out.status.code(),
        Some(7),
        "{}",
        String::from_utf8_lossy(&out.stderr)
    );
    let out = binary()
        .arg("run")
        .arg(dir.path().join("missing.al"))
        .output()
        .unwrap();
    assert!(!out.status.success());
}
#[test]
fn export_signature_and_exec_files() {
    let mut p = binary()
        .args(["--export", "[\"int\",\"solve\"]"])
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .unwrap();
    p.stdin.take().unwrap().write_all(b"return 3;").unwrap();
    let out = p.wait_with_output().unwrap();
    assert!(out.status.success());
    assert!(String::from_utf8(out.stdout)
        .unwrap()
        .contains("int solve()"));
    let dir = tempfile::tempdir().unwrap();
    let source = dir.path().join("source.al");
    let input = dir.path().join("input");
    let output = dir.path().join("output");
    std::fs::write(&source, "int@a; $$(a+1);").unwrap();
    std::fs::write(&input, "3\n").unwrap();
    let result = binary()
        .arg("exec")
        .arg(source)
        .arg(input)
        .arg(&output)
        .output()
        .unwrap();
    assert!(
        result.status.success(),
        "{}",
        String::from_utf8_lossy(&result.stderr)
    );
    assert_eq!(std::fs::read_to_string(output).unwrap(), "4\n");
}
#[test]
fn compile_failure_is_nonzero() {
    let dir = tempfile::tempdir().unwrap();
    let file = dir.path().join("bad.al");
    std::fs::write(&file, "undefined_symbol();").unwrap();
    let out = binary().arg("run").arg(file).output().unwrap();
    assert!(!out.status.success());
}
#[test]
fn relative_include_and_missing_compiler() {
    let dir = tempfile::tempdir().unwrap();
    std::fs::write(dir.path().join("helper.al"), "int helper(){return 8;}").unwrap();
    std::fs::write(
        dir.path().join("main.al"),
        "#include helper.al\n{cout<<helper();}",
    )
    .unwrap();
    let out = binary()
        .current_dir(dir.path())
        .args(["run", "main.al"])
        .output()
        .unwrap();
    assert!(
        out.status.success(),
        "{}",
        String::from_utf8_lossy(&out.stderr)
    );
    assert_eq!(out.stdout, b"8");
    let out = binary()
        .current_dir(dir.path())
        .env("PATH", dir.path())
        .args(["run", "main.al"])
        .output()
        .unwrap();
    assert!(!out.status.success());
    assert!(String::from_utf8_lossy(&out.stderr).contains("cannot execute g++"));
}
#[cfg(target_os = "linux")]
#[test]
fn redirected_stdin_retains_source_filename() {
    let dir = tempfile::tempdir().unwrap();
    let file = dir.path().join("input.al");
    std::fs::write(&file, "cout<<1;").unwrap();
    let out = binary()
        .stdin(std::fs::File::open(&file).unwrap())
        .output()
        .unwrap();
    assert!(out.status.success());
    assert!(String::from_utf8(out.stdout)
        .unwrap()
        .contains(&format!("#line 1 \"{}\"", file.display())));
}
