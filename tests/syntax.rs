mod support;
use support::{run_cpp, translate};

#[test]
fn implicit_main_preserves_basic_statements() {
    assert_eq!(run_cpp(&translate("int a = 1; cout << a;"), ""), "1");
}
#[test]
fn functions_and_nested_control_flow() {
    let code = "{ cout << twice(3); } int twice(int x){ if(x>0) return x*2; else return 0; }";
    assert_eq!(run_cpp(&translate(code), ""), "6");
}
#[test]
fn explicit_main_and_empty_input() {
    assert_eq!(run_cpp(&translate("int main(){cout << 4;}"), ""), "4");
    assert_eq!(run_cpp(&translate(""), ""), "");
}
#[test]
fn nested_types_and_lambda() {
    let source="struct Box { int value; }; { Box b{3}; auto f = [](int x){return x+1;}; cout << f(b.value); }";
    assert_eq!(run_cpp(&translate(source), ""), "4");
}
#[test]
fn malformed_source_reports_location() {
    let e = albat::transpile(
        "int main(){\n if(1){",
        &albat::Options::default(),
        &albat::NoSources,
    )
    .unwrap_err();
    assert!(e.line >= 1);
    assert!(e.message.contains("unclosed"));
}
#[test]
fn filenames_are_escaped_and_lines_preserved() {
    let options = albat::Options {
        source_name: "a\"b.al".into(),
        ..Default::default()
    };
    let cpp = albat::transpile("\nint a=1;\ncout<<a;", &options, &albat::NoSources).unwrap();
    assert!(cpp.contains("#line 2 \"a\\\"b.al\""));
    assert_eq!(run_cpp(&cpp, ""), "1");
}

#[test]
fn templates_and_default_arguments() {
    let source = "int add(int x,int y=2){return x+y;} { cout << add(3); }";
    assert_eq!(run_cpp(&translate(source), ""), "5");
    let source = "template<class T> T twice(T x){return x+x;} { cout<<twice(3); }";
    assert_eq!(run_cpp(&translate(source), ""), "6");
}
#[test]
fn include_uses_loader_and_rejects_cycles() {
    struct Files;
    impl albat::SourceLoader for Files {
        fn load(&self, name: &str, _from: &str) -> Result<String, albat::Diagnostic> {
            Ok(match name {
                "helper.al" => "int helper(){return 3;}",
                "loop.al" => "#include loop.al",
                _ => panic!("unexpected source"),
            }
            .into())
        }
    }
    let cpp = albat::transpile(
        "#include helper.al\n{cout<<helper();}",
        &albat::Options::default(),
        &Files,
    )
    .unwrap();
    assert_eq!(run_cpp(&cpp, ""), "3");
    assert!(cpp.contains("helper.al"));
    let err =
        albat::transpile("#include loop.al\n{}", &albat::Options::default(), &Files).unwrap_err();
    assert!(err.message.contains("cyclic"));
}
#[test]
fn do_while_and_lambda_body() {
    assert_eq!(
        run_cpp(&translate("int i=0; do { i++; } while(i<3); cout<<i;"), ""),
        "3"
    );
    assert_eq!(
        run_cpp(&translate("auto f=[](int x){$$(x);}; f(3);"), ""),
        "3\n"
    );
}
#[test]
fn inserted_libraries_have_forward_declarations() {
    assert_eq!(
        run_cpp(
            &translate("auto p=trim(string(\"xxabcxx\"),'x'); cout<<p.first;"),
            ""
        ),
        "abc"
    );
    assert_eq!(
        run_cpp(
            &translate("vi v={0,0,1,0}; auto p=trim(v,0); cout<<p.first[0];"),
            ""
        ),
        "1"
    );
}
#[test]
fn array_initializers_are_not_lambda_bodies() {
    assert_eq!(run_cpp(&translate("int a[2]={1,2}; cout<<a[1];"), ""), "2");
}
#[test]
fn namespace_and_template_struct() {
    let source="namespace n {int twice(int x){return 2*x;}} template<class T> struct Box { T x; }; {Box<int> b{3}; cout<<n::twice(b.x);}";
    assert_eq!(run_cpp(&translate(source), ""), "6");
}
#[test]
fn pointer_declarations_are_not_power_expressions() {
    let source = "struct Box {int x;}; {Box b{3}; Box *p=&b; Box **q=&p; cout<<(**q).x;}";
    assert_eq!(run_cpp(&translate(source), ""), "3");
}
#[test]
fn library_names_used_as_variables_do_not_request_functions() {
    let cpp = translate("int trim=1; cout<<trim;");
    assert!(!cpp.contains("ltrim("));
    assert_eq!(run_cpp(&cpp, ""), "1");
}
#[test]
fn template_class_methods_are_translated() {
    let source =
        "template<class T> struct Box { T x; void show(){ $$(x); } }; { Box<int> b{3}; b.show(); }";
    assert_eq!(run_cpp(&translate(source), ""), "3\n");
}
#[test]
fn diagnostics_retain_the_configured_source_name() {
    let options = albat::Options {
        source_name: "broken.al".into(),
        ..Default::default()
    };
    let error = albat::transpile("\nif(1 {", &options, &albat::NoSources).unwrap_err();
    assert_eq!(error.source, "broken.al");
    assert_eq!(error.line, 2);
}
#[test]
fn grouped_container_and_user_type_declarations() {
    assert_eq!(
        run_cpp(&translate("vi(a={1,2},b={3,4}); cout<<a[0]<<b[1];"), ""),
        "14"
    );
    assert_eq!(
        run_cpp(
            &translate("struct Box {int x;}; {Box(a={3},b={4}); cout<<a.x<<b.x;}"),
            ""
        ),
        "34"
    );
}
#[test]
fn included_names_do_not_collide_with_generated_names() {
    struct Files;
    impl albat::SourceLoader for Files {
        fn load(&self, _: &str, _: &str) -> Result<String, albat::Diagnostic> {
            Ok("int albat_tmp_0=9,a=1,b=2; (a,b)=(b,a); cout<<albat_tmp_0<<a<<b;".into())
        }
    }
    let cpp = albat::transpile("#include values.al\n", &albat::Options::default(), &Files).unwrap();
    assert_eq!(run_cpp(&cpp, ""), "921");
}
#[test]
fn do_while_conditions_are_translated() {
    assert_eq!(
        run_cpp(&translate("{ do { $$1; } while(2**1==0); }"), ""),
        "1\n"
    );
}
#[test]
fn macro_prefix_in_user_identifiers_does_not_panic() {
    assert_eq!(
        run_cpp(
            &translate("{ int do_perm_count=0; do_perm_count++; $$do_perm_count; }"),
            ""
        ),
        "1\n"
    );
}
#[test]
fn unmatched_closing_delimiters_are_diagnostics() {
    for source in ["int a=1);", "int a=1];"] {
        let error =
            albat::transpile(source, &albat::Options::default(), &albat::NoSources).unwrap_err();
        assert_eq!(error.line, 1);
        assert_eq!(error.column, 8);
    }
}
#[test]
fn main_like_text_in_literals_does_not_change_program_structure() {
    assert_eq!(
        run_cpp(&translate("if(string(\"main(\").size()==5) cout<<1;"), ""),
        "1"
    );
}

