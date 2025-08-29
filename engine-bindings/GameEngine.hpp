#pragma once

#include <sol/sol.hpp>
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

class GameObject;

class GameEngine {
public:
    GameEngine();
    ~GameEngine();

    bool initialize();
    void run(const std::string& lua_script_path);
    void cleanup();

    // Texture cache management
    SDL_Texture* GetCachedTexture(const std::string& path);
    void ClearTextureCache();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    sol::state lua;
    std::vector<std::unique_ptr<GameObject>> game_objects;
    std::string script_dir;
    bool show_debug = false;

    // Lua update function
    sol::function lua_update_function;

    // Texture cache
    std::unordered_map<std::string, SDL_Texture*> texture_cache;

    void setup_lua_bindings();
    void main_loop();
    void handle_events(bool& running, int& window_width, int& window_height);

    SDL_Texture* CreateDefaultTexture();
};
