# IC - Information and Coding (2024/25)

---
# PROJECT 1 : Part III 
<p style="text-align: right;"><strong>Author:</strong> Pedro Carneiro</p>


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

### **Option selection**
To implement the command definition I adopt a grammar-like structure:
```java
COMMAND :   "-diff" arg "," arg  ("-display")* 
        |   "-mse" arg "," arg 
        |   "-psnr" arg "," arg
        |   arg ("-display")*
        |   "-hist" (arg | "-diff" arg "," arg )
        ;

arg     :    "-load" IMAGE ("-grayscale")* ("-gaussian" NUM NUM)* ("-quantization" NUM)* ("-channels")* ("-highPass")*
        ;

IMAGE   :  path_to_image
        ; 

NUM     :    [0 9]+;

```
1. **COMMAND:**
A command can be:
    - `-diff` *arg* *arg* --> Absolute difference between two images *[simple|altered]*, optionally followed by `-display`
    - `-mse` *arg* *arg* --> Calculate the **MSE** between two images *[simple|altered]*
    - `-psnr` *arg* *arg* --> Calculate the **PSNR** between two images *[simple|altered]*
    - `arg` --> *[simple|altered]*, optionally followed by `-display`
    - `-hist` *arg* --> Display the histogram of a single image *[simple|altered]*

2. **arg:**
    - `-load` *IMAGE* --> load the *IMAGE*
    - <u>Optionally</u>, following operations can be applied to the loaded image:
        - `-grayscale` -->  Grayscale conversion
        - `-gaussian` *NUM* *NUM* --> Apply Gaussian-blur <***kernel size***> <***sigma***> 
        - `-quantization` *NUM* --> Quantize <***nº levels***>.

3. **IMAGE:**
    - ***Path*** where the image is stored.

4. **NUM:**
    - ***Integer*** value.

## COMMAND EXAMPLES:

1. **Abs. differece between `image1` and `image1 -blured`**
```bash
./ImageDecoder -diff -load "../../../datasets/image/boat.ppm" , -load "../../../datasets/image/boat.ppm" -gaussian 5 1 -display
```
2. **Mean Squared Error between `image1 -highPass filter` and `image1 -quantized to 4bits`**
```bash
./ImageDecoder -mse -load "../../../datasets/image/girl.ppm" -highPass , -load "../../../datasets/image/girl.ppm" -quantization 4
```
3. **Peak Signal-to-Noise Ratio between `image1 -highPass filter` and `image1 -quantized to 4bits`**
```bash
./ImageDecoder -psnr -load "../../../datasets/image/girl.ppm" -highPass , -load "../../../datasets/image/girl.ppm" -quantization 4
```
4. **Display an `image -blured -quantized to 5bits`** 
```bash
./ImageDecoder -load "../../../datasets/image/boat.ppm" -grayscale -gaussian 5 1 -quantization 5 -display
```
5. **Histogram of an `image -grayscale -quantized to 5bits`**
```bash
./ImageDecoder -hist -load "../../../datasets/image/boat.ppm" -grayscale -quantization 5 
```
6. **Histogram of the matrix of absolute difference between `image1` and `image1 -highPass filter`**
```bash
./ImageDecoder -hist -diff -load "../../../datasets/image/boat.ppm" , -load "../../../datasets/image/boat.ppm" -highPass 
```

---

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

### Mathematical formula 

$$
G(x) = 0.299 * Red + 0.587 * Green + 0.114 * Blue
$$
The weights are determined by perceptual factors that ensure the grayscale image looks natural to the human eye.


### Visual demonstration
| **Color image** (boat.ppm) | **Grayscale image** (boat.ppm) |
|----------------------------|--------------------------------|
|  ![Color](../partIII/imgs/gs_color.png) | ![Grayscale](../partIII/imgs/gs_grayscale.png) | 

