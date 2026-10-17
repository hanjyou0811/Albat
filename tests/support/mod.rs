use std::{
    fs,
    io::Write,
    process::{Command, Stdio},
    sync::atomic::{AtomicU64, Ordering},
};
static NEXT: AtomicU64 = AtomicU64::new(0);

pub fn run_cpp(cpp: &str, input: &str) -> String {
    let dir = std::env::temp_dir().join(format!(
        "albat-test-{}-{}",
        std::process::id(),
        NEXT.fetch_add(1, Ordering::Relaxed)
    ));
    fs::create_dir(&dir).unwrap();
    struct Cleanup(std::path::PathBuf);
    impl Drop for Cleanup {
        fn drop(&mut self) {
            let _ = fs::remove_dir_all(&self.0);
        }
    }
    let _cleanup = Cleanup(dir.clone());
    let source = dir.join("program.cpp");
    let binary = dir.join("program");
    fs::write(&source, cpp).unwrap();
    let output = Command::new("g++")
        .args(["-std=gnu++20"])
        .arg(&source)
        .arg("-o")
        .arg(&binary)
        .output()
        .unwrap();
    assert!(
        output.status.success(),
        "{}\nGenerated:\n{}",
        String::from_utf8_lossy(&output.stderr),
        cpp
    );
    let mut child = Command::new("timeout")
        .arg("5")
        .arg(&binary)
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .unwrap();
    child
        .stdin
        .take()
        .unwrap()
        .write_all(input.as_bytes())
        .unwrap();
    let output = child.wait_with_output().unwrap();
    assert!(
        output.status.success(),
        "{:?}: {}",
        output.status,
        String::from_utf8_lossy(&output.stderr)
    );
    String::from_utf8(output.stdout).unwrap()
}
pub fn translate(source: &str) -> String {
    albat::transpile(source, &albat::Options::default(), &albat::NoSources).unwrap()
}
