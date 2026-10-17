use std::process::Command;
#[test]
fn executable_fixture_corpus() {
    let output = Command::new("python3")
        .args([
            "tools/compat.py",
            "--candidate",
            env!("CARGO_BIN_EXE_albat"),
        ])
        .current_dir(env!("CARGO_MANIFEST_DIR"))
        .output()
        .unwrap();
    assert!(
        output.status.success(),
        "{}\n{}",
        String::from_utf8_lossy(&output.stdout),
        String::from_utf8_lossy(&output.stderr)
    );
}
