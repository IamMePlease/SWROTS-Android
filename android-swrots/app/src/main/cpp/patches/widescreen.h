#pragma once
#include <memory>

// Widescreen patch for 16:9 aspect ratio
class WidescreenPatch {
public:
    WidescreenPatch(int width, int height);
    ~WidescreenPatch();
    
    void apply();
    void remove();
    void setResolution(int width, int height);
    
    // Patch configuration
    struct PatchConfig {
        int width = 1920;
        int height = 1080;
        float fov = 75.0f;  // Field of view
        bool hud_scale = true;  // Scale HUD for widescreen
        bool aspect_ratio_fix = true;
    };
    
    PatchConfig& getConfig();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
