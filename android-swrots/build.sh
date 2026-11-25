#!/bin/bash
# Build script for SWROTS Android - run on a Linux/macOS/Windows computer

set -e

echo "=== SWROTS Android Build Script ==="
echo "Run this on a computer (Linux/macOS/Windows with WSL2), NOT on the Android device"

# Check for Android SDK
if [ -z "$ANDROID_HOME" ]; then
    echo "Error: ANDROID_HOME not set"
    echo "Please set ANDROID_HOME to your Android SDK path"
    echo "Example: export ANDROID_HOME=~/Android/Sdk"
    exit 1
fi

# Check for Java
if ! command -v java &> /dev/null; then
    echo "Error: Java not found. Please install JDK 17 or 21"
    exit 1
fi

echo "Java version:"
java -version

echo "Android SDK: $ANDROID_HOME"

# Create gradle wrapper if needed
if [ ! -f ./gradlew ]; then
    echo "Generating Gradle wrapper..."
    gradle wrapper
fi

# Build
echo "Building Debug APK..."
./gradlew assembleDebug

echo ""
echo "=== Build Complete ==="
echo "Debug APK: app/build/outputs/apk/debug/app-debug.apk"
echo ""
echo "To install on device:"
echo "  adb install app/build/outputs/apk/debug/app-debug.apk"
