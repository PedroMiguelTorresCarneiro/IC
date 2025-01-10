cmake_minimum_required(VERSION 3.10)

# Project name
project(StringToBinaryConverter)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# Add the encoder and decoder executables
add_executable(golongMain 
    GolombMain.cpp 
    GolombCoding.cpp 
    BitStream.cpp
)

