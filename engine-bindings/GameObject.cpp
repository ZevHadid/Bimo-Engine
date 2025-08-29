#include "GameObject.hpp"
#include <SDL2/SDL_image.h>
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

int GameObject::REFERENCE_WIDTH = 800;
int GameObject::REFERENCE_HEIGHT = 600;

GameObject::GameObject(SDL_Texture* texture, SDL_Renderer* renderer)
    : visible(true), renderer(renderer), texture(texture) {

    if (!renderer) {
        throw std::runtime_error("Renderer is null");
    }

    if (!texture) {
        throw std::runtime_error("Texture is null");
    }

    SDL_QueryTexture(texture, NULL, NULL, &original_width, &original_height);
}

GameObject::~GameObject() {
    // Textures are managed by GameEngine's cache, so we don't destroy them here
}

void GameObject::show() { visible = true; }
void GameObject::hide() { visible = false; }
bool GameObject::is_visible() const { return visible; }

void GameObject::set_reference_dimensions(int width, int height) {
    if (width > 0 && height > 0) {
        REFERENCE_WIDTH = width;
        REFERENCE_HEIGHT = height;
    }
}

int GameObject::get_reference_width() {
    return REFERENCE_WIDTH;
}

int GameObject::get_reference_height() {
    return REFERENCE_HEIGHT;
}

void GameObject::location(float x_percent, float y_percent) {
    if (is_delayed_flag) return;

    x_percent = std::clamp(x_percent, 0.0f, 100.0f);
    y_percent = std::clamp(y_percent, 0.0f, 100.0f);
    base_loc_x = x_percent;
    base_loc_y = y_percent;
}

void GameObject::scale(float size_percent) {
    if (is_delayed_flag) return;

    base_scale_percent = std::max(0.0f, size_percent);
}

void GameObject::set_scale_mode(ScaleMode mode) {
    if (is_delayed_flag) return;

    scale_mode = mode;
}

void GameObject::set_depth(int depth) {
    if (is_delayed_flag) return;

    this->depth = depth;
}

int GameObject::get_depth() const {
    return depth;
}

void GameObject::delay(int milliseconds) {
    delay_end_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
    is_delayed_flag = true;
}

bool GameObject::is_delayed() const {
    return is_delayed_flag;
}

void GameObject::update(float delta_time) {
    if (is_delayed_flag) {
        if (std::chrono::steady_clock::now() >= delay_end_time) {
            is_delayed_flag = false;
        }
    }
}

void GameObject::render(int window_width, int window_height, bool show_debug) {
    if (visible && texture) {
        // Calculate scale factors
        float scale_x = static_cast<float>(window_width) / REFERENCE_WIDTH;
        float scale_y = static_cast<float>(window_height) / REFERENCE_HEIGHT;

        float scale_factor;
        switch (scale_mode) {
            case ScaleMode::Stretch:
                scale_factor = std::max(scale_x, scale_y);
                break;
            case ScaleMode::Crop:
                scale_factor = std::min(scale_x, scale_y);
                break;
            case ScaleMode::None:
                scale_factor = 1.0f;
                break;
            case ScaleMode::MaintainAspectRatio:
            default:
                scale_factor = std::min(scale_x, scale_y);
                break;
        }

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

        if (show_debug) {
            // Draw bounding box
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &dstrect);

            // Draw delay status
            if (is_delayed_flag) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_Rect delay_rect = {pixel_x, pixel_y - 5, 10, 5};
                SDL_RenderFillRect(renderer, &delay_rect);
            }
        }
    }
}
