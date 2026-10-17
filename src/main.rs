#[cfg(not(target_arch = "wasm32"))]
fn main() {
    std::process::exit(albat::cli::run(std::env::args_os().skip(1)));
}
#[cfg(target_arch = "wasm32")]
fn main() {}
