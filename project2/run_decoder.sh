#!/bin/bash
# Script to compile and run the decoder

# Compile the decoder
g++ -std=c++11 -o decoder decoder.cpp BitStream.cpp

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the decoder..."
    ./decoder
else
    echo "Compilation failed."
fi
