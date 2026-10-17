mod support;
use support::{run_cpp, translate};
#[test]
fn input_and_output() {
    assert_eq!(run_cpp(&translate("int@a,b; $$(a+b);"), "3 4\n"), "7\n");
}
#[test]
fn tuple_assignment_is_simultaneous() {
    assert_eq!(
        run_cpp(&translate("int a=1,b=2; (a,b)=(b,a); cout<<a<<b;"), ""),
        "21"
    );
}
#[test]
fn legacy_input_increment_remains_a_decrement() {
    assert_eq!(run_cpp(&translate("int@a++; $$(a);"), "5\n"), "4\n");
}
#[test]
fn operators_and_compound_assignment() {
    let code = "int a=2,c=-2; a **= 3; cout << a << ' ' << (5 /+ 3) << ' ' << (c %+ 5);";
    assert_eq!(run_cpp(&translate(code), ""), "8 2 3");
}
#[test]
fn tuple_names_do_not_collide_and_literals_are_preserved() {
    assert_eq!(
        run_cpp(
            &translate(
                "int albat_tmp_0=9,a=1,b=2; (a,b)=(b,a); cout<<albat_tmp_0<<a<<b<<\"あ@ **\";"
            ),
            ""
        ),
        "921あ@ **"
    );
}
#[test]
fn sample_io() {
    assert_eq!(
        run_cpp(&translate(include_str!("../sample/io.al")), "3 4 5\n"),
        "12\n60\n"
    );
}
#[test]
fn sample_op() {
    assert_eq!(
        run_cpp(&translate(include_str!("../sample/op.al")), "5 3\n"),
        "a ** b = 125\n⌊ a / b ⌋ = 2\nc % a = 3\n"
    );
}
#[test]
fn sample_rep() {
    assert_eq!(
        run_cpp(&translate(include_str!("../sample/rep.al")), "10\n"),
        "1\n3\n6\n10\n15\n21\n28\n36\n45\n55\n"
    );
}
#[test]
fn sample_one() {
    assert_eq!(
        run_cpp(
            &translate(include_str!("../sample/sample1.al")),
            "3 5 1 2 3 1 2 4\n"
        ),
        "Yes\n"
    );
}
#[test]
fn sample_two() {
    assert_eq!(
        run_cpp(
            &translate(include_str!("../sample/sample2.al")),
            "3 3 11 1 3 9 1 2 4 2 4 0 1 2 2 1 3 1 2\n"
        ),
        "Yes\nNo\nYes\n"
    );
}
#[test]
fn sample_tuple() {
    assert_eq!(
        run_cpp(&translate(include_str!("../sample/tuple.al")), ""),
        "a : 1, b : 2, c : 3\na : 2, b : 3, c : 1\n"
    );
}
#[test]
fn loops_and_permutations() {
    let source = "int s=0; rep(i,4) s+=i; rrep(j,1,5,2) s+=j; cout<<s;";
    assert_eq!(run_cpp(&translate(source), ""), "12");
    let source = "vi v={1,2,3}; int n=0; do_perm(all(v)){n++;} cout<<n;";
    assert_eq!(run_cpp(&translate(source), ""), "6");
}
#[test]
fn slices_and_collection_macros() {
    let source = "vi v={1,2,3,4}; auto s=v[1:3]; cout<<len(s)<<':'<<Count(v,2);";
    assert_eq!(run_cpp(&translate(source), ""), "2:1");
}
#[test]
fn grouped_declarations_and_input_options() {
    assert_eq!(run_cpp(&translate("int(a=1,b=2); cout<<a+b;"), ""), "3");
    assert_eq!(
        run_cpp(&translate("vi v(3); @v[size=3]; $$(v);"), "1 2 3\n"),
        "1 2 3\n"
    );
}
