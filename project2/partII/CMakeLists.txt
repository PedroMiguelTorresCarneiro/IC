cmake_minimum_required(VERSION 3.10)

# Project name
project(StringToBinaryConverter)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the encoder and decoder executables
add_executable(golongMain GolombMain.cpp GolombCoding.cpp BitStream.cpp)

