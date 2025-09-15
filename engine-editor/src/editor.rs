use leptos::ev::event_target_value;
use leptos::prelude::*;
use leptos_router::use_query_map;
use serde::{Deserialize, Serialize};
use wasm_bindgen::prelude::*;
use wasm_bindgen::JsValue;
use std::path::Path;

#[wasm_bindgen]
extern "C" {
    #[wasm_bindgen(js_namespace = ["window", "__TAURI__", "core"])]
    async fn invoke(cmd: &str, args: JsValue) -> JsValue;
}

#[derive(Serialize)]
struct ReadDirArgs<'a> {
    path: &'a str,
}

#[derive(Serialize)]
struct ReadFileContentArgs<'a> {
    path: &'a str,
}

#[derive(Serialize)]
struct WriteFileContentArgs<'a> {
    path: &'a str,
    content: &'a str,
}

#[derive(Deserialize, Clone, Debug, PartialEq)]
struct DirEntry {
    path: String,
    is_dir: bool,
    name: String,
}

#[component]
pub fn Editor() -> impl IntoView {
    let query = use_query_map();
    let path = move || {
        query.with(|q| {
            q.get("path")
                .and_then(|p| js_sys::decode_uri_component(p).ok())
                .unwrap_or_default()
        })
    };

    let (files, set_files) = create_signal(Vec::<DirEntry>::new());
    let (selected_file, set_selected_file) = create_signal(None::<String>);
    let (file_content, set_file_content) = create_signal(String::new());

    create_effect(move |_| {
        let p = path();
        if !p.is_empty() {
            spawn_local(async move {
                let args = serde_wasm_bindgen::to_value(&ReadDirArgs { path: &p }).unwrap();
                let result = invoke("read_dir", args).await;
                match serde_wasm_bindgen::from_value(result) {
                    Ok(files_val) => set_files.set(files_val),
                    Err(e) => web_sys::console::error_1(&e.into()),
                }
            });
        }
    });

    let on_file_click = move |file: DirEntry| {
        if !file.is_dir {
            spawn_local(async move {
                let args =
                    serde_wasm_bindgen::to_value(&ReadFileContentArgs { path: &file.path }).unwrap();
                let result = invoke("read_file_content", args).await;
                if let Some(content) = result.as_string() {
                    set_selected_file.set(Some(file.path));
                    set_file_content.set(content);
                } else {
                    web_sys::console::error_1(&"Failed to read file content".into());
                }
            });
        }
    };

    let on_save = move |ev: web_sys::KeyboardEvent| {
        if ev.key() == "s" && ev.ctrl_key() {
            ev.prevent_default();
            if let Some(path) = selected_file.get_untracked() {
                let content = file_content.get_untracked();
                spawn_local(async move {
                    let args = serde_wasm_bindgen::to_value(&WriteFileContentArgs {
                        path: &path,
                        content: &content,
                    })
                    .unwrap();
                    invoke("write_file_content", args).await;
                });
            }
        }
    };

    view! {
        <div class="editor-container">
            <div class="file-tree">
                <h3>{ path }</h3>
                <ul>
                    {move || {
                        files.get().into_iter().map(|f| {
                            let f_clone = f.clone();
                            view! {
                                <li on:click=move |_| on_file_click(f_clone.clone())>{f.name}</li>
                            }
                        }).collect::<Vec<_>>()
                    }}
                </ul>
            </div>
            <div class="editor">
                <textarea
                    on:keydown=on_save
                    on:input=move |ev| set_file_content.set(event_target_value(&ev))
                    prop:value=move || file_content.get()
                ></textarea>
            </div>
        </div>
    }
}
