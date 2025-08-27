#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <memory>

class GameObject {
public:
    GameObject(const std::string& image_path, SDL_Renderer* renderer, const std::string& base_dir);
    ~GameObject();

    void show();
    void hide();
    bool is_visible() const;
    void render(int window_width, int window_height);

    // Simple coordinate methods
    void location(float x_percent, float y_percent);  // 0-100% from top-left
    void scale(float size_percent);  // 0-100% of window's smaller dimension

private:
    SDL_Texture* texture = nullptr;
    SDL_Renderer* renderer;
    bool visible;
    int original_width = 0;
    int original_height = 0;

    // Position and scale properties
    float loc_x = 50.0f;    // 50% from left (center)
    float loc_y = 50.0f;    // 50% from top (center)
    float scale_percent = 20.0f; // 20% of window size
};
