use std::fs;
use serde::{Deserialize, Serialize};
use tauri_plugin_dialog::Dialog;

#[derive(Serialize, Deserialize, Debug, Clone)]
struct DirEntry {
    path: String,
    is_dir: bool,
    name: String,
}

// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

#[tauri::command]
async fn open_project(dialog: Dialog) -> Option<String> {
    if let Some(path) = dialog.file().pick_dir().await {
        return Some(path.to_string_lossy().to_string());
    }
    None
}

#[tauri::command]
fn read_dir(path: &str) -> Result<Vec<DirEntry>, String> {
    let mut entries = vec![];
    for entry in fs::read_dir(path).map_err(|e| e.to_string())? {
        let entry = entry.map_err(|e| e.to_string())?;
        let path = entry.path();
        let name = path.file_name().unwrap().to_string_lossy().to_string();
        let is_dir = path.is_dir();
        entries.push(DirEntry {
            path: path.to_string_lossy().to_string(),
            is_dir,
            name,
        });
    }
    Ok(entries)
}

#[tauri::command]
fn read_file_content(path: &str) -> Result<String, String> {
    fs::read_to_string(path).map_err(|e| e.to_string())
}

#[tauri::command]
fn write_file_content(path: &str, content: &str) -> Result<(), String> {
    fs::write(path, content).map_err(|e| e.to_string())
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_opener::init())
        .plugin(tauri_plugin_dialog::init())
        .plugin(tauri_plugin_fs::init())
        .invoke_handler(tauri::generate_handler![
            greet,
            open_project,
            read_dir,
            read_file_content,
            write_file_content
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
