#[cfg_attr(mobile, tauri::mobile_entry_point)]

use tauri::Manager;
use std::fs;
use std::path::Path;



#[tauri::command]
fn create_project_dir(base_dir: String, project_name: String) -> Result<String, String> {
    let full_path = Path::new(&base_dir).join(&project_name);

    if full_path.exists() {
        return Err("Directory already exists".into());
    }

    fs::create_dir_all(&full_path).map_err(|e| e.to_string())?;

    Ok(full_path.to_string_lossy().to_string())
}



pub fn run() {
    tauri::Builder::default()
        .setup(|app| {
            if cfg!(debug_assertions) {
                app.handle().plugin(
                    tauri_plugin_log::Builder::default()
                        .level(log::LevelFilter::Info)
                        .build(),
                )?;
            }
            Ok(())
        })
        .invoke_handler(tauri::generate_handler![create_project_dir])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

