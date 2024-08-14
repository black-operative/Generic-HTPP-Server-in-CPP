#!/bin/bash

# Set the directories
INCLUDE_DIR=./Headers
SRC_DIR=./Source
BUILD_DIR=./Build

# Set the output executable name
OUTPUT=$BUILD_DIR/App

# Compile the source files
g++ -std=c++20 -I$INCLUDE_DIR -c $SRC_DIR/Server.cpp -o $BUILD_DIR/Server.o
g++ -std=c++20 -I$INCLUDE_DIR -c $SRC_DIR/Main.cpp -o $BUILD_DIR/Main.o

# Link the object files into an executable
g++ -std=c++20 $BUILD_DIR/Server.o $BUILD_DIR/Main.o -o $OUTPUT

# Print a success message
echo "Build successful. Executable created: $OUTPUT"
