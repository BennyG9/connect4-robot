#!/bin/bash
# build.sh - compile Connect4AI module for Python

# Go to src folder
cd src || exit

# Make sure build directory exists
mkdir -p build
cd build || exit

# Compile the module
c++ -O1 -Wall -shared -std=c++17 -fPIC \
-I../ \
-I$(python3 -m pybind11 --includes) \
../Connect4AI.cpp ../bindings.cpp \
-o connect4ai$(python3-config --extension-suffix)

# Done
echo "Build complete!"
