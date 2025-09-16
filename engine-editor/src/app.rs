use crate::editor::Editor;
use crate::home::Home;
use leptos::*;
use leptos_router::*;

#[component]
pub fn App() -> impl IntoView {
    view! {
        <Router>
            <main>
                <Routes>
                    <Route path="/editor" view=Editor/>
                    <Route path="/" view=Home/>
                </Routes>
            </main>
        </Router>
    }
}
