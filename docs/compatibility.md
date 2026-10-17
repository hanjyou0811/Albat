# Rust移植の互換性記録

参照: 6ab364e。実装・検証は進行中。未検証の行は移植済みを意味しない。

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

## 追加検証対象

| 機能 | 検証担当 | 状態 |
| --- | --- | --- |
| リテラル/コメント/UTF-8 | scan / transforms | 未検証 |
| rep/REP/rrep/RREP/fore/do_perm全引数形式 | transforms | 未検証 |
| all/len/Count/_upper_bound/_lower_bound | transforms | 未検証 |
| template/inline/class/struct/namespace/lambda | syntax | 未検証 |
| define/include/ifdef/endif/undef/pragma | syntax | 未検証 |
| 入力size/前後増減/!、出力sep | transforms | 未検証 |
| 独自複合代入/優先順位/スライス | transforms | 未検証 |
| run/export/exec/#line/終了コード | cli | 未検証 |
| WASM/連続変換 | web | 未検証 |

## 旧版から分離して扱う問題

- int@a++ の誤変換は特性ケースで保持する。
- inversionNumber検出とinversionnumber登録に表記差がある。
- //atcoderのライブラリ要求は無効化されている。
- execが呼ぶrun_albat.shは存在しない。Rustのexecは設計で承認した修復対象。
- 未定義動作、異常終了、状態漏れは再現しない。

## ライブラリ登録の全件一覧

本文・登録順序・依存・排他の照合と、発動可能な利用例をTask 3/6で検証する。

| 登録名 | 状態 |
| --- | --- |
| pragma | 未検証 |
| Ofast | 未検証 |
| unroll-loops | 未検証 |
| stdc++ | 未検証 |
| namespace_std | 未検証 |
| ACL | 未検証 |
| namespace_acl | 未検証 |
| DEFINE_INT_LL | 未検証 |
| gcd | 未検証 |
| lcm | 未検証 |
| _pow | 未検証 |
| _div | 未検証 |
| _mod | 未検証 |
| makePrimes | 未検証 |
| divnum | 未検証 |
| popcount | 未検証 |
| popcountl | 未検証 |
| bit_enumerate | 未検証 |
| hasbit | 未検証 |
| bitlen | 未検証 |
| UnionFind | 未検証 |
| WeightedUnionFind | 未検証 |
| _SegTree | 未検証 |
| SegTree_pt_max | 未検証 |
| SegTree_pt_min | 未検証 |
| SegTree_pt_sum | 未検証 |
| Fenwick_tree | 未検証 |
| DEFINE_MOD | 未検証 |
| mint | 未検証 |
| mint_init | 未検証 |
| modint | 未検証 |
| modint_init | 未検証 |
| mod_int | 未検証 |
| mod_int_init | 未検証 |
| MexSet | 未検証 |
| FastIO | 未検証 |
| FastIO_flush | 未検証 |
| istream | 未検証 |
| istream_pair | 未検証 |
| istream_vector | 未検証 |
| ostream | 未検証 |
| ostream_pair | 未検証 |
| ostream_set | 未検証 |
| ostream_map | 未検証 |
| ostream_vector | 未検証 |
| ostream_vector_vector | 未検証 |
| SYNC_WITH_STDIO | 未検証 |
| CIN_TIE | 未検証 |
| chmax | 未検証 |
| chmin | 未検証 |
| biseek | 未検証 |
| runLengthEncoding | 未検証 |
| CycleLib | 未検証 |
| yesno | 未検証 |
| inversionnumber | 未検証 |
| Slice | 未検証 |
| Slice_vec | 未検証 |
| Slice_str | 未検証 |
| trim | 未検証 |
| trim_str | 未検証 |
| trim_vec | 未検証 |
| ltrim | 未検証 |
| ltrim_str | 未検証 |
| ltrim_vec | 未検証 |
| rtrim | 未検証 |
| rtrim_str | 未検証 |
| rtrim_vec | 未検証 |
