cmake_minimum_required(VERSION 3.10)
project(SoundDProject)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Include SFML using FetchContent
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Add the executable for your audio test program
add_executable(SoundDecoder partII.cpp audioDecoder.cpp)

# Link the correct SFML libraries (sfml-audio for sound functionality)
target_link_libraries(SoundDecoder PRIVATE sfml-audio sfml-graphics sfml-window sfml-system)

# Specify C++17 standard
target_compile_features(SoundDecoder PRIVATE cxx_std_17)
