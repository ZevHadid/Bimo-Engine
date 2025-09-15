use leptos::prelude::*;
use leptos::task::spawn_local;
use leptos_router::{use_navigate, NavigateOptions};
use wasm_bindgen::prelude::*;
use wasm_bindgen::JsValue;

#[wasm_bindgen]
extern "C" {
    #[wasm_bindgen(js_namespace = ["window", "__TAURI__", "core"])]
    async fn invoke(cmd: &str, args: JsValue) -> JsValue;
}

#[component]
pub fn Home() -> impl IntoView {
    let navigate = use_navigate();

    let open_project = move |_| {
        spawn_local(async move {
            let result = invoke("open_project", JsValue::NULL).await;
            if !result.is_null() && !result.is_undefined() {
                if let Some(path) = result.as_string() {
                    let encoded_path = js_sys::encode_uri_component(&path);
                    navigate(&format!("/editor?path={}", encoded_path), Default::default());
                }
            }
        });
    };

    view! {
        <div>
            <h1>"Bimo Editor"</h1>
            <button on:click=open_project>"Open Project"</button>
        </div>
    }
}
