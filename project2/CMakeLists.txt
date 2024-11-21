cmake_minimum_required(VERSION 3.10)

# Project name
project(StringToBinaryConverter)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the encoder and decoder executables
#(encoder encoder.cpp BitStream.cpp)
add_executable(encoder1 encoder1.cpp BitStream.cpp)
add_executable(encoderMeaning encoderMeaning.cpp BitStream.cpp)
add_executable(encoderMeaning1 encoderMeaning1.cpp BitStream.cpp)
add_executable(decoder decoder.cpp BitStream.cpp)
add_executable(golongMain GolombMain.cpp GolombCoding.cpp BitStream.cpp)

