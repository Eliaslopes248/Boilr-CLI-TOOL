#!/bin/bash

# Cross-Platform Build Script for macOS, Linux, and Windows
# This script builds binaries for all three platforms from macOS

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    print_error "CMake is not installed. Please install it with: brew install cmake"
    exit 1
fi

# Get the script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

print_status "Starting cross-platform builds from macOS..."

# ============================================================================
# 1. macOS NATIVE BUILD (Current Architecture)
# ============================================================================
print_status "Building for macOS (native architecture)..."
if [ -d "build-mac-native" ]; then
    rm -rf build-mac-native
fi

cmake -B build-mac-native -DCMAKE_BUILD_TYPE=Release
cmake --build build-mac-native --config Release

if [ -f "build-mac-native/br" ]; then
    print_success "macOS native build completed: build-mac-native/br"
    file build-mac-native/br
else
    print_error "macOS native build failed!"
fi

# ============================================================================
# 2. macOS ARM64 BUILD (Apple Silicon)
# ============================================================================
print_status "Building for macOS ARM64 (Apple Silicon)..."
if [ -d "build-mac-arm64" ]; then
    rm -rf build-mac-arm64
fi

cmake -B build-mac-arm64 \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES=arm64

cmake --build build-mac-arm64 --config Release

if [ -f "build-mac-arm64/br" ]; then
    print_success "macOS ARM64 build completed: build-mac-arm64/br"
    file build-mac-arm64/br
else
    print_error "macOS ARM64 build failed!"
fi

# ============================================================================
# 3. macOS x86_64 BUILD (Intel)
# ============================================================================
print_status "Building for macOS x86_64 (Intel)..."
if [ -d "build-mac-x86_64" ]; then
    rm -rf build-mac-x86_64
fi

cmake -B build-mac-x86_64 \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES=x86_64

cmake --build build-mac-x86_64 --config Release

if [ -f "build-mac-x86_64/br" ]; then
    print_success "macOS x86_64 build completed: build-mac-x86_64/br"
    file build-mac-x86_64/br
else
    print_error "macOS x86_64 build failed!"
fi

# ============================================================================
# 4. macOS UNIVERSAL BINARY (Both architectures)
# ============================================================================
print_status "Building for macOS Universal Binary (ARM64 + x86_64)..."
if [ -d "build-mac-universal" ]; then
    rm -rf build-mac-universal
fi

cmake -B build-mac-universal \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"

cmake --build build-mac-universal --config Release

if [ -f "build-mac-universal/br" ]; then
    print_success "macOS Universal Binary build completed: build-mac-universal/br"
    file build-mac-universal/br
else
    print_error "macOS Universal Binary build failed!"
fi

# ============================================================================
# 5. LINUX BUILD (Using Docker)
# ============================================================================
print_status "Building for Linux (using Docker)..."
if [ -d "build-linux" ]; then
    rm -rf build-linux
fi

# Check if Docker is installed
if ! command -v docker &> /dev/null; then
    print_warning "Docker is not installed. Skipping Linux build."
    print_warning "Install Docker Desktop from https://www.docker.com/products/docker-desktop"
else
    # Use Docker to build Linux binary
    docker run --rm -v "$SCRIPT_DIR:/workspace" -w /workspace \
        gcc:latest bash -c "
        apt-get update -qq && \
        apt-get install -y -qq cmake && \
        cmake -B build-linux -DCMAKE_BUILD_TYPE=Release && \
        cmake --build build-linux --config Release
    "
    
    if [ -f "build-linux/br" ]; then
        print_success "Linux build completed: build-linux/br"
        file build-linux/br
    else
        print_error "Linux build failed!"
    fi
fi

# ============================================================================
# 6. WINDOWS BUILD (Using MinGW-w64)
# ============================================================================
print_status "Building for Windows (using MinGW-w64)..."
if [ -d "build-windows" ]; then
    rm -rf build-windows
fi

# Check if MinGW-w64 is installed
if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    print_warning "MinGW-w64 is not installed. Skipping Windows build."
    print_warning "Install with: brew install mingw-w64"
else
    cmake -B build-windows \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
        -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
        -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres
    
    cmake --build build-windows --config Release
    
    if [ -f "build-windows/br.exe" ]; then
        print_success "Windows build completed: build-windows/br.exe"
        file build-windows/br.exe
    else
        print_error "Windows build failed!"
    fi
fi

# ============================================================================
# SUMMARY
# ============================================================================
echo ""
print_status "Build Summary:"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

if [ -f "build-mac-native/br" ]; then
    print_success "✓ macOS Native: build-mac-native/br"
fi

if [ -f "build-mac-arm64/br" ]; then
    print_success "✓ macOS ARM64: build-mac-arm64/br"
fi

if [ -f "build-mac-x86_64/br" ]; then
    print_success "✓ macOS x86_64: build-mac-x86_64/br"
fi

if [ -f "build-mac-universal/br" ]; then
    print_success "✓ macOS Universal: build-mac-universal/br"
fi

if [ -f "build-linux/br" ]; then
    print_success "✓ Linux: build-linux/br"
fi

if [ -f "build-windows/br.exe" ]; then
    print_success "✓ Windows: build-windows/br.exe"
fi

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
print_status "All builds completed!"
