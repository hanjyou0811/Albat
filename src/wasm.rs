use wasm_bindgen::prelude::*;

#[wasm_bindgen]
pub fn process_string(input: &str) -> Result<String, JsValue> {
    crate::transpile(input, &crate::Options::default(), &crate::NoSources)
        .map_err(|error| JsValue::from_str(&error.to_string()))
}
