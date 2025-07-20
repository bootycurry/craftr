# This script does a clean build in the build directory using MSYS2 MinGW UCRT environment

$ErrorActionPreference = "Stop" # Exit on any error

# Path to your MSYS2 MinGW UCRT programs
$env:MSYS2_ROOT = "C:\msys64"
$env:MINGW_BIN = "$env:MSYS2_ROOT\ucrt64\bin"

# Make sure required tools are found (cmake, gcc, pkg-config from MSYS2 MinGW UCRT)
$env:Path = "$env:MINGW_BIN;$env:Path"

# Check if cmake is from MSYS2 (not system or Visual Studio)
Write-Host "Using cmake from: $(Get-Command cmake).Path"
Write-Host "Using g++ from: $(Get-Command g++).Path"

# Set the build directory
$BUILD_DIR = "build"

# Create build directory if it doesn't exist
if (-Not (Test-Path -Path $BUILD_DIR -PathType Container)) {
    New-Item -ItemType Directory -Path $BUILD_DIR | Out-Null
}

Set-Location -Path $BUILD_DIR

# Clean previous build artifacts
try {
    cmake --build . --target clean
} catch {
    Write-Host "No previous build to clean."
}

# Generate build system
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build .

Write-Host "Build completed successfully with MSYS2 MinGW UCRT."
