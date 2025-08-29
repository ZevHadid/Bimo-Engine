#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include <chrono>

enum class ScaleMode {
    MaintainAspectRatio,
    Stretch,
    Crop,
    None
};

class GameObject {
public:
    GameObject(SDL_Texture* texture, SDL_Renderer* renderer);
    ~GameObject();

    void show();
    void hide();
    bool is_visible() const;
    void render(int window_width, int window_height, bool show_debug = false);
    void update(float delta_time);

    void location(float x_percent, float y_percent);
    void scale(float size_percent);
    void set_scale_mode(ScaleMode mode);
    void set_depth(int depth);
    int get_depth() const;

    void delay(int milliseconds);
    bool is_delayed() const;

    static void set_reference_dimensions(int width, int height);
    static int get_reference_width();
    static int get_reference_height();

private:
    SDL_Texture* texture = nullptr;
    SDL_Renderer* renderer;
    bool visible;
    int original_width = 0;
    int original_height = 0;
    int depth = 0;
    ScaleMode scale_mode = ScaleMode::MaintainAspectRatio;

    // Delay functionality
    std::chrono::steady_clock::time_point delay_end_time;
    bool is_delayed_flag = false;

    // Base properties (for reference window size)
    float base_loc_x = 50.0f;
    float base_loc_y = 50.0f;
    float base_scale_percent = 20.0f;

    // Reference window size (what we designed for)
    static int REFERENCE_WIDTH;
    static int REFERENCE_HEIGHT;
};
