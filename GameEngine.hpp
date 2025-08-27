#pragma once

#include <sol/sol.hpp>
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <string>

class GameObject;

class GameEngine {
public:
    GameEngine();
    ~GameEngine();

    bool initialize();
    void run(const std::string& lua_script_path);
    void cleanup();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    sol::state lua;
    std::vector<std::unique_ptr<GameObject>> game_objects;
    std::string script_dir;

    void setup_lua_bindings();
    void main_loop();
};
