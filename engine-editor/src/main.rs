mod app;
mod editor;
mod home;

use app::*;
use leptos::*;

fn main() {
    #[cfg(target_arch = "wasm32")]
    {
        console_error_panic_hook::set_once();
        mount_to_body(|| {
            view! {
                <App/>
            }
        })
    }
}
