CARGO ?= cargo
WASM_BINDGEN ?= wasm-bindgen

all: native

native:
	$(CARGO) build --release --locked
	mkdir -p bin
	cp target/release/albat bin/albat

test:
	$(CARGO) test --locked
	python3 tools/test_compat.py

reference:
	$(MAKE) -C compat/cpp

test-reference: native reference
	python3 tools/check_catalog.py
	python3 tools/compat.py --reference build/reference/albat --candidate bin/albat

wasm:
	$(CARGO) build --lib --release --locked --target wasm32-unknown-unknown --features wasm-bindgen
	$(WASM_BINDGEN) --target web --out-dir bin --out-name albat target/wasm32-unknown-unknown/release/albat.wasm

test-web: wasm
	$(CARGO) build --locked
	node tools/test_web.mjs

clean:
	rm -rf build bin target

.PHONY: all native test reference test-reference wasm test-web clean
