# Cross-Platform Compilation Guide

**Important**: You cannot build a single binary that runs on all platforms. Each platform requires its own binary format:
- **macOS**: Mach-O binary
- **Linux**: ELF binary  
- **Windows**: PE binary (.exe)

However, the **source code is cross-platform compatible**, meaning you can:
1. Build separate binaries for each platform (one per platform)
2. Cross-compile from one platform to create binaries for others

This guide explains how to build the `br` binary for different platforms.

## Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler
- Platform-specific build tools

## Building for Different Platforms

### macOS (Current Platform)

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

The binary will be created as `build/br`.

### Linux

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

The binary will be created as `build/br`.

### Windows

**Important**: Windows has two environments:
- **Windows Command Prompt/PowerShell**: Needs `br.exe` (Windows binary)
- **WSL (Windows Subsystem for Linux)**: Needs `br` (Linux binary, same as Linux)

#### Building Windows Binary (.exe)

For Windows Command Prompt/PowerShell:

**Using Visual Studio:**
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

The binary will be created as `build/Release/br.exe`.

**Using MinGW:**
```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

The binary will be created as `build/br.exe`.

#### Building Linux Binary for WSL

If you're using WSL (Windows Subsystem for Linux), you need the **Linux binary**, not the Windows .exe:

**Option 1: Build directly in WSL**
```bash
# In WSL Ubuntu terminal
mkdir build && cd build
cmake ..
cmake --build .
```
The binary will be created as `build/br` (Linux ELF format).

**Option 2: Cross-compile Linux binary from Windows**
See the "Cross-Compilation" section below for Docker-based cross-compilation.

## Understanding Cross-Platform Compatibility

**Key Point**: You need **separate binaries** for each platform. One binary cannot run on all platforms.

**What "cross-platform" means here**:
- ✅ The **source code** works on all platforms
- ✅ You can **build** on any platform
- ✅ You can **cross-compile** from one platform to create binaries for others
- ❌ One binary does **NOT** run on all platforms

## Cross-Compilation

Cross-compilation lets you build binaries for other platforms from your current machine:

### Building for Linux from macOS/Windows

You can use Docker to cross-compile:

```bash
docker run --rm -v $(pwd):/workspace -w /workspace \
  gcc:latest bash -c "mkdir build && cd build && cmake .. && cmake --build ."
```

### Building for Windows from Linux/macOS

Use MinGW-w64:

```bash
# Install MinGW-w64 (macOS)
brew install mingw-w64

# Build
mkdir build && cd build
cmake .. -DCMAKE_SYSTEM_NAME=Windows \
         -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
         -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++
cmake --build .
```

### Building for macOS from Linux/Windows

Use osxcross:

```bash
# Install osxcross (Linux)
# Follow instructions at: https://github.com/tpoechtrager/osxcross

mkdir build && cd build
cmake .. -DCMAKE_SYSTEM_NAME=Darwin \
         -DCMAKE_C_COMPILER=o64-clang \
         -DCMAKE_CXX_COMPILER=o64-clang++
cmake --build .
```

## Platform-Specific Notes

### Windows

- Windows 10+ includes `tar` command for extracting ZIP files
- ANSI color codes are enabled automatically via Windows API
- Binary will have `.exe` extension

### macOS

- Uses standard Unix `unzip` command
- ANSI color codes work natively in Terminal.app

### Linux

- Uses standard Unix `unzip` command
- ANSI color codes work natively in most terminals

## Building Multiple Platform Binaries

To distribute your tool, you'll need to build **three separate binaries**:

1. **macOS binary**: Build on macOS → `br` (Mach-O format)
2. **Linux binary**: Build on Linux or cross-compile → `br` (ELF format)  
3. **Windows binary**: Build on Windows or cross-compile → `br.exe` (PE format)

Each binary will only work on its target platform.

## Testing Cross-Platform Compatibility

After building, test the binary on its target platform:

```bash
# Test help
./br --help

# Test registry listing
./br -pr

# Test template creation
./br -I 1 -N test-project -D ./test-output
```

## Troubleshooting

### Windows: Colors not showing

If colors don't appear on Windows, ensure you're using:
- Windows 10 version 1511 or later
- Windows Terminal, PowerShell, or Command Prompt with ANSI support enabled

### Missing unzip/tar command

- **Windows**: Ensure you're on Windows 10+ or install 7-Zip
- **macOS/Linux**: Install via package manager:
  - macOS: `brew install unzip`
  - Ubuntu/Debian: `sudo apt-get install unzip`
  - Fedora: `sudo dnf install unzip`

### CMake version too old

Update CMake:
- macOS: `brew upgrade cmake`
- Linux: Download from https://cmake.org/download/
- Windows: Download installer from https://cmake.org/download/
