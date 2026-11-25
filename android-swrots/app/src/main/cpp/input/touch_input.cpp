#include "touch_input.h"
#include "../common.h"
#include <cmath>

struct TouchInput::Impl {
    GamepadState state{};
    int active_pointers = 0;
    
    // Touch zones for virtual gamepad
    struct TouchZone {
        float x, y;
        int pointer_id;
        bool active;
    };
    
    TouchZone left_stick_zone = {0.25f, 0.75f, -1, false};  // Left side
    TouchZone right_stick_zone = {0.75f, 0.75f, -1, false}; // Right side
    
    // Button zones
    struct ButtonZone {
        float x, y, radius;
        bool* state_ptr;
        const char* name;
    };
    
    std::vector<ButtonZone> button_zones = {
        {0.85f, 0.85f, 0.08f, &state.button_a, "A"},
        {0.92f, 0.75f, 0.08f, &state.button_b, "B"},
        {0.78f, 0.75f, 0.08f, &state.button_x, "X"},
        {0.85f, 0.68f, 0.08f, &state.button_y, "Y"},
    };
};

TouchInput::TouchInput() : impl(std::make_unique<Impl>()) {}

TouchInput::~TouchInput() = default;

void TouchInput::onTouchEvent(int action, int pointer_id, float x, float y, float pressure) {
    // Normalize coordinates to 0-1 range
    // (Android passes raw coordinates, we'd need screen size to normalize)
    
    switch (action) {
        case 0: // ACTION_DOWN
            impl->active_pointers++;
            
            // Check if touch is in a button zone
            for (auto& zone : impl->button_zones) {
                float dx = x - zone.x;
                float dy = y - zone.y;
                if (std::sqrt(dx*dx + dy*dy) < zone.radius) {
                    *zone.state_ptr = true;
                    LOGI("Button %s pressed", zone.name);
                }
            }
            
            // Check stick zones
            if (x < 0.5f) {
                impl->left_stick_zone.pointer_id = pointer_id;
                impl->left_stick_zone.active = true;
            } else {
                impl->right_stick_zone.pointer_id = pointer_id;
                impl->right_stick_zone.active = true;
            }
            break;
            
        case 1: // ACTION_UP
            impl->active_pointers--;
            
            // Release buttons
            for (auto& zone : impl->button_zones) {
                *zone.state_ptr = false;
            }
            
            // Release sticks
            if (impl->left_stick_zone.pointer_id == pointer_id) {
                impl->left_stick_zone.active = false;
                impl->left_stick_zone.pointer_id = -1;
                impl->state.left_stick_x = 0;
                impl->state.left_stick_y = 0;
            }
            if (impl->right_stick_zone.pointer_id == pointer_id) {
                impl->right_stick_zone.active = false;
                impl->right_stick_zone.pointer_id = -1;
                impl->state.right_stick_x = 0;
                impl->state.right_stick_y = 0;
            }
            break;
            
        case 2: // ACTION_MOVE
            // Update stick positions
            if (impl->left_stick_zone.active && impl->left_stick_zone.pointer_id == pointer_id) {
                // Map to -1 to 1 range
                float delta_x = (x - impl->left_stick_zone.x) * 2.0f;
                float delta_y = (y - impl->left_stick_zone.y) * 2.0f;
                impl->state.left_stick_x = std::max(-1.0f, std::min(1.0f, delta_x));
                impl->state.left_stick_y = std::max(-1.0f, std::min(1.0f, delta_y));
            }
            if (impl->right_stick_zone.active && impl->right_stick_zone.pointer_id == pointer_id) {
                float delta_x = (x - impl->right_stick_zone.x) * 2.0f;
                float delta_y = (y - impl->right_stick_zone.y) * 2.0f;
                impl->state.right_stick_x = std::max(-1.0f, std::min(1.0f, delta_x));
                impl->state.right_stick_y = std::max(-1.0f, std::min(1.0f, delta_y));
            }
            break;
    }
}

TouchInput::GamepadState& TouchInput::getState() {
    return impl->state;
}
