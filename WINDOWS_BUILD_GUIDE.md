# Building Thicc Bass for Windows

## Current Status

The plugin is **cross-platform compatible** but currently only built for macOS. To use on Windows, you need to build Windows binaries.

## Quick Summary

| Platform | Status | Plugin Formats |
|----------|--------|----------------|
| macOS | ✅ Built & Ready | AU, VST3, CLAP |
| Windows | ⚠️ Needs Building | VST3 only |
| Linux | ⚠️ Needs Building | VST3, CLAP |

## Option 1: Build on Windows PC (Easiest)

### Requirements
- Windows 10/11
- Visual Studio 2022 Community (free)
- CMake 3.25+
- Git

### Installation Steps

1. **Install Visual Studio 2022:**
   - Download from: https://visualstudio.microsoft.com/downloads/
   - Select "Desktop development with C++" workload
   - Install

2. **Install CMake:**
   - Download from: https://cmake.org/download/
   - Choose "Windows x64 Installer"
   - During install, select "Add CMake to PATH"

3. **Install Git:**
   - Download from: https://git-scm.com/download/win
   - Use default settings

### Build Instructions

Open Command Prompt or PowerShell:

```bash
# Clone the repository with submodules
git clone --recursive https://github.com/[your-username]/thicc-bass.git
cd thicc-bass

# Create build directory
mkdir build
cd build

# Configure with Visual Studio
cmake .. -G "Visual Studio 17 2022"

# Build Release version
cmake --build . --config Release

# Your VST3 will be at:
# build\ThiccBass_artefacts\Release\VST3\Thicc Bass.vst3
```

### Install the Plugin

Copy the built plugin to:
```
C:\Program Files\Common Files\VST3\
```

## Option 2: GitHub Actions (Automated)

I've created a GitHub Actions workflow that automatically builds for both macOS and Windows when you push code.

### Setup:

1. **Push your code to GitHub:**
   ```bash
   git add .
   git commit -m "Add GitHub Actions workflow"
   git push origin main
   ```

2. **GitHub will automatically build both versions!**

3. **Download the builds:**
   - Go to your repo on GitHub
   - Click "Actions" tab
   - Click on the latest workflow run
   - Download "thicc-bass-windows" and "thicc-bass-macos" artifacts

4. **Create a Release:**
   ```bash
   git tag v1.0-beta
   git push origin v1.0-beta
   ```
   GitHub will automatically create a release with both macOS and Windows versions!

## Option 3: Ask a Windows User to Build

Send them this repository and the build instructions above. They can build it and send you the VST3 file.

## Which DAWs Work on Windows?

| DAW | Works with VST3 |
|-----|-----------------|
| FL Studio | ✅ Yes |
| Ableton Live | ✅ Yes |
| Cubase | ✅ Yes |
| Studio One | ✅ Yes |
| Reaper | ✅ Yes |
| Bitwig Studio | ✅ Yes |
| Cakewalk | ✅ Yes |

**Note:** Audio Units (AU) don't exist on Windows - only VST3 is needed.

## Testing Without Windows

If you don't have a Windows PC, you can:

1. **Use GitHub Actions** (recommended) - automatically builds Windows version
2. **Virtual Machine** - Run Windows in VirtualBox/VMware
3. **Wine** - Run Windows apps on Mac (not recommended for audio)
4. **Ask testers** - Have Windows users build and test for you

## Troubleshooting Windows Builds

### Visual Studio not found
```
CMake Error: Could not find Visual Studio
```
Solution: Install Visual Studio 2022 with "Desktop development with C++" workload

### CMake version too old
```
CMake 3.25 or higher is required
```
Solution: Download latest CMake from cmake.org

### Submodules not found
```
Error: JUCE not found
```
Solution: Clone with `--recursive` flag or run:
```bash
git submodule update --init --recursive
```

## File Size Comparison

- macOS package: ~26 MB (AU + VST3 + CLAP)
- Windows package: ~8 MB (VST3 only)

## Recommendation

**For easy distribution to Windows users:** Use GitHub Actions (Option 2). This automatically builds for both platforms every time you update your code, and you can easily distribute both versions.
