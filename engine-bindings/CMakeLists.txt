cmake_minimum_required(VERSION 3.10)
project(BimoEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find dependencies using PkgConfig, mirroring the g++ command
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA REQUIRED lua5.4)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# Create the executable from the source files
add_executable(bimo-engine
    main.cpp
    GameObject.cpp
    GameEngine.cpp
)

# Add include directories
target_include_directories(bimo-engine PRIVATE
    ${LUA_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    "sol2/include"  # As mentioned in the README
)

# Link the required libraries
target_link_libraries(bimo-engine PRIVATE
    ${LUA_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)
