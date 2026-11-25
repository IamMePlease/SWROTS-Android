#include <jni.h>
#include <android/log.h>
#include <string>
#include <memory>
#include "emulation/box86_loader.h"
#include "emulation/kernel_services.h"
#include "emulation/dxvk_bridge.h"
#include "input/touch_input.h"
#include "input/controller_input.h"
#include "audio/audio_bridge.h"
#include "patches/widescreen.h"
#include "utils/log_utils.h"

#define TAG "SWROTS-JNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

static std::unique_ptr<Box86Loader> g_box86;
static std::unique_ptr<KernelServices> g_kernel;
static std::unique_ptr<DXVKBridge> g_dxvk;
static std::unique_ptr<TouchInput> g_touch;
static std::unique_ptr<ControllerInput> g_controller;
static std::unique_ptr<AudioBridge> g_audio;
static std::unique_ptr<WidescreenPatch> g_widescreen;

extern "C" {

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad");
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_swrots_android_MainActivity_nativeInitialize(
    JNIEnv* env, jobject /* this */,
    jstring game_path,
    jstring iso_path,
    jboolean enable_widescreen,
    jint widescreen_width,
    jint widescreen_height
) {
    const char* gamePath = env->GetStringUTFChars(game_path, nullptr);
    const char* isoPath = env->GetStringUTFChars(iso_path, nullptr);
    
    LOGI("Initializing SWROTS: game=%s, iso=%s", gamePath, isoPath);
    
    // Initialize emulation layer
    g_box86 = std::make_unique<Box86Loader>();
    if (!g_box86->initialize(gamePath)) {
        LOGE("Failed to initialize box86");
        return;
    }
    
    // Initialize Xbox kernel services
    g_kernel = std::make_unique<KernelServices>();
    g_kernel->initialize();
    
    // Initialize DXVK bridge (D3D9 -> Vulkan)
    g_dxvk = std::make_unique<DXVKBridge>();
    if (!g_dxvk->initialize(android_app->window)) {
        LOGE("Failed to initialize DXVK");
        return;
    }
    
    // Initialize input systems
    g_touch = std::make_unique<TouchInput>();
    g_controller = std::make_unique<ControllerInput>();
    g_controller->initialize();
    
    // Initialize audio
    g_audio = std::make_unique<AudioBridge>();
    g_audio->initialize();
    
    // Apply widescreen patch if enabled
    if (enable_widescreen) {
        g_widescreen = std::make_unique<WidescreenPatch>(widescreen_width, widescreen_height);
        g_widescreen->apply();
        LOGI("Widescreen patch applied: %dx%d", widescreen_width, widescreen_height);
    }
    
    // Load the game
    if (!g_box86->loadGame(isoPath)) {
        LOGE("Failed to load game ISO");
        return;
    }
    
    LOGI("SWROTS initialized successfully");
    
    env->ReleaseStringUTFChars(game_path, gamePath);
    env->ReleaseStringUTFChars(iso_path, isoPath);
}

JNIEXPORT void JNICALL
Java_com_swrots_android_MainActivity_nativeShutdown(JNIEnv* env, jobject /* this */) {
    LOGI("Shutting down SWROTS");
    
    g_widescreen.reset();
    g_audio.reset();
    g_controller.reset();
    g_touch.reset();
    g_dxvk.reset();
    g_kernel.reset();
    g_box86->shutdown();
    g_box86.reset();
}

JNIEXPORT void JNICALL
Java_com_swrots_android_MainActivity_nativeRenderFrame(JNIEnv* env, jobject /* this */) {
    if (g_box86) {
        g_box86->runFrame();
    }
}

JNIEXPORT void JNICALL
Java_com_swrots_android_InputHandler_nativeTouchEvent(
    JNIEnv* env, jobject /* this */,
    jint action, jint pointer_id,
    jfloat x, jfloat y,
    jfloat pressure
) {
    if (g_touch) {
        g_touch->onTouchEvent(action, pointer_id, x, y, pressure);
    }
}

JNIEXPORT void JNICALL
Java_com_swrots_android_InputHandler_nativeControllerEvent(
    JNIEnv* env, jobject /* this */,
    jint event_type, jint key_code, jfloat value
) {
    if (g_controller) {
        g_controller->onControllerEvent(event_type, key_code, value);
    }
}

JNIEXPORT jstring JNICALL
Java_com_swrots_android_MainActivity_nativeGetVersion(JNIEnv* env, jobject /* this */) {
    return env->NewStringUTF("SWROTS-Android 0.1.0");
}

JNIEXPORT jboolean JNICALL
Java_com_swrots_android_MainActivity_nativeIsWidescreenSupported(JNIEnv* env, jobject /* this */) {
    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_swrots_android_MainActivity_nativeSetWidescreenPatch(
    JNIEnv* env, jobject /* this */,
    jboolean enabled, jint width, jint height
) {
    if (g_widescreen) {
        if (enabled) {
            g_widescreen->setResolution(width, height);
            g_widescreen->apply();
        } else {
            g_widescreen->remove();
        }
    }
}

} // extern "C"
