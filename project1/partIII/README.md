# IC - Information and Coding (2024/25)
---
# PROJECT 1 : Part III

### **1º** OpenCv instalation

```md
# macOs:
    brew install opencv

# linux:
    sudo apt-get install libopencv-dev

# win:
    download from the website
```

### **2º** SetUp enviroment
In order to be more easy to collaborating with others, as CMake will handle the build configuration across differents enviroments

- 2.1 : **Create a *CMakeLists.txt***
    ```md
        - find the OpenCv library installed on system;
        - pointing automatically to the correct include;
        - include all the OpenCv libraries

    
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


---
---
# NOTES:

### **BGR (Blue,Green, Red)** :
when you load an image using *OpenCV’s* ***cv::imread()***, the pixel data is stored in **BGR** order, with the Blue component first, followed by Green and then Red.
```md
    - pixel[0] = Blue
    - pixel[1] = Green
    - pixel[2] = Red
```
---

### **RGB Channels and Grayscale Appearance**
Each channel contains intensity values for thar specific color (0 to 255).
When displayed alone these single-channel matrices as an image, it appears **grayscale** because on a intensity scale:
```md
    - 0     : means no intensity (BLACK) 
    - 255   : full intensity (WHITE)
```
---
    
### **OpenCv Displays RGB Images**:
To display an RGB image, *OpenCV* combines the 3 channels into a color image.
```md
    - The image appears in colors because the viewer assigns each channel to the specific color 
    and use the scale to control the intensity of each color(Red,Green,Blue)
```
---

### **Grayscale Convertion**
Is combining the 3 channels into a single channel representing the **light intensity** of each pixel.
The problem facing on this is how much each channel weights during the fusion of the channels, for this we have the following formula:
$$
GraysFormula = 0.299 * Red + 0.587 * Green + 0.114 * Blue
$$

The weights are determined by perceptual factors that ensure the grayscale image looks natural to the human eye.
- Table with pixel channels ( **RGB | Grayscale**)
```md
    - Pixel (0,0): R: 95  , G: 90  , B: 39   | Gray: 85  
    - Pixel (0,1): R: 98  , G: 90  , B: 41   | Gray: 86  
    - Pixel (0,2): R: 99  , G: 90  , B: 42   | Gray: 87  
    - Pixel (0,3): R: 105 , G: 94  , B: 45   | Gray: 91  
    - Pixel (0,4): R: 108 , G: 96  , B: 46   | Gray: 93  
    - Pixel (1,0): R: 116 , G: 103 , B: 52   | Gray: 101 
    - Pixel (1,1): R: 119 , G: 105 , B: 56   | Gray: 103 
    - Pixel (1,2): R: 120 , G: 106 , B: 57   | Gray: 104 
    - Pixel (1,3): R: 122 , G: 105 , B: 57   | Gray: 104 
    - Pixel (1,4): R: 124 , G: 106 , B: 56   | Gray: 105 
    - Pixel (2,0): R: 121 , G: 103 , B: 49   | Gray: 102 
    - Pixel (2,1): R: 121 , G: 102 , B: 51   | Gray: 101 
    - Pixel (2,2): R: 120 , G: 101 , B: 50   | Gray: 100 
    - Pixel (2,3): R: 125 , G: 106 , B: 54   | Gray: 105 
    - Pixel (2,4): R: 124 , G: 106 , B: 51   | Gray: 105 
    - Pixel (3,0): R: 118 , G: 99  , B: 45   | Gray: 98  
    - Pixel (3,1): R: 120 , G: 102 , B: 47   | Gray: 101 
    - Pixel (3,2): R: 121 , G: 103 , B: 49   | Gray: 102 
    - Pixel (3,3): R: 124 , G: 103 , B: 50   | Gray: 103
```
---

### **Histogram Grayscale**
For the purpose of printing on the terminal the histogram have the height between [0,  32] (255/8) and for best fit on terminal only print each 8th intensity value.

```md 
MONARCH.PPM                                                                                                                         
                                                    *                                                                          
                                                    *                                                                          
                                                    *                                                                          
                                                    *                                                                          
                            *                       *                                                                          
                            *                       *                                                                          
                            *                       *   *                                                                      
                            *       *               *   *                                                                      
                            *       *   *           *   *   *                                                                  
                            *       *   *       *   *   *   *                                                                  
                            *       *   *       *   *   *   *                                                                  
                            *       *   *       *   *   *   *                                                                  
                            *   *   *   *   *   *   *   *   *                                                                  
                            *   *   *   *   *   *   *   *   *                                                                  
                            *   *   *   *   *   *   *   *   *                                                                  
                            *   *   *   *   *   *   *   *   *                                                                  
                        *   *   *   *   *   *   *   *   *   *                                                                  
                        *   *   *   *   *   *   *   *   *   *                                                                  
                        *   *   *   *   *   *   *   *   *   *                                                                  
                        *   *   *   *   *   *   *   *   *   *   *                                                              
                        *   *   *   *   *   *   *   *   *   *   *                                                              
                        *   *   *   *   *   *   *   *   *   *   *   *                                                          
                        *   *   *   *   *   *   *   *   *   *   *   *           *               *   *                          
                        *   *   *   *   *   *   *   *   *   *   *   *   *       *   *           *   *                          
                    *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *                          
                    *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *           *              
                *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *              
