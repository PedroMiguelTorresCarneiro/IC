cmake_minimum_required(VERSION 3.10)
project(ImageDecoderProject)

set(CMAKE_CXX_STANDARD 17)

# Check if std::filesystem requires linking with stdc++fs
if (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_library(STD_FS_LIBRARY stdc++fs)
    if (STD_FS_LIBRARY)
        message(STATUS "Linking with stdc++fs")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lstdc++fs")
    endif()
endif()

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add the executable
add_executable(ImageDecoder partIII.cpp ImageDecoder.cpp)

# Link OpenCV libraries
target_link_libraries(ImageDecoder ${OpenCV_LIBS})
