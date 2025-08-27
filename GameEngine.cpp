#include "GameEngine.hpp"
#include "GameObject.hpp"
#include <SDL2/SDL_image.h>
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

GameEngine::GameEngine() {}

GameEngine::~GameEngine() {
    cleanup();
}

bool GameEngine::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Lua-SDL2 Game",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              800, 600,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    lua.open_libraries(sol::lib::base);
    setup_lua_bindings();

    return true;
}

void GameEngine::setup_lua_bindings() {
    lua.set_function("MakeObject", [&](const std::string& image_path) -> GameObject* {
        auto obj = std::make_unique<GameObject>(image_path, renderer, script_dir);
        GameObject* ptr = obj.get();
        game_objects.push_back(std::move(obj));
        return ptr;
    });

    lua.new_usertype<GameObject>("GameObject",
        "show", &GameObject::show,
        "hide", &GameObject::hide,
        "is_visible", &GameObject::is_visible,
        "location", &GameObject::location,
        "scale", &GameObject::scale
    );
}

void GameEngine::run(const std::string& lua_script_path) {
    fs::path script_path(lua_script_path);
    script_dir = script_path.parent_path().string();
    std::cout << "Script directory: " << script_dir << std::endl;

    try {
        lua.script_file(lua_script_path);
        main_loop();
    } catch (const sol::error& e) {
        std::cerr << "Lua Error: " << e.what() << std::endl;
    }
}

void GameEngine::main_loop() {
    bool running = true;
    SDL_Event event;
    int window_width = 800;
    int window_height = 600;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        window_width = event.window.data1;
                        window_height = event.window.data2;
                        std::cout << "Window resized to: " << window_width << "x" << window_height << std::endl;
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto& obj : game_objects) {
            obj->render(window_width, window_height);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void GameEngine::cleanup() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}
