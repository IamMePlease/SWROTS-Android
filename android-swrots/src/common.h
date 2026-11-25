#pragma once
#include <jni.h>
#include <android/log.h>
#include <string>
#include <memory>

// Forward declarations
class Box86Loader;
class KernelServices;
class DXVKBridge;
class TouchInput;
class ControllerInput;
class AudioBridge;
class WidescreenPatch;

// Global application state
struct AppState {
    std::unique_ptr<Box86Loader> box86;
    std::unique_ptr<KernelServices> kernel;
    std::unique_ptr<DXVKBridge> dxvk;
    std::unique_ptr<TouchInput> touch;
    std::unique_ptr<ControllerInput> controller;
    std::unique_ptr<AudioBridge> audio;
    std::unique_ptr<WidescreenPatch> widescreen;
    
    std::string gamePath;
    std::string isoPath;
    bool initialized = false;
};

// Get global state
AppState* getAppState();

// Log utilities
#define LOG_TAG "SWROTS"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
