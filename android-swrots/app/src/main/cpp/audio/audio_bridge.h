#pragma once
#include <memory>

// Audio bridge: Xbox audio -> Android AAudio/OpenSL
class AudioBridge {
public:
    AudioBridge();
    ~AudioBridge();
    
    bool initialize();
    void shutdown();
    
    // Audio buffer submission
    void SubmitBuffer(const void* buffer, int size);
    
    // Volume control
    void SetVolume(float volume);
    
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