### Pixel values before `(RGB)` and after `(GRAYSCALE)` convertion
```md
    - Pixel (0,0): R: 101 , G: 0   , B: 0    | Gray: 30  
    - Pixel (0,1): R: 10  , G: 0   , B: 88   | Gray: 13  
    - Pixel (0,2): R: 19  , G: 0   , B: 78   | Gray: 14  
    - Pixel (0,3): R: 18  , G: 0   , B: 109  | Gray: 17  
    - Pixel (1,0): R: 123 , G: 0   , B: 0    | Gray: 36  
    - Pixel (1,1): R: 135 , G: 129 , B: 82   | Gray: 125 
    - Pixel (1,2): R: 131 , G: 126 , B: 86   | Gray: 122 
    - Pixel (1,3): R: 122 , G: 115 , B: 81   | Gray: 113 
    - Pixel (2,0): R: 126 , G: 0   , B: 0    | Gray: 37  
    - Pixel (2,1): R: 129 , G: 139 , B: 76   | Gray: 128 
    - Pixel (2,2): R: 139 , G: 136 , B: 78   | Gray: 130 
    - Pixel (2,3): R: 128 , G: 120 , B: 82   | Gray: 118 
    - Pixel (3,0): R: 123 , G: 0   , B: 0    | Gray: 36  
    - Pixel (3,1): R: 136 , G: 120 , B: 97   | Gray: 122 
    - Pixel (3,2): R: 134 , G: 140 , B: 93   | Gray: 132 
    - Pixel (3,3): R: 127 , G: 130 , B: 97   | Gray: 125 
```
Result of the following command:
> ./ImageDecoder -load "../../../datasets/image/boat.ppm" -channels 

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
    The larger the sigma, the more blurred the image becomes(more details are lost).
```
2. **Kernel Size - Defines the Area Over Which the Blur is Applied**: Kernel size determines how large an area around each pixel is considered when applying the blur. 
    ```md
    # KERNEL SIZES must be ODD NUMBERS , because they represent a matrix around one specific pixel, so if we choose even number i didnt have a central pixel.
    ```
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
### **MATRIX OF ABSOLUTE DIFFERENCE**
The matrix of absolute differences between two images is a pixel-by-pixel comparison that helps quantify how much two images differ from each other.
        
$$
D(i,j) = |A(i,j) - B(i,j)|
$$

    - (i,j) , position of the pixel
    - A(i,j) and B(i,j) , intensity values (or color channels)


Tipically used to see how much a image changed :
- after a compression, 
- temporal images, 
- user resgistration align images

<br>

Visual interpretation :
- <u>Brighter areas</u> : suggests **significant differences**
- <u>Darker areas</u> : suggests **similarities**

---
###  **MSE and PNSR** 
```md    
    - MSE   : Mean Squared Error
    - PSMR  : Peak Signal-to-Noise Ratio
```

1. **MSE quantify the difference between two images** - mesuring the *average* squared difference between corresponding pixels in two images:
    - <u>Lower MSE</u> =  **higher** similarity (less diffences)
    - <u>Higher MSE</u> = **lower** similarity (more differences)

        ### Mathematical formula of MSE

$$
\text{MSE} = \frac{\sum \text{s}}{\text{p} \times \text{c}}
$$
        
            - s , square difference
            - p , total nº of pixels
            - c , number of channels  
         
2. **PSNR (dB) quantify the quality of a reconstructed/compressed image** - mesuring the ratio between the maximum possible value and the noise ( difference between two images):
    - <u>Lower PSNR</u> = images have significant differences(more noise/distortions)
    - <u>Higher PSNR</u> = images are more similar(less noise/distortions)

        ### Mathematical formula of PSNR

$$
\text{PSNR} = 10 \times \log_{10} \left( \frac{255^2}{\text{MSE}} \right)
$$

<br>

**CONCLUSION**
- **MSE** and **PSNR** complement each other. 
- They are used together in image processing to quantify differences and the level of distortion in images

---
###  **QUANTIZATION** 
```md    
    - Reduction of bits used to represent an image, by reducing the number of distinct colors or intensity levels used to represent an image
