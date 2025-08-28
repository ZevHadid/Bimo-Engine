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

    void location(float x_percent, float y_percent);
    void scale(float size_percent);

private:
    SDL_Texture* texture = nullptr;
    SDL_Renderer* renderer;
    bool visible;
    int original_width = 0;
    int original_height = 0;

    // Base properties (for reference window size)
    float base_loc_x = 50.0f;
    float base_loc_y = 50.0f;
    float base_scale_percent = 20.0f;

    // Reference window size (what we designed for)
    static const int REFERENCE_WIDTH = 800;
    static const int REFERENCE_HEIGHT = 600;
};
