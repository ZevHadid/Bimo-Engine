# 🎮 Bimo ENgine
## Lua → SDL2 Graphics Embedder

**Embed Lua scripts directly into high-performance SDL2 applications!** Write your graphics in Lua and compile to standalone native executables with no external dependencies.

---

## ✨ Features

- **📝 Lua Scripting**: Write graphics code in simple, clean Lua
- **⚡ Zero Dependencies**: Lua code embedded directly into executable
- **🎨 SVG-like Syntax**: Use familiar HTML-like tags: `<rect>`, `<circle>`, `<polygon>`, `<line>`
- **🚀 Native Performance**: Direct C++ compilation with SDL2 backend
- **📦 Single File Output**: No external files needed to run
- **🔧 Cross-Platform**: Works on Linux, Windows, and macOS

---

## 🚀 Quick Start

### 1. Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt update && sudo apt install -y g++ libsdl2-dev liblua5.4-dev python3 git
```

**Arch Linux:**
```bash
sudo pacman -Syu --needed gcc sdl2 lua54 python git
```

**macOS (Homebrew):**
```bash
brew update && brew install gcc sdl2 lua@5.4 python git
```

### 2. Get the Code
```bash
git clone https://github.com/ZevHadid/Bimo-Engine.git
cd lua-cppSDL2-embedder
```

### 3. Install sol2 (Header-only Library)
```bash
git clone https://github.com/ThePhD/sol2.git
```

### 4. Write Your Graphics Script (`script.lua`)
```lua
-- Create a red rectangle
rect = MakeShape('<rect x="300" y="250" width="200" height="100" fill="red"/>')

-- Create a blue circle  
circle = MakeShape('<circle cx="400" cy="300" r="50" fill="blue"/>')

-- Show the shapes
rect:show()
circle:show()

-- Add a gold star
star = MakeShape('<polygon points="400,100 420,140 460,140 430,170 440,210 400,190 360,210 370,170 340,140 380,140" fill="gold"/>')
star:show()
```

### 5. Build & Run!
```bash
./build.sh    # Embeds Lua into executable and compiles
./app         # Run your standalone graphics app!
```

---

## 🧠 How It Works

### Compilation Process:
1. **Lua Script** → Your graphics code (`script.lua`)
2. **Build Script** → Embeds Lua code into C++ string (`build.sh`)
3. **C++ Compilation** → Creates native executable with embedded Lua
4. **Runtime** → Lua VM executes embedded code calling SDL2 functions

### Architecture:
```
[Your Lua Code] → [Build Process] → [C++ Executable with Embedded Lua] → [SDL2 Graphics]
```

---

## 🛠️ Build System

### Production Build (Recommended):
```bash
./build.sh  # Creates standalone 'app' with embedded Lua
```

### Manual Build (Development):
```bash
# For testing with external script file
g++ main.cpp -I./sol2/include -I/usr/include/lua5.4 -o dev_app -lSDL2 -llua5.4 -ldl -lm -std=c++17
```

### The Build Script Magic:
```bash
#!/bin/bash
# build.sh - Embeds Lua into C++ and compiles

# 1. Convert Lua to C++ string using Python
python3 << EOF > embedded_script.h
# Python code reads script.lua and creates embedded_script.h
EOF

# 2. Compile with embedded code
g++ main.cpp -I./sol2/include -I/usr/include/lua5.4 -o app -lSDL2 -llua5.4 -ldl -lm -std=c++17
```

---

## 🎨 Available Shapes & Methods

### Shapes:
```lua
-- Rectangle
rect = MakeShape('<rect x="50" y="50" width="200" height="100" fill="red"/>')

-- Circle
circle = MakeShape('<circle cx="400" cy="300" r="50" fill="blue"/>')

-- Line
line = MakeShape('<line x1="50" y1="500" x2="750" y2="500" stroke="green"/>')

-- Polygon
star = MakeShape('<polygon points="400,100 420,140 460,140 430,170 440,210 400,190 360,210 370,170 340,140 380,140" fill="gold"/>')
```

### Methods:
```lua
shape:show()  -- Make shape visible
shape:hide()  -- Hide shape
```

### Supported Colors:
- `red`, `blue`, `green`, `gold`
- *Extend by modifying `parseSVG()` in main.cpp*

---

## 📁 Project Structure

```
Bimo-Engine/
├── main.cpp              # Main C++ application
├── build.sh              # Build script (embeds + compiles)
├── script.lua            # Your graphics script (edit this!)
├── embedded_script.h     # Auto-generated (contains embedded Lua)
└── sol2/                 # sol2 library (Lua-C++ binding)
```

---

## 🔧 Advanced Usage

### Adding New Colors:
Edit the `parseSVG()` function in `main.cpp`:

```cpp
if(color == "purple") s.r_col=128, s.g_col=0, s.b_col=128;
if(color == "orange") s.r_col=255, s.g_col=165, s.b_col=0;
```

### Custom Build Optimization:
Modify `build.sh` for maximum performance:

```bash
g++ main.cpp -O3 -march=native -I./sol2/include -I/usr/include/lua5.4 \
    -o app -lSDL2 -llua5.4 -ldl -lm -std=c++17
```

---

## ❓ Troubleshooting

### "SDL2 not found"
```bash
sudo apt install libsdl2-dev    # Ubuntu/Debian
sudo pacman -S sdl2             # Arch
brew install sdl2               # macOS
```

### "Lua not found" 
```bash
sudo apt install liblua5.4-dev  # Ubuntu/Debian
sudo pacman -S lua54            # Arch
brew install lua@5.4            # macOS
```

### Build Issues:
```bash
# Clean and rebuild
rm -f app embedded_script.h
./build.sh
```

---

## 🎯 Performance Benefits

- **🚀 Faster Startup**: No file I/O - Lua code in memory
- **⚡ Better Performance**: Eliminated disk access overhead
- **🔒 Improved Security**: Code cannot be modified post-compilation
- **📦 Simplified Distribution**: Single executable file

---

## 📝 License

MIT License - free for personal and commercial use!

---

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`) 
5. Open a Pull Request

---

## 💡 Learning Resources

- [SDL2 Documentation](https://wiki.libsdl.org/)
- [Lua 5.4 Reference](https://www.lua.org/manual/5.4/)
- [sol2 Lua-C++ Binding](https://github.com/ThePhD/sol2)

