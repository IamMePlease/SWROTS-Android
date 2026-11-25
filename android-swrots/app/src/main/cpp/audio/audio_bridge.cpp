#include "audio_bridge.h"
#include "../common.h"
#include <aaudio/AAudio.h>

struct AudioBridge::Impl {
    AAudioStream* stream = nullptr;
    bool initialized = false;
};

AudioBridge::AudioBridge() : impl(std::make_unique<Impl>()) {}

AudioBridge::~AudioBridge() {
    shutdown();
}

bool AudioBridge::initialize() {
    LOGI("Initializing audio bridge");
    
    // Create AAudio stream
    AAudioStreamBuilder* builder;
    AAudio_createStreamBuilder(&builder);
    
    AAudioStreamBuilder_setDeviceId(builder, AAUDIO_UNSPECIFIED);
    AAudioStreamBuilder_setChannelCount(builder, 2); // Stereo
    AAudioStreamBuilder_setSampleRate(builder, 48000);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
    AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_SHARED);
    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
    
    aaudio_result_t result = AAudioStreamBuilder_openStream(builder, &impl->stream);
    AAudioStreamBuilder_delete(builder);
    
    if (result != AAUDIO_OK) {
        LOGE("Failed to open audio stream: %d", result);
        return false;
    }
    
    impl->initialized = true;
    LOGI("Audio bridge initialized");
    return true;
}

void AudioBridge::shutdown() {
    if (impl->stream) {
        AAudioStream_close(impl->stream);
        impl->stream = nullptr;
    }
    impl->initialized = false;
}

void AudioBridge::SubmitBuffer(const void* buffer, int size) {
    if (!impl->initialized || !impl->stream) return;
    
    // Write audio data to stream
    AAudioStream_write(impl->stream, buffer, size, 0);
}

void AudioBridge::SetVolume(float volume) {
    // TODO: Set stream volume
}
