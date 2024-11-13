#!/bin/bash
# Script to compile and run the string to bin encoder

# Compile the bin to string encoder
g++ -std=c++11 -o bts binary_to_string.cpp BitStream.cpp

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. "
    ./bts
else
    echo "Compilation failed."
fi
