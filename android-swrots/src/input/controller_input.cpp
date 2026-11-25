#include "controller_input.h"
#include "../common.h"

struct ControllerInput::Impl {
    ControllerState state;
    bool initialized = false;
};

ControllerInput::ControllerInput() : impl(std::make_unique<Impl>()) {}

ControllerInput::~ControllerInput() {
    shutdown();
}

bool ControllerInput::initialize() {
    LOGI("Initializing controller input");
    impl->initialized = true;
    return true;
}

void ControllerInput::shutdown() {
    impl->initialized = false;
}

void ControllerInput::onControllerEvent(int event_type, int key_code, float value) {
    if (!impl->initialized) return;
    
    // Map Android key codes to gamepad buttons
    // (These would come from Android's input system)
    
    switch (key_code) {
        case 96: // KEYCODE_BUTTON_A
            impl->state.button_a = (value > 0.5f);
            break;
        case 97: // KEYCODE_BUTTON_B
            impl->state.button_b = (value > 0.5f);
            break;
        case 99: // KEYCODE_BUTTON_X
            impl->state.button_x = (value > 0.5f);
            break;
        case 100: // KEYCODE_BUTTON_Y
            impl->state.button_y = (value > 0.5f);
            break;
        case 108: // KEYCODE_BUTTON_START
            impl->state.button_start = (value > 0.5f);
            break;
        case 109: // KEYCODE_BUTTON_SELECT
            impl->state.button_select = (value > 0.5f);
            break;
    }
}

ControllerInput::ControllerState& ControllerInput::getState() {
    return impl->state;
}
