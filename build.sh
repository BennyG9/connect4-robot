#!/bin/bash
# build.sh - Compile Connect4AI module for Python on Raspberry Pi

echo "Building..."

# Go to src folder
cd src || { echo "src folder not found"; exit 1; }

# Make sure build directory exists
mkdir -p build
cd build || { echo "Failed to enter build folder"; exit 1; }

# Compile the module
nice -n 15 taskset -c 0 c++ -O0 -Wall -shared -std=c++17 -fPIC \
-I../ \
-I/usr/include/python3.11 \
../Connect4AI.cpp ../bindings.cpp \
-o connect4ai$(python3-config --extension-suffix)

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Build complete!"
else
    echo "Build failed! Check errors above."
fi
