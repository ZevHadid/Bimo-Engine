# Bimo-Engine

A minimal Lua-scriptable game engine built with C++, SDL2, and Sol2. Write your game logic in Lua while the engine handles the heavy lifting.

## Features

- **Lua Scripting**: Control game objects and logic with simple Lua scripts
- **Simple API**: Easy-to-use `MakeObject()`, `location()`, and `scale()` functions
- **Resolution Independent**: Objects maintain position and scale across window sizes
- **No Image Warping**: Aspect ratios are preserved automatically
- **Cross-Platform**: Built on SDL2 for multi-platform support

## Quick Start

### Lua API

```lua
-- Create an object from image
obj = MakeObject("image.png")

-- Position (0-100% from top-left, center of image at location)
obj:location(50, 50)  -- Center of screen
obj:location(10, 10)  -- Top-left corner
obj:location(90, 90)  -- Bottom-right corner

-- Scale (0-100% of window size, maintains aspect ratio)
obj:scale(20)  -- 20% of window size

-- Visibility control
obj:show()
obj:hide()
visible = obj:is_visible()
```

### Example Game

```lua
-- script.lua
character = MakeObject("player.png")
character:location(50, 70)  -- Center horizontally, 70% down
character:scale(15)         -- 15% of window size
character:show()

enemy = MakeObject("enemy.png")
enemy:location(20, 30)
enemy:scale(10)
enemy:show()
```

## Building

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install libsdl2-dev libsdl2-image-dev liblua5.4-dev

# Compile and run
g++ -std=c++17 main.cpp GameObject.cpp GameEngine.cpp -I./sol2/include $(pkg-config --cflags --libs lua5.4) -lSDL2 -lSDL2_image -o game

# Run with your script
./game path/to/your/script.lua
```

## Project Structure

```
Bimo-Engine/
├── main.cpp          # Entry point
├── GameObject.hpp    # GameObject class interface
├── GameObject.cpp    # GameObject implementation
├── GameEngine.hpp    # Engine interface
├── GameEngine.cpp    # Engine implementation
├── sol2/            # Sol2 Lua binding library
└── test_game/
    └── scripts/
        └── script.lua  # Example game script
```

## How It Works

1. **Paths**: Image paths are relative to your Lua script's location
2. **Coordinates**: `location(x, y)` uses percentages (0-100) from top-left
3. **Scaling**: `scale(size)` uses percentage of window size, maintains aspect ratio
4. **Rendering**: Automatically handles window resizing and maintains proportions

## License

MIT License - feel free to use for your projects!

---

**Bimo-Engine**: Write games in Lua, run them anywhere.
