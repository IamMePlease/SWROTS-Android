#pragma once
#include <string>
#include <memory>

// Box86/Box64 x86 emulator loader
class Box86Loader {
public:
    Box86Loader();
    ~Box86Loader();
    
    bool initialize(const std::string& gamePath);
    bool loadGame(const std::string& isoPath);
    void runFrame();
    void shutdown();
    
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
