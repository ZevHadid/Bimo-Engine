#include "GameObject.hpp"
#include <SDL2/SDL_image.h>
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

GameObject::GameObject(const std::string& image_path, SDL_Renderer* renderer, const std::string& base_dir)
    : visible(true), renderer(renderer) {

    // SIMPLE: Just use base_dir + image_path exactly as provided
    fs::path full_path = fs::path(base_dir) / image_path;
    std::cout << "Loading image from: " << full_path << std::endl;

    // Check if the resolved path exists
    if (!fs::exists(full_path)) {
        // Get the absolute path for better error message
        fs::path absolute_path = fs::absolute(full_path);
        std::cerr << "ERROR: File does not exist: " << absolute_path << std::endl;

        // Show what directory we're relative to
        std::cerr << "Script directory: " << fs::absolute(base_dir) << std::endl;
        std::cerr << "Requested path: " << image_path << std::endl;
        return;
    }

    SDL_Surface* surface = IMG_Load(full_path.c_str());
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    } else {
        std::cout << "Successfully loaded: " << full_path << std::endl;
        SDL_QueryTexture(texture, NULL, NULL, &original_width, &original_height);
    }
}

GameObject::~GameObject() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void GameObject::show() { visible = true; }
void GameObject::hide() { visible = false; }
bool GameObject::is_visible() const { return visible; }

void GameObject::location(float x_percent, float y_percent) {
    base_loc_x = x_percent;
    base_loc_y = y_percent;
}

void GameObject::scale(float size_percent) {
    base_scale_percent = size_percent;
}

void GameObject::render(int window_width, int window_height) {
    if (visible && texture) {
        // Calculate scale factors
        float scale_x = static_cast<float>(window_width) / REFERENCE_WIDTH;
        float scale_y = static_cast<float>(window_height) / REFERENCE_HEIGHT;

        // Only scale if both dimensions increased proportionally
        float scale_factor = std::min(scale_x, scale_y);

        // If window is smaller than reference, don't scale down (clamp at 1.0)
        scale_factor = std::max(scale_factor, 1.0f);

        // Calculate size based on reference window
        float reference_min_dimension = static_cast<float>(std::min(REFERENCE_WIDTH, REFERENCE_HEIGHT));
        float base_size = reference_min_dimension * (base_scale_percent / 100.0f);

        // Apply scale factor to size
        float scaled_size = base_size * scale_factor;

        // Maintain original aspect ratio
        float aspect_ratio = static_cast<float>(original_height) / static_cast<float>(original_width);
        int pixel_width = static_cast<int>(scaled_size);
        int pixel_height = static_cast<int>(scaled_size * aspect_ratio);

        // Calculate position - only scale positions when window grows proportionally
        int scaled_reference_width = static_cast<int>(REFERENCE_WIDTH * scale_factor);
        int scaled_reference_height = static_cast<int>(REFERENCE_HEIGHT * scale_factor);

        // Center the "virtual reference window" within the actual window
        int offset_x = (window_width - scaled_reference_width) / 2;
        int offset_y = (window_height - scaled_reference_height) / 2;

        // Calculate position within the scaled reference window
        int pixel_x = offset_x + static_cast<int>((scaled_reference_width * base_loc_x / 100.0f) - (pixel_width / 2));
        int pixel_y = offset_y + static_cast<int>((scaled_reference_height * base_loc_y / 100.0f) - (pixel_height / 2));

        SDL_Rect dstrect = {pixel_x, pixel_y, pixel_width, pixel_height};
        SDL_RenderCopy(renderer, texture, nullptr, &dstrect);
    }
}
