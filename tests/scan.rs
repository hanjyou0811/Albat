use albat::scan::{matching_close, split_top_level};

#[test]
fn splits_only_unquoted_top_level_delimiters() {
    assert_eq!(
        split_top_level("f(1,2),\"あ,b\",v[0]", ',').unwrap(),
        vec!["f(1,2)", "\"あ,b\"", "v[0]"]
    );
    assert_eq!(
        split_top_level("a,/* , ] */b", ',').unwrap(),
        vec!["a", "/* , ] */b"]
    );
}
#[test]
fn matches_brackets_without_reading_literals_or_comments() {
    let s = "(\"あ)\", /* ] */ f('\\\''))";
    assert_eq!(matching_close(s, 0).unwrap(), s.len() - 1);
    assert_eq!(matching_close("あ(x)", 3).unwrap(), 5);
}
#[test]
fn rejects_unbalanced_input_with_position() {
    let err = matching_close("(abc", 0).unwrap_err();
    assert_eq!(err.line, 1);
    assert_eq!(err.column, 1);
    assert!(split_top_level("a,(b]", ',').is_err());
    assert!(split_top_level("a,\"open", ',').is_err());
    assert!(matching_close("あ(x)", 1).is_err());
    assert!(matching_close("", 0).is_err());
}
#[test]
fn empty_and_escaped_inputs() {
    assert_eq!(split_top_level("", ',').unwrap(), vec![""]);
    assert_eq!(
        split_top_level("\"a\\\",b\",c", ',').unwrap(),
        vec!["\"a\\\",b\"", "c"]
    );
    assert_eq!(
        split_top_level("a,// ,\nb", ',').unwrap(),
        vec!["a", "// ,\nb"]
    );
}