#[test]
fn review_else_if_chains_preserve_all_branches() {
    let source = "{rep(a,3){if(a==0) cout<<1; else if(a==1) cout<<2; else cout<<3;} if(false) {} else if(false) {} else if(true) {cout<<4;} else {cout<<5;}}";
    assert_eq!(run_cpp(&translate(source), ""), "1234");
}

#[test]
fn review_construction_statements_are_not_grouped_declarations() {
    assert_eq!(
        run_cpp(&translate("struct Box { int x; }; { Box(3); }"), ""),
        ""
    );
    let cpp = translate("struct Box {int x;}; {Box(a={3},b={4}); cout<<a.x<<b.x;}");
    assert_eq!(run_cpp(&cpp, ""), "34");
}

#[test]
fn review_enum_members_are_not_statements() {
    assert_eq!(
        run_cpp(&translate("enum E { A=3, B=4 }; {cout<<A<<B;}"), ""),
        "34"
    );
    assert_eq!(
        run_cpp(
            &translate("enum class E : unsigned { A=3, B=A+1, }; {cout<<int(E::B);}"),
            ""
        ),
        "4"
    );
}

#[test]
fn review_headers_with_explicit_main_remain_at_file_scope() {
    let dir = tempfile::tempdir().unwrap();
    let header = dir.path().join("header.hpp");
    std::fs::write(
        &header,
        "namespace test_header { inline int value(){return 7;} }\n",
    )
    .unwrap();
    let source = format!(
        "#include \"{}\"\n{{cout<<test_header::value();}}",
        header.display()
    );
    assert_eq!(run_cpp(&translate(&source), ""), "7");
}

#[test]
fn review_parenthesized_construction_expressions_are_preserved() {
    let source = "struct Box {int x;}; {int x=3; (Box(x)); vector<int> v={1,2}; (vector<int>(v)); cout<<x<<v.size();}";
    assert_eq!(run_cpp(&translate(source), ""), "32");
}
