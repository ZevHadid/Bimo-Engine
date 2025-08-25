# 🎮 Lua → SDL2 Graphics Transpiler

**Turn Lua scripts into standalone graphics applications!** Write your graphics in Lua using SVG-like syntax and compile to high-performance SDL2 executables.

---

## ✨ Features

- **📝 Lua Scripting**: Write graphics code in simple, clean Lua
- **🎨 SVG-like Syntax**: Use familiar HTML-like tags: `<rect>`, `<circle>`, `<polygon>`, `<line>`
- **⚡ Native Performance**: Compiled to optimized C++ with SDL2 backend
- **📦 Standalone Executables**: Single file output, no dependencies needed
- **🔧 Cross-Platform**: Works on Linux, Windows, and macOS

---

## 🚀 Quick Start

### 1. Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt update && sudo apt install -y g++ libsdl2-dev liblua5.4-dev python3 git make


**Arch Linux:**
```bash
sudo pacman -Syu --needed gcc sdl2 lua54 python git make
```

**macOS (Homebrew):**
```bash
brew update && brew install gcc sdl2 lua@5.4 python git make
```

### 2. Get the Code
```bash
git clone https://github.com/yourusername/lua-sdl2-transpiler.git
cd lua-sdl2-transpiler
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
./build.sh    # Compiles to standalone executable
./app         # Run your graphics application!
```

---

## 🛠️ Build Options

**Development Build** (with external script file):
```bash
g++ main.cpp -I./sol2/include -I/usr/include/lua5.4 -o dev_app -lSDL2 -llua5.4 -ldl -lm -std=c++17
```

**Production Build** (embedded, standalone):
```bash
./build.sh  # Creates self-contained 'app' executable
```

---

## 🎨 Available Shapes

### Rectangle
```lua
rect = MakeShape('<rect x="50" y="50" width="200" height="100" fill="red"/>')
```

### Circle  
```lua
circle = MakeShape('<circle cx="400" cy="300" r="50" fill="blue"/>')
```

### Line
```lua
line = MakeShape('<line x1="50" y1="500" x2="750" y2="500" stroke="green"/>')
```

### Polygon
```lua
star = MakeShape('<polygon points="400,100 420,140 460,140 430,170 440,210 400,190 360,210 370,170 340,140 380,140" fill="gold"/>')
```

### Colors Supported:
- `red`, `blue`, `green`, `gold`
- *More colors can be added in parseSVG() function*

---

## 📁 Project Structure

```
lua-sdl2-transpiler/
├── main.cpp          # Main C++ application
├── build.sh          # Build script (creates standalone)
├── script.lua        # Your graphics script (edit this!)
├── sol2/            # sol2 library (git cloned)
└── embedded_script.h # Auto-generated (don't edit)
```

---

## 🔧 Advanced Usage

### Adding New Colors
Edit the `parseSVG()` function in `main.cpp` to add more color support:

```cpp
if(color == "purple") s.r_col=128, s.g_col=0, s.b_col=128;
if(color == "orange") s.r_col=255, s.g_col=165, s.b_col=0;
```

### Custom Build Flags
Modify `build.sh` for custom compilation:

```bash
# Add optimization flags
g++ main.cpp -O3 -march=native -I./sol2/include -I/usr/include/lua5.4 \
    -o app -lSDL2 -llua5.4 -ldl -lm -std=c++17
```

---

## ❓ Troubleshooting

### "SDL2 not found"
```bash
sudo apt install libsdl2-dev  # Ubuntu/Debian
sudo pacman -S sdl2           # Arch
brew install sdl2             # macOS
```

### "Lua not found"
```bash
sudo apt install liblua5.4-dev  # Ubuntu/Debian
sudo pacman -S lua54           # Arch  
brew install lua@5.4           # macOS
```

### "sol2 not found"
```bash
git clone https://github.com/ThePhD/sol2.git
```

---

## 🎯 How It Works

1. **Lua Script** → Your graphics code
2. **Build Process** → Embeds Lua into C++ executable
3. **SDL2 Backend** → Renders graphics with hardware acceleration
4. **Standalone EXE** → No dependencies required!

---

## 📝 License

MIT License - feel free to use this for your own projects!

---

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📧 Support

Having issues? 
- 📋 Open an issue on GitHub
- 💬 Discuss in discussions
- 🐛 Check existing issues for solutions

---

**Happy Coding!** 🚀✨

*Turn your Lua graphics dreams into reality!*
```

This README provides:
- ✅ Clean, professional formatting
- ✅ Easy-to-follow installation instructions  
- ✅ Usage examples with code snippets
- ✅ Troubleshooting section
- ✅ Technical details for advanced users
- ✅ Visual hierarchy with emojis and sections

Just save this as `README.md` in your project directory! 🎯
