# Specifies the minimum version of CMake that is required to build the project.
cmake_minimum_required(VERSION 3.10)

# Defines the name of the project.
project(TextFileProject)

# Sets the C++ standard version for the project to C++23.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use pkg-config to find ICU where supported
find_package(PkgConfig)

# Try to find ICU using pkg-config
if(PKG_CONFIG_FOUND)
    pkg_check_modules(ICU REQUIRED icu-i18n icu-uc)
else()
    message(STATUS "pkg-config not found, falling back to default ICU detection.")
endif()

# Fallback: Use CMake's built-in FindICU module if pkg-config is not available
if(NOT ICU_FOUND)
    find_package(ICU REQUIRED COMPONENTS i18n uc)
endif()

# Check if ICU was found and provide feedback
if(ICU_FOUND)
    message(STATUS "ICU found: ${ICU_VERSION}")
    include_directories(${ICU_INCLUDE_DIRS})
    link_directories(${ICU_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "ICU not found. Please install ICU and try again.")
endif()

# Specifies the name of the output executable and the source files to be compiled.
add_executable(TextDecoder partI.cpp TextFileReader.cpp)

# Link ICU libraries to the executable
target_link_libraries(TextDecoder ${ICU_LIBRARIES})
