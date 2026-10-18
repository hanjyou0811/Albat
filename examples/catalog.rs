use albat::library::{Position, CATALOG};
fn main() {
    for entry in CATALOG {
        let body: String = entry.body.bytes().map(|b| format!("{b:02x}")).collect();
        println!(
            "{}\t{}\t{}\t{}",
            entry.name,
            body,
            if entry.position == Position::Head {
                "head"
            } else {
                "header_of_main"
            },
            entry.dependencies.join(",")
        );
    }
}
