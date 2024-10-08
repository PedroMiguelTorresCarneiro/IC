# IC - Information and Coding (2024/25)
---
# PROJECT 1 : Part III

### **1º** OpenCv instalation

---
- macOs:
```
brew install opencv
```
---
- linux:
```
sudo apt-get install libopencv-dev
```
---
- win:
```
download from the website
```

---


### **2º** SetUp enviroment
In order to be more easy to collaborating with others, as CMake will handle the build configuration across differents enviroments

- 2.1 : **Create a *CMakeLists.txt***
    - find the OpenCv library installed on system;
    - pointing automatically to the correct include;
    - include all the OpenCv libraries

    ```txt
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
    ```

- 2.2 : **Build and Run**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ./ImageDecoder
    ```


### NOTES:

- **BGR (Blue,Green, Red)** :
    when you load an image using *OpenCV’s* ***cv::imread()***, the pixel data is stored in **BGR** order, with the Blue component first, followed by Green and then Red.
    ```md
        - pixel[0] = Blue
        - pixel[1] = Green
        - pixel[2] = Red
    ```
- **RGB Channels and Grayscale Appearance**:
    Each channel contains intensity values for thar specific color (0 to 255).
    When displayed alone these single-channel matrices as an image, it appears **grayscale** because on a intensity scale:
    ```md
        - 0     : means no intensity (BLACK) 
        - 255   : full intensity (WHITE)
    ```
    
- **OpenCv Displays RGB Images**:
    To display an RGB image, *OpenCV* combines the 3 channels into a color image.
    ```md
        - The image appears in colors because the viewer assigns each channel to the specific color 
        and use the scale to control the intensity of each color(Red,Green,Blue)
    ```

- **Grayscale Convertion**:
    Is combining the 3 channels into a single channel representing the **light intensity** of each pixel.
    The problem facing on this is how much each channel weights during the fusion of the channels, for this we have the following formula:
    ```math
    GraysFormula = 0.299 * Red + 0.587 * Green + 0.114 * Blue
    ```
    The weights are determined by perceptual factors that ensure the grayscale image looks natural to the human eye.
