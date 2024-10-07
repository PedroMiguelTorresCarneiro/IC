cmake_minimum_required(VERSION 3.10)
project(ImageDecoderProject)

set(CMAKE_CXX_STANDARD 17)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add the executable and link libraries
add_executable(ImageDecoder partIII.cpp ImageDecoder.cpp)
target_link_libraries(ImageDecoder ${OpenCV_LIBS})
