#!/bin/bash
# Script to compile and run the string to bin encoder

# Compile the string to bin
g++ -std=c++11 -o stb string_to_binary.cpp BitStream.cpp

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. "
    ./stb
else
    echo "Compilation failed."
fi
