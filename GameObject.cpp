#include "GameObject.hpp"
#include <SDL2/SDL_image.h>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

GameObject::GameObject(const std::string& image_path, SDL_Renderer* renderer, const std::string& base_dir)
    : visible(true), renderer(renderer) {

    fs::path full_path = fs::path(base_dir) / image_path;
    std::cout << "Loading image: " << full_path << std::endl;

    if (!fs::exists(full_path)) {
        std::cerr << "ERROR: File does not exist: " << full_path << std::endl;
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
    loc_x = x_percent;
    loc_y = y_percent;
}

void GameObject::scale(float size_percent) {
    scale_percent = size_percent;
}

void GameObject::render(int window_width, int window_height) {
    if (visible && texture) {
        // Calculate size based on window's smaller dimension to maintain aspect ratio
        float window_min_dimension = static_cast<float>(std::min(window_width, window_height));
        float base_size = window_min_dimension * (scale_percent / 100.0f);

        // Maintain original aspect ratio
        float aspect_ratio = static_cast<float>(original_height) / static_cast<float>(original_width);
        int pixel_width = static_cast<int>(base_size);
        int pixel_height = static_cast<int>(base_size * aspect_ratio);

        // Calculate position (center of image at the specified percentage)
        int pixel_x = static_cast<int>((window_width * loc_x / 100.0f) - (pixel_width / 2));
        int pixel_y = static_cast<int>((window_height * loc_y / 100.0f) - (pixel_height / 2));

        SDL_Rect dstrect = {pixel_x, pixel_y, pixel_width, pixel_height};
        SDL_RenderCopy(renderer, texture, nullptr, &dstrect);
    }
}