```
This processe reduce the number of bits to represent each pixel, compressing the image. However, quantization inherently introduces some loss of information, which can affect image quality.

### **Mathematical Formula for Quantization**

$$
Q(x) = \text{round}\left( \frac{x}{\text{stepSize}} \right) \times \text{stepSize} \quad,\quad \text{stepSize} = \left( \frac{256}{2^{q}} \right)
$$

    - x , pixel value
    - q , quantization level(nº bits) 

In this formula we use the `stepSize` to distribute the quantized color on the hole gamma of colors.

- ***Grayscale Image***:
    - **Pixel representation**: 
        - Single intensity value [*0 to 256*] `8 bits`.
    - **Quantization process**: 
        - Reducing the number of intensity levels, mapped to original `8bits gamma` 
- ***RGB Image***:
    - **Pixel representation**: 
        - Three intensity values: **R** `8 bits`, **G** `8 bits`, **B** `8 bits`.
    - **Quantization process**: 
        - Reducing the intensity levels for each channel (R, G, B), mapped to original `8bits gamma`    
<br>

#### - <u>QUANTIZATION LEVELS</u>:
- Refers to the number of $bits$ to represent diferent intensity values ($2^{bits}$) that a pixel can take after quantization.

    - **1-bit quantization** => 2 intensity levels $(2^1 = 2)$.
    - **2-bit quantization** => 4 intensity levels $(2^2 = 4)$.
    - **8-bit quantization** => `ORIGINAL IMAGE` 256 intensity levels $(2^8 = 256)$.

    >- *Grayscale* : The number of different **gray shades**
    >- *RGB* : The number of **colors**

| Quantization level : 7 (boat.ppm & boat.ppm - grayscale) | Quantization level : 3 (boat.ppm & boat.ppm - grayscale)|
|----------------|----------------|
| ![Quantization 7](../partIII/imgs/qt7_hist.png) | ![Quantization 3](../partIII/imgs/qt3_hist.png) |
| ![Quantization 7](../partIII/imgs/qt7_gs_hist.png) | ![Quantization 3](../partIII/imgs/qt3_gs_hist.png) |

</p>

---

<br>

- **Effects of Quantization Levels on Image**:
    - **Detail loss:**
        - <u>**higher levels**</u>: Preserve more detail
        - <u>**lower levels**</u>: Significant detail loss *(2 bits mean only 4 levels, resulting in more uniform areas)*

        | Quantization level : 7 (boat.ppm) | Quantization level : 2 (boat.ppm) |
        |------------------------|------------------------|
        | ![Quantization levels 7](../partIII/imgs/qt7_display.png) | ![Quantization levels 2](../partIII/imgs/qt2_display.png) |

    - **Compression:**
        - <u>**higher levels**</u>: Weak compression
        - <u>**lower levels**</u>: Strong compression (fewer bits are used to represent each pixel)

    `As you can see above, compression and detail loss are inversely related`
<br>

### Comparing the original image with the quantized one using MSE and PSNR to evaluate the quality.
Comparing the original image with the quantized one using ***MSE*** and ***PSNR*** show us how much image quantization affects the visual quality of the image. 

| stats | Quantization Lvl = 2bits | Quantization Lvl = 5bits | Quantization Lvl = 7bits | 
|-------|---------|---------|---------|
| **MSE** | 1172.41 | 17.3985 | 0.495516 |
| **PSNR** | 17.44 dB | 35.7257 dB | 51.1802 dB |

- At a `quantization level of 2`, the <u>MSE is significantly high</u> (19680.2), indicating a substantial difference from the original image, with many pixels changing drastically. Correspondingly, the <u>PSNR is low</u> (5.19 dB), reflecting a notable loss of detail and image degradation. 
- As the `quantization levels increase (150 and 230)` the <u>MSE decreases</u>, indicating fewer differences between the quantized and original images. The <u>PSNR values rise</u>, showing a clearer and more accurate image with less noticeable degradation.

> This coherence between quantization levels and error metrics demonstrates that as the number of quantization levels increases, more details are preserved, and the image quality improves.

---