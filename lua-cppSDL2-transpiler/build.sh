#!/bin/bash

# Use Python to properly escape the Lua script and create the header file
python3 << EOF > embedded_script.h
import re

# Read the Lua script
with open('script.lua', 'r') as f:
    lua_content = f.read()

# Escape the content for C++ string
escaped = lua_content.replace('\\\\', '\\\\\\\\').replace('"', '\\\\"').replace('\\n', '\\\\n\\\\\n')

# Create the header file
print('// Auto-generated embedded Lua script')
print('const char* EMBEDDED_LUA_SCRIPT =')
print('"{}";'.format(escaped))
EOF

# Compile with your specific flags
g++ main.cpp -I./sol2/include -I/usr/include/lua5.4 -o app -lSDL2 -llua5.4 -ldl -lm -std=c++17

if [ $? -eq 0 ]; then
    echo "Build complete! Executable created successfuly."
else
    echo "Build failed!"
    exit 1
fi
