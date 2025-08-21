#!/bin/bash

# This script does a clean build in the build directory

set -e # Exit immediately if a command fails

BUILD_DIR=build

# Create build dir if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Clean previous build artifacts (object files, executables, etc.)
cmake --build . --target clean || echo "No previous build to clean."

# Generate build files
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build the project
cmake --build .

echo "Build completed successfully."
