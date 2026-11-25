#pragma once
#include <memory>

// Bluetooth/USB controller input handler
class ControllerInput {
public:
    ControllerInput();
    ~ControllerInput();
    
    bool initialize();
    void shutdown();
    
    void onControllerEvent(int event_type, int key_code, float value);
    
    // Controller state
    struct ControllerState {
        float left_stick_x, left_stick_y;
        float right_stick_x, right_stick_y;
        bool button_a, button_b, button_x, button_y;
        bool button_start, button_select;
        bool trigger_left, trigger_right;
        bool connected = false;
    };
    
    ControllerState& getState();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
