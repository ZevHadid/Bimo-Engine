
import Button from "../components/Button/Button";
import { open } from '@tauri-apps/api/dialog';
import { invoke } from '@tauri-apps/api/tauri';

function HomePage() {

  async function handleOpenProject() {
    console.log("Open project clicked");
    // TODO: Implement open project functionality
  }

  async function handleNewProject() {
    // Let user pick a folder where the project will be created
    const selected = await open({
      directory: true,  // true = pick a directory
      multiple: false,
      title: "Select folder for new project",
    });

    if (!selected) return;

    try {
      const createdPath = await invoke<string>("create_project_dir", {
        baseDir: selected,
        projectName: "MyNewProject", // optional, can make dynamic later
      });
      console.log("✅ Project created at:", createdPath);
    } catch (err) {
      console.error("❌ Failed to create project:", err);
    }
  }

  return (
    <>  
      <Button
        style={{ fontSize: "1.5rem", width: "250px", height: "60px", marginTop: "50px" }}
        onClick={handleNewProject}
      >
        New Project
      </Button>

      <Button
        style={{ fontSize: "1.5rem", width: "250px", height: "60px" }}
        onClick={handleOpenProject}
      >
        Open Project
      </Button>
    </>
  )
}

export default HomePage;
