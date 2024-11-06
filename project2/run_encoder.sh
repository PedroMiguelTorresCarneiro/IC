#!/bin/bash
# Script to compile and run the encoder

# Compile the encoder
g++ -std=c++11 -o encoder encoder.cpp BitStream.cpp

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the encoder..."
    ./encoder
else
    echo "Compilation failed."
fi
