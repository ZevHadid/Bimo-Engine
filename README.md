# Bimo-Engine

A minimal Lua-scriptable game engine built with C++, SDL2, and Sol2. Write your game logic in Lua while the engine handles the heavy lifting.

## Features

- **Lua Scripting**: Control game objects and logic with simple Lua scripts
- **Simple API**: Easy-to-use `MakeObject()`, `location()`, `scale()`, and `delay()` functions
- **Resolution Independent**: Objects maintain position and scale across window sizes
- **No Image Warping**: Aspect ratios are preserved automatically
- **Cross-Platform**: Built on SDL2 for multi-platform support
- **Texture Caching**: Automatic texture management and reuse
- **Layered Rendering**: Depth-based rendering order with `set_depth()`
- **Collision Detection**: Basic AABB collision detection between objects
- **Input Handling**: Keyboard state checking with `is_key_pressed()`
- **Debug Mode**: Visual debugging with bounding boxes (F3 to toggle)

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

-- Scale modes
obj:set_scale_mode(ScaleMode.MaintainAspectRatio)  -- Default
obj:set_scale_mode(ScaleMode.Stretch)              -- Fill available space
obj:set_scale_mode(ScaleMode.Crop)                 -- Crop to fit
obj:set_scale_mode(ScaleMode.None)                 -- No scaling

-- Visibility control
obj:show()
obj:hide()
visible = obj:is_visible()

-- Depth/layering
obj:set_depth(2)  -- Higher depth = rendered on top
depth = obj:get_depth()

-- Delay functionality (blocks method calls)
obj:delay(2000)  -- Delay for 2 seconds (2000ms)
delayed = obj:is_delayed()

-- Collision detection
if obj1:check_collision(obj2) then
    print("Collision detected!")
end

bounds = obj:get_bounds()  -- Get SDL_Rect of object bounds
```

### Input Handling
```lua
-- Check keyboard state
if is_key_pressed("Space") then
    print("Space bar is pressed!")
end

if is_key_pressed("A") or is_key_pressed("Left") then
    -- Move left
end
```

### Example Game

```lua
-- script.lua
game_title("My Awesome Game")

-- Create objects
player = MakeObject("player.png")
player:location(50, 70)
player:scale(15)
player:set_depth(2)
player:show()

enemy = MakeObject("enemy.png")
enemy:location(20, 30)
enemy:scale(10)
enemy:set_depth(1)
enemy:show()

background = MakeObject("bg.png")
background:location(50, 50)
background:scale(100)
background:set_depth(0)
background:show()

-- Game state
local score = 0
local game_time = 0

-- Update function (called every frame)
function update(delta_time)
    game_time = game_time + delta_time

    -- Move enemy every 2 seconds
    if game_time % 2.0 < delta_time then
        enemy:location(math.random(10, 90), math.random(10, 90))
    end

    -- Check collisions
    if player:check_collision(enemy) then
        score = score + 1
        print("Score: " .. score)
        enemy:delay(1000)  -- Freeze enemy for 1 second
    end

    -- Input handling
    if is_key_pressed("Right") then
        local x, y = player:get_location()
        player:location(math.min(x + 1, 95), y)
    end

    if is_key_pressed("Left") then
        local x, y = player:get_location()
        player:location(math.max(x - 1, 5), y)
    end
end

set_update_function(update)
set_debug(true)  -- Enable debug bounding boxes
```

## Building

### Dependencies
- C++17 compiler
- SDL2
- SDL2_image
- Lua 5.4+
- Sol2 (included)

### Linux (Ubuntu/Debian)
```bash
# Install dependencies
sudo apt-get install libsdl2-dev libsdl2-image-dev liblua5.4-dev

# Compile
g++ -std=c++17 main.cpp GameObject.cpp GameEngine.cpp \
    -I./sol2/include \
    $(pkg-config --cflags --libs lua5.4) \
    -lSDL2 -lSDL2_image \
    -o bimo-engine

# Run with your script
./bimo-engine path/to/your/script.lua
```

### Windows (MinGW)
```bash
# Compile with MinGW
g++ -std=c++17 main.cpp GameObject.cpp GameEngine.cpp \
    -I./sol2/include \
    -I/path/to/lua/include \
    -L/path/to/lua/libs \
    -lSDL2 -lSDL2_image -llua54 \
    -o bimo-engine.exe
```

## Project Structure

```
Bimo-Engine/
├── main.cpp              # Entry point
├── GameObject.hpp        # GameObject class interface
├── GameObject.cpp        # GameObject implementation
├── GameEngine.hpp        # Engine interface
├── GameEngine.cpp        # Engine implementation
├── sol2/                 # Sol2 Lua binding library
├── assets/               # Your game assets (images, etc.)
└── scripts/
    └── game_script.lua   # Example game script
```

## Key Features Explained

### 1. Coordinate System
- `location(x, y)`: Uses percentages (0-100) from top-left of window
- Objects are centered on their location coordinates
- Automatically adjusts for window resizing

### 2. Scaling System
- `scale(size)`: Percentage of window size (0-100)
- Maintains original aspect ratio by default
- Multiple scale modes available for different behaviors

### 3. Texture Management
- Automatic caching - same image only loaded once
- Memory management handled by engine
- Supports PNG, JPG, and other SDL_image formats

### 4. Rendering Pipeline
- Depth-based rendering order (`set_depth()`)
- Debug visualization with bounding boxes (F3 toggle)
- Resolution-independent positioning

### 5. Collision Detection
- Axis-Aligned Bounding Box (AABB) collision
- `check_collision()` method between objects
- `get_bounds()` to retrieve object boundaries

## Advanced Usage

### Custom Update Loop
```lua
function update(delta_time)
    -- Your game logic here
    -- delta_time is time since last frame in seconds
end
set_update_function(update)
```

### Object Management
```lua
-- Create object groups
local enemies = {}
for i = 1, 5 do
    enemies[i] = MakeObject("enemy.png")
    enemies[i]:location(i * 15, 50)
    enemies[i]:scale(8)
    enemies[i]:show()
end

-- Batch operations
function hide_all_enemies()
    for _, enemy in ipairs(enemies) do
        enemy:hide()
    end
end
```

### Input Handling
```lua
function update(dt)
    -- Movement with arrow keys
    local speed = 50 * dt  -- 50 pixels per second

    if is_key_pressed("Up") then
        -- Move up logic
    end
    if is_key_pressed("Down") then
        -- Move down logic
    end
    -- Add left/right handling
end
```

## Performance Tips

1. **Reuse Textures**: Same image path = same texture in memory
2. **Use Appropriate Scale**: Don't scale images larger than necessary
3. **Limit Object Count**: Each GameObject has overhead
4. **Use Depth Wisely**: Too many depth layers can impact sorting performance

## Troubleshooting

### Common Issues
1. **Images not loading**: Check file paths relative to script location
2. **Black screen**: Enable debug mode (`set_debug(true)`) to see bounding boxes
3. **Performance issues**: Reduce object count or image sizes

### Debug Mode
Press F3 during runtime to toggle debug visualization:
- Red boxes: Object boundaries
- Yellow indicators: Objects in delay state

## License

MIT License - feel free to use for your projects!

---

**Bimo-Engine**: Write games in Lua, run them anywhere. Perfect for beginners learning game development or experienced developers creating quick prototypes.
