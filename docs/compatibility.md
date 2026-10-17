# Rust移植の互換性記録

参照: 6ab364e。Rust実装に対して、構文テスト、実行ケース、旧版との比較、実ブラウザ検証を実施する。正常動作の比較と既知不具合の特性テストは区別する。

## 実行ケース

| ケース | 分類 | 期待動作 |
| --- | --- | --- |
| io | 正常 | 入力とcout/FastIO混在 |
| op | 正常 | **、/+、%+ |
| rep | 正常 | rep、fore、添字 |
| sample1 | 正常 | vector入力、ネストした単文fore、yesno |
| sample2 | 正常 | solve前方宣言、while入力、複数ケース |
| tuple | 正常 | タプルの同時代入 |
| input_increment_legacy | 既知不具合 | 入力5で4（++を--へ変換） |

## 構文の検証対応

| 機能 | 検証担当 | 状態 |
| --- | --- | --- |
| リテラル/コメント/UTF-8 | tests/scan.rs、tests/transforms.rs | 実行検証 |
| rep/REP/rrep/RREP/fore/do_perm | tests/transforms.rs、loop_forms/uppercase_loops/permutations | 実行検証。do_permは旧生成器エラーを区別 |
| all/len/Count/_upper_bound/_lower_bound | collection_macros/bound、tests/transforms.rs | 実行検証 |
| template/class/struct/namespace/lambda | tests/syntax.rs | 実行検証 |
| プリプロセッサ/include | tests/syntax.rs、ACL手動比較 | include・循環・外部ヘッダー・条件付き指示行を検証 |
| 入力size/増減/!、出力sep | tests/transforms.rs、input_increment_legacy/input_skip/output_separator | 実行検証 |
| 独自複合代入/スライス | tests/transforms.rs、op/compound_ops/operator_precedence/operator_operands/slice | 実行検証 |
| run/export/exec/#line/終了コード | tests/cli.rs、tests/syntax.rs | 実行検証 |
| WASM/連続変換 | tools/test_web.mjs | 実Chromiumでネイティブ出力と比較 |

## 旧版から分離して扱う問題

- int@a++ の誤変換は特性ケースで保持する。
- inversionNumber検出とinversionnumber登録に表記差がある。
- //atcoderのライブラリ要求は無効化されている。
- execが呼ぶrun_albat.shは存在しない。Rustのexecは設計で承認した修復対象。
- 未定義動作、異常終了、状態漏れは再現しない。

## ライブラリ登録の全件一覧

`python3 tools/check_catalog.py` が両実装の実行時カタログを取り出し、全67件の本文・名前・順序・挿入位置・依存を完全一致で照合する。選択・排他はtests/library.rsで検証する。使用例はtests/fixtures/cases.jsonを参照。

| 登録名 | 状態 |
| --- | --- |
| pragma | カタログ完全一致 |
| Ofast | カタログ完全一致 |
| unroll-loops | カタログ完全一致 |
| stdc++ | カタログ完全一致 |
| namespace_std | カタログ完全一致 |
| ACL | カタログ完全一致 |
| namespace_acl | カタログ完全一致 |
| DEFINE_INT_LL | カタログ完全一致 |
| gcd | カタログ完全一致 |
| lcm | カタログ完全一致 |
| _pow | カタログ完全一致 |
| _div | カタログ完全一致 |
| _mod | カタログ完全一致 |
| makePrimes | カタログ完全一致 |
| divnum | カタログ完全一致 |
| popcount | カタログ完全一致 |
| popcountl | カタログ完全一致 |
| bit_enumerate | カタログ完全一致 |
| hasbit | カタログ完全一致 |
| bitlen | カタログ完全一致 |
| UnionFind | カタログ完全一致 |
| WeightedUnionFind | カタログ完全一致 |
| _SegTree | カタログ完全一致 |
| SegTree_pt_max | カタログ完全一致 |
| SegTree_pt_min | カタログ完全一致 |
| SegTree_pt_sum | カタログ完全一致 |
| Fenwick_tree | カタログ完全一致 |
| DEFINE_MOD | カタログ完全一致 |
| mint | カタログ完全一致 |
| mint_init | カタログ完全一致 |
| modint | カタログ完全一致 |
| modint_init | カタログ完全一致 |
| mod_int | カタログ完全一致 |
| mod_int_init | カタログ完全一致 |
| MexSet | カタログ完全一致 |
| FastIO | カタログ完全一致 |
| FastIO_flush | カタログ完全一致 |
| istream | カタログ完全一致 |
| istream_pair | カタログ完全一致 |
| istream_vector | カタログ完全一致 |
| ostream | カタログ完全一致 |
| ostream_pair | カタログ完全一致 |
| ostream_set | カタログ完全一致 |
| ostream_map | カタログ完全一致 |
| ostream_vector | カタログ完全一致 |
| ostream_vector_vector | カタログ完全一致 |
| SYNC_WITH_STDIO | カタログ完全一致 |
| CIN_TIE | カタログ完全一致 |
| chmax | カタログ完全一致 |
| chmin | カタログ完全一致 |
| biseek | カタログ完全一致 |
| runLengthEncoding | カタログ完全一致 |
| CycleLib | カタログ完全一致 |
| yesno | カタログ完全一致 |
| inversionnumber | カタログ完全一致 |
| Slice | カタログ完全一致 |
| Slice_vec | カタログ完全一致 |
| Slice_str | カタログ完全一致 |
| trim | カタログ完全一致 |
| trim_str | カタログ完全一致 |
| trim_vec | カタログ完全一致 |
| ltrim | カタログ完全一致 |
| ltrim_str | カタログ完全一致 |
| ltrim_vec | カタログ完全一致 |
| rtrim | カタログ完全一致 |
| rtrim_str | カタログ完全一致 |
| rtrim_vec | カタログ完全一致 |

