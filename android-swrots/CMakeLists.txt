cmake_minimum_required(VERSION 3.22.1)
project(SWROTSAndroid LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use NDK's toolchain - don't override these
# set(CMAKE_ANDROID_NDK ${ANDROID_NDK})  # Set by toolchain
# set(CMAKE_ANDROID_ARCH_ABI ${ANDROID_ABI})  # Set by toolchain

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    src/jni_bridge.cpp
    src/emulation/box86_loader.cpp
    src/emulation/kernel_services.cpp
    src/emulation/dxvk_bridge.cpp
    src/input/touch_input.cpp
    src/input/controller_input.cpp
    src/audio/audio_bridge.cpp
    src/patches/widescreen.cpp
    src/utils/file_utils.cpp
    src/utils/log_utils.cpp
)

# Create shared library
add_library(swrots SHARED ${SOURCES})

# Link Android libraries
target_link_libraries(swrots
    log
    android
    vulkan
    EGL
    GLESv3
)

# Compile options
target_compile_options(swrots PRIVATE
    -Wall -Wextra -Wpedantic
    -fno-exceptions -fno-rtti
    -ffast-math
)

# Output directory
set_target_properties(swrots PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/app/src/main/jniLibs/${ANDROID_ABI}
)
