# Albat

競技プログラミング向けの簡潔な記法をC++へ変換するツールです。変換器本体はRustで実装され、CLIとブラウザ版が同じ変換処理を使います。

```cpp
int@a,b;
$$(a+b);
```

入力 `3 4` に対して `7` を出力するC++プログラムへ変換します。`@` は入力、`$` は出力、`$$` は改行付き出力です。`rep` / `fore`、`vi` / `ll`、`**` / `/+` / `%+`、タプル代入、スライスなどの既存記法も扱います。

## ビルドと実行

Rustのstableツールチェーン（Cargoを含む）が必要です。通常の変換器ビルドにはC++コンパイラは不要です。生成C++のコンパイル、`run` / `exec`、実行テストには `g++`（C++20とGNU標準ヘッダーを利用）が必要です。

```sh
cargo build --release --locked
./target/release/albat < sample/sample1.al > /tmp/albat-main.cpp
g++ -std=gnu++20 /tmp/albat-main.cpp -o /tmp/albat-main
printf '3 5 1 2 3 1 2 4\n' | /tmp/albat-main
```

従来の `make` も利用できます。Rust版をビルドして `bin/albat` に配置します。

```sh
make
printf '3 4 5\n' | ./bin/albat run sample/io.al
```

`run` は変換・コンパイル・実行を一時ディレクトリ内で行い、標準入出力を引き継ぎます。コンパイル失敗や実行プログラムの終了コードは呼び出し元へ返します。

```sh
# 入出力をファイルへ接続
./bin/albat exec sample/io.al /tmp/input.txt /tmp/output.txt

# 暗黙mainの代わりに指定した関数を生成
printf 'return a + b;\n' | ./bin/albat --export '["int","solve","int a, int b"]'
```

`#include helper.al` は従来どおり作業ディレクトリを基準に読み込みます。通常のC++ヘッダー指定（`#include <...>` / `#include "..."`）は生成C++へ渡します。外部ライブラリのヘッダーはC++コンパイラ側の検索パスへ設定してください。

## ブラウザ版

RustのWASMターゲットと、Cargo.lockに対応するwasm-bindgen CLIを一度インストールします。

```sh
rustup target add wasm32-unknown-unknown
cargo install wasm-bindgen-cli --version 0.2.129 --locked
make wasm
python3 -m http.server 8080 --bind 127.0.0.1
```

`http://127.0.0.1:8080/albat.html` を開きます。入力とサンプル選択からC++を生成できます。ブラウザ内ではC++のコンパイル・実行やローカルファイルの取り込みは行いません。生成したWASM/JSは `bin/` に配置されます。

## 検証

Python 3、g++、GNU `timeout` を使用します。テスト生成物は一時ディレクトリに作り、`sample/` のファイルを上書きしません。

```sh
cargo fmt --check
cargo clippy --all-targets --all-features -- -D warnings
make test
./run_sample.sh
```

ブラウザの実動作テスト:

```sh
npm ci
npx playwright install chromium
make test-web
```

旧C++版との比較とライブラリ全件照合:

```sh
make test-reference
```

旧変換器は `compat/cpp/` に比較専用として残しています。Rust版の通常ビルド、CLI、WASMはこれをリンク・起動しません。`assets/cpp/` は生成プログラムに埋め込むライブラリ本文であり、変換器の実装ではありません。

## 互換性と既知の制限

[互換性記録](docs/compatibility.md)にケースと既知の問題、[移植の検証記録](docs/rust-port-validation.md)に要件ごとの確認結果を記載しています。元ライブラリに含まれる不具合（入力の `++` が減算になる、min/maxセグメント木のコンストラクタ名、剰余型の加算など）は移植に紛れて変更していません。これらを使う場合は記録を確認してください。

旧変換器が不正なC++を生成するケースと、正常な実行結果比較は区別して検証します。不正なAlbat入力では位置付きのエラーを返します。`exec` の欠落スクリプトへの依存と成功扱いになる終了コードは解消しました。