## 追加調査で再現した旧実装の問題

- `do_perm(all(v))` は旧変換器が生成するC++の構文エラー。Rustでは変換の意図に沿ったdo/whileを生成する。
- `SegTree_pt_sum` と `MexSet` は旧変換器が型/メンバー宣言を破壊するためコンパイルに失敗。Rustは元の定義を保持する。
- `SegTree_pt_min/max` は元のライブラリ本文に `SegTree_point_min/max` というコンストラクタ名の誤りがある。本文を保持し、両実装が同じエラーを出すことを特性テストで確認する。
- mint/modint/mod_int は旧変換器の不正なメンバー前方宣言によりコンパイル失敗。Rustでは定義を保持し、明示的なint変換で利用を確認する。元本文の `operator+=` は減算を行うため、加算結果の修正は今回の移植に含めない。
- `trim` 系のライブラリは依存関数の前方宣言が必要。Rust初期版の欠落を回帰テストで修正した。

上記の旧版エラーを期待するケースは、正常な実行結果比較と分類を分ける。タイムアウトや任意の失敗を合格扱いにせず、コンパイラエラーの対象名を照合する。

## 再現コマンドと補足

- `make test`: Rustの単体・結合テストと58ケースのコンパイル/実行/既知コンパイルエラー検証。
- `make test-reference`: 全カタログ照合と同じ58ケースを旧変換器にも適用。現在の分類は {'normal': 49, 'known-bug': 1, 'legacy-generator-failure': 6, 'known-library-compile-error': 2}。ケース追加時はJSONを正とする。
- `make test-web`: 実ブラウザで全ケースの生成C++をネイティブ版と完全一致比較し、不正入力後の復帰と状態の分離を検証する。C++をブラウザ内で実行するテストではない。
- `python3 tools/test_build.py`: Makeビルド、CLIヘルプ、6サンプル、sampleディレクトリの内容・パーミッション不変を検証。
- ACLは公式ac-libraryを用意し、`CPLUS_INCLUDE_PATH` を指定して `#include <atcoder/all>` とdsuの利用を両版で検証した。外部ヘッダーは同梱しない。
- C++コンパイラを失敗する代替プログラムに置き換えたクリーンCargoビルドと変換の成功を確認。旧版は通常のRustビルド・実行には不要。

生成C++の整形、一時変数名、元ソースを末尾へ付けるコメントは互換性の対象外。元ソースに `*/` が含まれると壊れる旧CLIのコメント付加は廃止した。`exec`、不正入力の診断、変換ごとの状態分離は設計で承認された差分。

## 最終レビューで追加した回帰検証

- `do_perm_count` のような利用者の識別子をマクロと誤認しない。
- `do ... while` の条件にも独自演算子とライブラリ検出を適用する。
- `$$a,b;` は旧版どおりaだけを出力し、bを通常の式文として扱う。両方を出力する記法は `$$(a,b);`。
- 不正な閉じ括弧 `)` / `]` は位置付きの診断を返す。
- `vi(...)` やユーザー型のまとめた宣言を変換する。
- 取り込んだソース中の名前も一時変数名の衝突回避に含める。
- `_lower_bound(all(s), x)` はall展開後の引数に従って変換する。
- 文字列中の `main(` をプログラムのmain定義と誤認しない。

旧版ですでに壊れているraw string内の引用符、typedef二重ポインター、複雑な左辺へのべき乗、スライス端点の省略は、新しい構文として追加していない。dangling elseの既存の結び付きも維持する。負の指数で旧変換器が異常終了する動作は再現対象から除外する。
