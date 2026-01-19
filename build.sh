#!/bin/bash

# Individual Build Script - Build specific platform/architecture
# Usage: ./build.sh [macos|macos-arm64|macos-x86_64|macos-universal|linux|windows|all]

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() { echo -e "${BLUE}[INFO]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# Default to all if no argument
BUILD_TARGET="${1:-all}"

build_macos_native() {
    print_status "Building macOS (native)..."
    rm -rf build-mac-native
    cmake -B build-mac-native -DCMAKE_BUILD_TYPE=Release
    cmake --build build-mac-native --config Release
    if [ -f "build-mac-native/br" ]; then
        print_success "macOS native: build-mac-native/br"
        file build-mac-native/br
    fi
}

build_macos_arm64() {
    print_status "Building macOS ARM64..."
    rm -rf build-mac-arm64
    cmake -B build-mac-arm64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
    cmake --build build-mac-arm64 --config Release
    if [ -f "build-mac-arm64/br" ]; then
        print_success "macOS ARM64: build-mac-arm64/br"
        file build-mac-arm64/br
    fi
}

build_macos_x86_64() {
    print_status "Building macOS x86_64..."
    rm -rf build-mac-x86_64
    cmake -B build-mac-x86_64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=x86_64
    cmake --build build-mac-x86_64 --config Release
    if [ -f "build-mac-x86_64/br" ]; then
        print_success "macOS x86_64: build-mac-x86_64/br"
        file build-mac-x86_64/br
    fi
}

build_macos_universal() {
    print_status "Building macOS Universal Binary..."
    rm -rf build-mac-universal
    cmake -B build-mac-universal -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
    cmake --build build-mac-universal --config Release
    if [ -f "build-mac-universal/br" ]; then
        print_success "macOS Universal: build-mac-universal/br"
        file build-mac-universal/br
    fi
}

build_linux() {
    print_status "Building Linux (using Docker)..."
    if ! command -v docker &> /dev/null; then
        print_error "Docker is not installed. Install from https://www.docker.com/products/docker-desktop"
        return 1
    fi
    rm -rf build-linux
    docker run --rm -v "$SCRIPT_DIR:/workspace" -w /workspace \
        gcc:latest bash -c "
        apt-get update -qq && \
        apt-get install -y -qq cmake && \
        cmake -B build-linux -DCMAKE_BUILD_TYPE=Release && \
        cmake --build build-linux --config Release
    "
    if [ -f "build-linux/br" ]; then
        print_success "Linux: build-linux/br"
        file build-linux/br
    fi
}

build_windows() {
    print_status "Building Windows (using MinGW-w64)..."
    if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
        print_error "MinGW-w64 is not installed. Install with: brew install mingw-w64"
        return 1
    fi
    rm -rf build-windows
    cmake -B build-windows \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
        -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
        -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres
    cmake --build build-windows --config Release
    if [ -f "build-windows/br.exe" ]; then
        print_success "Windows: build-windows/br.exe"
        file build-windows/br.exe
    fi
}

case "$BUILD_TARGET" in
    macos)
        build_macos_native
        ;;
    macos-arm64)
        build_macos_arm64
        ;;
    macos-x86_64)
        build_macos_x86_64
        ;;
    macos-universal)
        build_macos_universal
        ;;
    linux)
        build_linux
        ;;
    windows)
        build_windows
        ;;
    all)
        build_macos_native
        build_macos_arm64
        build_macos_x86_64
        build_macos_universal
        build_linux
        build_windows
        ;;
    *)
        echo "Usage: $0 [macos|macos-arm64|macos-x86_64|macos-universal|linux|windows|all]"
        echo ""
        echo "Options:"
        echo "  macos           - Build for macOS (native architecture)"
        echo "  macos-arm64     - Build for macOS ARM64 (Apple Silicon)"
        echo "  macos-x86_64    - Build for macOS x86_64 (Intel)"
        echo "  macos-universal - Build universal binary (ARM64 + x86_64)"
        echo "  linux           - Build for Linux (requires Docker)"
        echo "  windows         - Build for Windows (requires MinGW-w64)"
        echo "  all             - Build all platforms (default)"
        exit 1
        ;;
esac
