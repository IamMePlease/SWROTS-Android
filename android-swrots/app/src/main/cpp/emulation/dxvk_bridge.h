#pragma once
#include <memory>
#include <string>

// DXVK bridge: Direct3D 9 -> Vulkan translation
class DXVKBridge {
public:
    DXVKBridge();
    ~DXVKBridge();
    
    bool initialize(void* android_window);
    void shutdown();
    
    // D3D9 device methods (stubs)
    int CreateDevice(int adapter, int device_type, void* focus_window,
                     int behavior_flags, void* presentation_parameters,
                     void** device);
    
    // Present
    void Present(void* src_rect, void* dst_rect, void* dirty_rect);
    
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
