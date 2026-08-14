#!/usr/bin/bash

# Create build directory
if [ ! -e build ]; then
  mkdir build || exit 1
fi

# Build application
cmake -B build
cmake --build build

# Move application to root directory
mv build/FlappyBirdTerminal .
