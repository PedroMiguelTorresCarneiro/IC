cmake_minimum_required(VERSION 3.10)

# Project name
project(LosslessImageCoding)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add the executable for the main file
add_executable(ImageCoding
    ImageCoding.cpp
    LosslessImage.cpp
    BitStream.cpp
    GolombCoding.cpp
    ImageDecoder.cpp
)

add_executable( HuffmanTest
    HuffmanTest.cpp
    HuffmanCoding.cpp
    ImageDecoder.cpp
)

add_executable( ArithmeticTest
    ArithmeticTest.cpp
    ArithmeticEncoding.cpp
)

# Link libraries
target_link_libraries(ImageCoding ${OpenCV_LIBS})
target_link_libraries(HuffmanTest ${OpenCV_LIBS})
target_link_libraries(ArithmeticTest ${OpenCV_LIBS})