0   8  16  24  32  40  48  56  64  72  80  88  96 104 112 120 128 136 144 152 160 168 176 184 192 200 208 216 224 232 240 248 
```
**High Concentration in Midtones(120-180)**: This suggests that much of the image is composed of midtones (not too dark or too bright), giving it a balanced contrast. Not having neither much withes neither blacks !

    
```md 
AIRPLANE.PPM                                                                                                                         
                                                                                                        *                      
                                                                                                        *                      
                                                                                                        *                      
                                                                                                        *                      
                                                                                                        *                      
                                                                                                        *                      
                                                                                                    *   *                      
                                                                                                    *   *                      
                                                                                                    *   *                      
                                                                                                    *   *                      
                                                                                                    *   *   *                  
                                                                                                    *   *   *                  
                                                                                                    *   *   *                  
                                                                                                    *   *   *                  
                                                                                                    *   *   *                  
                                                                                                    *   *   *                  
                                                                                                    *   *   *                  
                                                                                                    *   *   *                  
                                                                                                *   *   *   *                  
                                                                                                *   *   *   *                  
                                                                                                *   *   *   *                  
                                                                                                *   *   *   *                  
                                                                                                *   *   *   *                  
                                                                                                *   *   *   *                  
                                                                                            *   *   *   *   *                  
                                                *   *   *   *                               *   *   *   *   *                  
                                            *   *   *   *   *   *               *   *   *   *   *   *   *   *                  
                                    *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *                  
0   8  16  24  32  40  48  56  64  72  80  88  96 104 112 120 128 136 144 152 160 168 176 184 192 200 208 216 224 232 240 248 
```
**High Concentration in the Highlights(200-216)**: This means the image is highly dominated by highlights(whites), with many bright regions. What is understandable because *airplane.ppm* is a image of a plane above snow montains.

---

###  **Gaussian blur filter** 
Tipically used to reduce image **noise** and reduce **detail**.
    
A Gaussian blur filter uses a convoltion matrix (kernel) to smooth the image by averaging pixel values with their neighbors (usging a Gaussian function), for each of the 3 channels(RGB):
- **Gaussian kernel**: created based on a kernel size and sigma;
- **Convoltion Operation**: of the image with the gaussian kernel for each channel

***Convoltion**: is a mathematical operation on two functions , resultin in a third function:*

$$
(f * g)(t) = \int_{-\infty}^{\infty} f(\tau)g(t - \tau) \, d\tau \equiv (f * g)(n) = \sum_{m=-\infty}^{\infty} f(m)g(n - m)
$$


**How does kernel size and sigma affect the bluer effect?**
1. **Sigma (σ) - Controls How Much Detail is Lost**: Sigma defines how much influence the neighboring pixels have when averaging a pixel's value.
    - <u>Smaller sigma values</u>: will result in a subtle blur where only nearby pixels contribute significantly to the final value, preserving more details.
    - <u>Larger sigma values</u>: will cause more pixels to contribute to the blur, making the result smoother and less detailed.
```md    
    - The larger the sigma, the more blurred the image becomes(more details are lost).
```
2. **Kernel Size - Defines the Area Over Which the Blur is Applied**: Kernel size determines how large an area around each pixel is considered when applying the blur. 
    - <u>A small kernel size</u>: only considers the nearest neighbors, so the **blur is localized** and affects only a small area.
    - <u>A larger kernel size</u>: means the algorithm will look at a wider area of the image, blurring a larger region around each pixel.
```md
    The larger the kernel size, the wider the area of the image that gets blurred, 
    causing the blur effect to spread over a larger region.
```

**CONCLUSION**
- **SIGMA**: Dictates how much smoothing is done (higher sigma means more smoothing and more detail loss).
- **KERNEL**: Defines the region of influence (larger kernel means the blur affects a wider area around each pixel).

---
