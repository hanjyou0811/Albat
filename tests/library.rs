use albat::{
    library::{LibraryManager, Position, CATALOG},
    LibraryOptions,
};

#[test]
fn catalog_is_complete_and_ordered() {
    assert_eq!(CATALOG.len(), 67);
    assert_eq!(CATALOG[0].name, "pragma");
    assert_eq!(CATALOG.last().unwrap().name, "rtrim_vec");
    let names: std::collections::BTreeSet<_> = CATALOG.iter().map(|e| e.name).collect();
    assert_eq!(names.len(), CATALOG.len());
}
#[test]
fn defaults_and_dependency_order() {
    let mut libs = LibraryManager::new(&LibraryOptions::default());
    let head = libs.emit(Position::Head).unwrap();
    assert!(head.contains("#include <bits/stdc++.h>"));
    assert!(head.contains("#pragma GCC optimize"));
    libs.request("mint", false);
    assert!(libs
        .emit(Position::Head)
        .unwrap()
        .contains("#define mod 1000000009"));
    assert!(libs.emit(Position::Main).unwrap().contains("x.set(mod)"));
}
#[test]
fn requests_are_local_and_exclusions_win() {
    let mut a = LibraryManager::new(&LibraryOptions::default());
    let b = LibraryManager::new(&LibraryOptions::default());
    a.request("_pow", false);
    a.request("_pow", false);
    assert_eq!(
        a.emit(Position::Head)
            .unwrap()
            .matches("inline double _pow")
            .count(),
        1
    );
    assert!(!b.emit(Position::Head).unwrap().contains("_pow"));
    a.request("_pow", true);
    assert!(!a.emit(Position::Head).unwrap().contains("_pow"));
}
#[test]
fn flags_and_integer_configuration() {
    let options = LibraryOptions {
        pragma: false,
        stdcpp: false,
        namespace_std: false,
        namespace_acl: false,
        define_int_ll: 2,
    };
    let mut libs = LibraryManager::new(&options);
    assert_eq!(libs.emit(Position::Head).unwrap(), "");
    libs.request("DEFINE_INT_LL", false);
    assert!(libs
        .emit(Position::Head)
        .unwrap()
        .contains("#define int long long"));
    assert_eq!(libs.emit(Position::Main).unwrap(), "");
    libs.set_enabled("FastIO_flush", true);
    assert!(libs.is_enabled("FastIO_flush"));
}
