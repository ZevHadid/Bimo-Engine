#include "GameEngine.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <lua_script.lua>" << std::endl;
        return 1;
    }

    GameEngine engine;
    if (!engine.initialize()) {
        std::cerr << "Failed to initialize game engine!" << std::endl;
        return 1;
    }

    engine.run(argv[1]);
    return 0;
}
