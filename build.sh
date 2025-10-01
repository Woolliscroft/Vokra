#!/bin/bash
# Build script for Vokra 2D Renderer with GLFW, GLAD, OpenGL, and GLM

# Output binary
OUT=build

# Source files
SRC="main.c src/glad.c"

# Include paths
INCLUDES="-Iinclude -I/usr/include"

# Compiler flags
CFLAGS="-Wall -Wextra -O2 -g $INCLUDES"

# Linker flags
LDFLAGS="-lglfw -lGL -lm -ldl -pthread"

# Compile
gcc $CFLAGS $SRC -o $OUT $LDFLAGS

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "Build succeeded. Run ./$OUT to start."
else
    echo "Build failed."
fi

