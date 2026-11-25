#include "box86_loader.h"
#include "../common.h"
#include <dlfcn.h>

struct Box86Loader::Impl {
    void* box86_lib = nullptr;
    void* box64_lib = nullptr;
    bool initialized = false;
};

Box86Loader::Box86Loader() : impl(std::make_unique<Impl>()) {}

Box86Loader::~Box86Loader() {
    shutdown();
}

bool Box86Loader::initialize(const std::string& gamePath) {
    LOGI("Initializing Box86 for: %s", gamePath.c_str());
    
    // Load box86 library (32-bit x86 emulator)
    impl->box86_lib = dlopen("libbox86.so", RTLD_NOW);
    if (!impl->box86_lib) {
        LOGE("Failed to load libbox86.so: %s", dlerror());
        // Try box64 for 64-bit
        impl->box64_lib = dlopen("libbox64.so", RTLD_NOW);
        if (!impl->box64_lib) {
            LOGE("Failed to load libbox64.so: %s", dlerror());
            return false;
        }
        LOGI("Loaded box64 (64-bit mode)");
    } else {
        LOGI("Loaded box86 (32-bit mode)");
    }
    
    impl->initialized = true;
    return true;
}

bool Box86Loader::loadGame(const std::string& isoPath) {
    if (!impl->initialized) {
        LOGE("Box86 not initialized");
        return false;
    }
    
    LOGI("Loading game ISO: %s", isoPath.c_str());
    
    // TODO: Implement ISO mounting and game loading
    // This would involve:
    // 1. Mount the ISO as a virtual drive
    // 2. Extract XBE executable
    // 3. Configure box86/box64 environment
    // 4. Load the game executable
    
    return true;
}

void Box86Loader::runFrame() {
    // TODO: Execute one frame of the emulated game
    // This would call into box86/box64's execution loop
}

void Box86Loader::shutdown() {
    if (impl->box86_lib) {
        dlclose(impl->box86_lib);
        impl->box86_lib = nullptr;
    }
    if (impl->box64_lib) {
        dlclose(impl->box64_lib);
        impl->box64_lib = nullptr;
    }
    impl->initialized = false;
}
