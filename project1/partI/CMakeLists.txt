# Specifies the minimum version of CMake that is required to build the project.
cmake_minimum_required(VERSION 3.10)

# Defines the name of the project.
project(TextFileProject)

# Sets the C++ standard version for the project to C++23.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost.Locale component
find_package(Boost COMPONENTS locale REQUIRED)

# Specifies the name of the output executable and the source files to be compiled.
add_executable(TextDecoder partI.cpp TextFileReader.cpp)

# Link Boost.Locale to the executable
target_link_libraries(TextDecoder Boost::locale)
