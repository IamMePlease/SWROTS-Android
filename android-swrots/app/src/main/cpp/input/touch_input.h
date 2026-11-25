#pragma once
#include <vector>

// Touch input handler for virtual gamepad
class TouchInput {
public:
    TouchInput();
    ~TouchInput();
    
    void onTouchEvent(int action, int pointer_id, float x, float y, float pressure);
    
    // Virtual gamepad state
    struct GamepadState {
        float left_stick_x, left_stick_y;
        float right_stick_x, right_stick_y;
        bool button_a, button_b, button_x, button_y;
        bool button_start, button_select;
        bool trigger_left, trigger_right;
    };
    
    GamepadState& getState();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
