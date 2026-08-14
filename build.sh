#!/usr/bin/bash

# Install requirements
sudo apt update && sudo apt install -y build-essential cmake libncursesw5-dev || exit 1

# Build application
mkdir build || exit 1

cmake -B build
cmake --build build

# Move application to root directory
mv build/FlappyBirdTerminal .
