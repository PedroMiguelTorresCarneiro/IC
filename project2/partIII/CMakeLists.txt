# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(AUDIOCODEC)

# Specify the C++ standard to use
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add an executable target for the project
add_executable(AudioCodec audioParser.cpp BitStream.cpp GolombCoding.cpp losslessEncoder.cpp predictModel.cpp losslessDecoder.cpp audioCodecMain.cpp )
