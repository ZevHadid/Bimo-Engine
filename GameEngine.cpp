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

    lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::math, sol::lib::string);
    setup_lua_bindings();

    return true;
}

SDL_Texture* GameEngine::GetCachedTexture(const std::string& path) {
    if (texture_cache.find(path) != texture_cache.end()) {
        return texture_cache[path];
    }

    // Check if file exists
    if (!fs::exists(path)) {
        std::cerr << "ERROR: File does not exist: " << path << std::endl;
        return CreateDefaultTexture();
    }

    // Load the texture
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return CreateDefaultTexture();
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return CreateDefaultTexture();
    }

    std::cout << "Successfully loaded: " << path << std::endl;
    texture_cache[path] = texture;
    return texture;
}

SDL_Texture* GameEngine::CreateDefaultTexture() {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
    if (!surface) return nullptr;

    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 255, 0, 0));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void GameEngine::ClearTextureCache() {
    for (auto& pair : texture_cache) {
        if (pair.second) {
            SDL_DestroyTexture(pair.second);
        }
    }
    texture_cache.clear();
}

void GameEngine::setup_lua_bindings() {
    lua.set_function("game_title", [&](const std::string& title) {
        SDL_SetWindowTitle(window, title.c_str());
    });

    lua.set_function("set_debug", [&](bool debug) {
        show_debug = debug;
    });

    lua.set_function("MakeObject", [&](const std::string& image_path) -> GameObject* {
        fs::path full_path = fs::path(script_dir) / image_path;

        // Use cached texture
        SDL_Texture* texture = GetCachedTexture(full_path.string());

        // Create GameObject with pre-loaded texture
        auto obj = std::make_unique<GameObject>(texture, renderer);
        GameObject* ptr = obj.get();
        game_objects.push_back(std::move(obj));
        return ptr;
    });

    // Replace the is_key_pressed function with this simpler version:
    lua.set_function("is_key_pressed", [&](const std::string& key) {
        const Uint8* state = SDL_GetKeyboardState(NULL);

        static const std::unordered_map<std::string, SDL_Scancode> key_map = {
            {"up", SDL_SCANCODE_UP}, {"down", SDL_SCANCODE_DOWN},
            {"left", SDL_SCANCODE_LEFT}, {"right", SDL_SCANCODE_RIGHT},
            {"w", SDL_SCANCODE_W}, {"a", SDL_SCANCODE_A},
            {"s", SDL_SCANCODE_S}, {"d", SDL_SCANCODE_D},
            {"space", SDL_SCANCODE_SPACE}, {"enter", SDL_SCANCODE_RETURN},
            {"escape", SDL_SCANCODE_ESCAPE}
        };

        std::string lower_key = key;
        std::transform(lower_key.begin(), lower_key.end(), lower_key.begin(), ::tolower);

        auto it = key_map.find(lower_key);
        if (it != key_map.end()) return state[it->second] != 0;

        SDL_Scancode scancode = SDL_GetScancodeFromName(key.c_str());
        return scancode != SDL_SCANCODE_UNKNOWN && state[scancode] != 0;
    });


    lua.set_function("set_update_function", [&](sol::function func) {
        lua_update_function = func;
    });

    lua.new_enum<ScaleMode>("ScaleMode", {
        {"MaintainAspectRatio", ScaleMode::MaintainAspectRatio},
        {"Stretch", ScaleMode::Stretch},
        {"Crop", ScaleMode::Crop},
        {"None", ScaleMode::None}
    });

    lua.new_usertype<GameObject>("GameObject",
        "show", &GameObject::show,
        "hide", &GameObject::hide,
        "is_visible", &GameObject::is_visible,
        "location", &GameObject::location,
        "scale", &GameObject::scale,
        "set_scale_mode", &GameObject::set_scale_mode,
        "set_depth", &GameObject::set_depth,
        "get_depth", &GameObject::get_depth,
        "delay", &GameObject::delay,
        "is_delayed", &GameObject::is_delayed
    );
}

void GameEngine::handle_events(bool& running, int& window_width, int& window_height) {
    SDL_Event event;
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
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_F3) {
                    show_debug = !show_debug;
                    std::cout << "Debug mode: " << (show_debug ? "ON" : "OFF") << std::endl;
                }
                break;
        }
    }
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
    int window_width = 800;
    int window_height = 600;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    float deltaTime = 0;

    while (running) {
        SDL_PumpEvents(); // ensure keyboard state is updated

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (float)((NOW - LAST) * 1000 / (float)SDL_GetPerformanceFrequency()) * 0.001f;

        handle_events(running, window_width, window_height);

        // Update all game objects
        for (auto& obj : game_objects) {
            obj->update(deltaTime);
        }

        // Call Lua update function if set
        if (lua_update_function.valid()) {
            try {
                lua_update_function(deltaTime);
            } catch (const sol::error& e) {
                std::cerr << "Lua Update Error: " << e.what() << std::endl;
            }
        }

        // Sort by depth for proper rendering order
        std::sort(game_objects.begin(), game_objects.end(),
            [](const auto& a, const auto& b) { return a->get_depth() < b->get_depth(); });

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto& obj : game_objects) {
            obj->render(window_width, window_height, show_debug);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void GameEngine::cleanup() {
    // Clear game objects first
    game_objects.clear();

    // Clear texture cache
    ClearTextureCache();

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
