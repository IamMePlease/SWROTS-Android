# SWROTS Android Port

An Android port of SWROTS-PC using emulation layers (box86/box64 + DXVK) with widescreen patches.

## ⚠️ Important: Build on a Computer, Not on Android

**You cannot build this project directly on an Android device.** The Android NDK toolchain only runs on x86_64 hosts (Linux/macOS/Windows). The cross-compilers are x86_64 binaries.

### Build Options

| Method | Difficulty | Requirements |
|--------|------------|--------------|
| **Local computer** (Recommended) | Easy | Linux/macOS/Windows + Android SDK |
| **GitHub Actions** | Easy | GitHub account |
| **GitHub Codespaces** | Easy | GitHub account (free tier) |
| **Cloud VM** | Medium | Any cloud provider |

---

## Option 1: Build on Your Computer (Recommended)

### Prerequisites
- **Linux/macOS/Windows** (with WSL2 on Windows)
- **JDK 17 or 21** (Temurin/OpenJDK)
- **Android SDK** (command line tools or Android Studio)
- **Android NDK r26+** (installed via SDK Manager)
- **CMake 3.22+** and **Ninja**

### Quick Setup (Linux/macOS)
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt update && sudo apt install -y openjdk-21-jdk cmake ninja-build git wget unzip

# Download Android command line tools
mkdir -p ~/android-sdk && cd ~/android-sdk
wget https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip
unzip commandlinetools-linux-11076708_latest.zip
rm commandlinetools-linux-11076708_latest.zip

# Install SDK/NDK
export ANDROID_HOME=~/android-sdk
export PATH=$PATH:$ANDROID_HOME/cmdline-tools/bin
yes | sdkmanager --sdk_root=$ANDROID_HOME "platforms;android-34" "build-tools;34.0.0" "ndk;26.1.10909125" "cmake;3.22.1"

# Clone and build
git clone https://github.com/YOUR_USERNAME/SWROTS-Android.git
cd SWROTS-Android
./build.sh
```

### Windows (WSL2)
Same as Linux - run inside WSL2 Ubuntu.

---

## Option 2: GitHub Actions (Easiest - No Local Setup)

1. **Fork this repository** to your GitHub account
2. Go to **Actions** tab → **Build SWROTS Android APK** → **Run workflow**
3. Wait 5-10 minutes
4. Download the APK from **Artifacts** (swrots-android-debug)

The workflow is at `.github/workflows/build.yml`.

---

## Option 3: GitHub Codespaces (Free Cloud Build)

1. Open the repo on GitHub
2. Click **Code** → **Codespaces** → **Create codespace on main**
3. Wait for environment to load (2-3 min)
4. In the terminal: `./build.sh`
5. Download the APK from the file explorer

---

## After Building

### Install on Device
```bash
adb install app/build/outputs/apk/debug/app-debug.apk
```

### Usage
1. Launch **SWROTS Android** app
2. Tap **"Load Game ISO"** - select your Xbox ISO file
3. Configure **Widescreen** in Settings (16:9, FOV, etc.)
4. Tap **"Start Game"**
5. Use **touch controls** or connect a **Bluetooth controller**

---

## Project Structure

```
android-swrots/
├── .github/workflows/build.yml    # GitHub Actions CI
├── build.sh                       # Local build script
├── CMakeLists.txt                 # Native build config
├── settings.gradle                # Gradle settings
├── build.gradle                   # Top-level Gradle
├── widescreen_patches.ini         # Widescreen config
├── app/
│   ├── build.gradle               # App module
│   └── src/main/
│       ├── AndroidManifest.xml
│       ├── java/com/swrots/android/
│       │   ├── MainActivity.java
│       │   └── InputHandler.java
│       └── cpp/                   # Native sources (copied from src/)
│           ├── CMakeLists.txt
│           ├── jni_bridge.cpp
│           ├── common.h
│           ├── emulation/
│           │   ├── box86_loader.cpp/h
│           │   ├── kernel_services.cpp/h
│           │   └── dxvk_bridge.cpp/h
│           ├── input/
│           │   ├── touch_input.cpp/h
│           │   └── controller_input.cpp/h
│           ├── audio/
│           │   └── audio_bridge.cpp/h
│           ├── patches/
│           │   └── widescreen.cpp/h
│           └── utils/
│               └── file_utils.cpp, log_utils.cpp/h
└── src/                           # Source of truth for native code
    └── (same as app/src/main/cpp/)
```

---

## Architecture

```
Android App (Java)
    ↓ JNI
Native Layer (C++)
    ├── Emulation Layer
    │   ├── box86/box64 (x86 dynarec) - NEEDS INTEGRATION
    │   ├── DXVK-Native (D3D9→Vulkan) - NEEDS INTEGRATION
    │   └── Xbox Kernel Services - STUBBED
    ├── Input Layer
    │   ├── Touch Controls (virtual gamepad)
    │   └── Controller Support (Bluetooth/USB)
    ├── Audio Layer (AAudio)
    └── Patches
        └── Widescreen (16:9, FOV, HUD scaling)
```

---

## Current Status: Starter Project

This is a **complete project structure with stub implementations**. The actual emulation components need integration:

| Component | Status | Notes |
|-----------|--------|-------|
| Project structure | ✅ Complete | Gradle, CMake, JNI, Java UI |
| JNI Bridge | ✅ Complete | Java ↔ C++ communication |
| box86/box64 loader | ⚠️ Stub | Needs real binaries |
| DXVK bridge | ⚠️ Stub | Needs DXVK-Native integration |
| Xbox kernel | ⚠️ Stub | Basic file/thread/memory |
| Touch input | ✅ Complete | Virtual gamepad zones |
| Controller input | ✅ Complete | Bluetooth/USB mapping |
| Audio bridge | ⚠️ Stub | AAudio initialization |
| Widescreen patches | ⚠️ Stub | Config + memory patching |
| ISO picker | ⚠️ Stub | SAF framework needed |

---

## Next Steps for Full Implementation

1. **Integrate box86/box64** - Download ARM64 builds, add to `third_party/`
2. **Integrate DXVK-Native** - Build DXVK for Android ARM64
3. **Implement Xbox kernel services** - Reverse engineer required syscalls
4. **Add ISO mounting** - Use libarchive to read Xbox ISO
5. **Complete Storage Access Framework** - Real file picker
6. **Test and optimize** - Performance profiling

---

## Legal

- Users must provide their own game ISO (North American Xbox release)
- No game data is included
- Based on SWROTS-PC (GPL-3.0)
- Uses box86/box64, DXVK, and other open-source components

## License

GPL-3.0 (same as SWROTS-PC)
