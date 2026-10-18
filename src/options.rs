#[derive(Clone, Debug)]
pub struct LibraryOptions {
    pub pragma: bool,
    pub stdcpp: bool,
    pub namespace_std: bool,
    pub namespace_acl: bool,
    pub define_int_ll: u8,
}
impl Default for LibraryOptions {
    fn default() -> Self {
        Self {
            pragma: true,
            stdcpp: true,
            namespace_std: true,
            namespace_acl: true,
            define_int_ll: 1,
        }
    }
}
#[derive(Clone, Debug)]
pub struct Options {
    pub source_name: String,
    pub main_signature: String,
    pub libraries: LibraryOptions,
}
impl Default for Options {
    fn default() -> Self {
        Self {
            source_name: "<stdin>".into(),
            main_signature: "int main()".into(),
            libraries: LibraryOptions::default(),
        }
    }
}
