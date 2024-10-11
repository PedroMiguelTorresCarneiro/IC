# Specifies the minimum version of CMake that is required to build the project.
cmake_minimum_required(VERSION 3.10)

# Defines the name of the project.
project(TextFileProject)

# Sets the C++ standard version for the project to C++17.
# in order to use std::filesystem for listing the files in a directory.
set(CMAKE_CXX_STANDARD 17)

# Specifies the name of the output executable and the source files to be compiled.
# - `partI` name of executable file.
# - `partI.cpp` and `TextFileReader.cpp` are the source files that will be compiled and linked together.
# CMake will handle dependency tracking, building these files, and linking them to produce the final executable.
add_executable(TextDecoder partI.cpp TextFileReader.cpp)
