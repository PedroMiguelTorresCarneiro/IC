<!-- 
This is the README file for Part IV of Project 2 in the IC course. 
-->
# Project 2 - Part IV

## T1 - Lossless image coding using predictive coding

### OPTION SELECTION:

```java
COMMAND :   "-ImgCoding" arg  
        ;

arg     :    "-load" IMAGE ("-SignMag"|"-PosNeg") ("-m" NUM)* 
        ;

IMAGE   :  path_to_image
        ; 

NUM     :    [0 9]+;
```
1. **COMMAND:**
A command can be:
    - `-ImgCoding` ***arg*** --> Command to load the Image Coding algorithm

2. **arg:**
    - `-load` ***IMAGE*** `-SignMag | -PosNeg`--> load the *IMAGE* and choose the type of encoding for **Golomb coding**:
        - `-SignMag` ***(Sign and magnitude)***: encode the sign separately from the magnitude
        - `-PosNeg` ***(Positive/negative interleaving)***: Use a zigzag or odd-even mapping to interleave positive and negative
values, so all numbers map to non-negative integers.
    - <u>***Optionally***</u>:
        - `-m` ***NUM*** --> The **Golomb coding scheme** requires a parameter `m`, which controls the encoding length.
            - If ***provided***, the encoding uses this fixed `m` value.
            - If ***not provided***, the algorithm calculates an optimal `m` (**as expalined before**) by:
                1. Calculating the mean absolute residual value.
                2. Setting \( m = 1 \) initially.
                3. Doubling \( m \) until \( m \geq \text{mean absolute residual} \).

3. **IMAGE:**
    - ***Path*** where the image is stored.

4. **NUM:**
    - ***Integer*** value.

### COMMAND EXAMPLES:

1. **Encode `image1` using a Sign and magnitude approuch with optimal value for m**
```bash
./ImageCoding -ImgCoding -load "boat.ppm" -SignMag
```
2. **Encode `image1` using a Sign and magnitude approuch with `m` = 6**
```bash
./ImageCoding -ImgCoding -load "boat.ppm" -SignMag -m 6
```
3. **Encode `image1` using a Positive/negative interleaving approuch with optimal value for m**
```bash
./ImageCoding -ImgCoding -load "boat.ppm" -PosNeg
```
4. **Encode `image1` using a Positive/negative interleaving approuch with `m` = 12**
```bash
./ImageCoding -ImgCoding -load "boat.ppm" -PosNeg -m 12
```

---
<br>

## Header Design
The header have the following format:
- **Image Type (1 byte)**: 0 for grayscale, 1 for color.
- **Golomb Coding Type (1 byte)**: 0 for SIGN_MAG, 1 for POS_NEG.
- **Optimal m (4 bytes)**: Stored as an integer.
- **Image Width (4 bytes)**: Stored as an integer.
- **Image Height (4 bytes)**: Stored as an integer.
- **Format String (Variable Length)**: The format string (e.g., "PPM"), terminated by the delimiter.
- **Delimiter (#)**: A single byte ('#') to mark the end of the header.


This results in a `15-byte header(fixed)` + `X(format string)` + `1-byte (delimiter)` before the encoded data.
- for a `.png` total size of header = 15 + 3 + 1 = 19 bytes;




### A. ENCODING
1. Load the image
    - extract necessary info to write the header
2. Use a Median Edge Predicition
    - it works like the simple neighboring exploration to predict the pixel value
3. Use a matrix of Residuals
    - keep the difference between the original and the predited pixel
4. Use golomb encoding:
    - on the residuals matrix apply the golomb coding

- Additionally we create 2 tests in order to see if we can achieve better compression ratios and if wanted we can try the examples to see how this encoding works:
    - `Huffman Encoding`
    - `Arithmetic Encoding` 

### B. DECODING
1. Apply the Golomb decoding
    - after this we got the residuals matrix
2. Use Media Edge Prediction to reconstruct the image
    - Do the same as in encoding
    - use the residuals matrix and predict the pixel,
    - getting the original

--- 



### CONCLUSIONS:
- 1st attempt : we try to convert the image to a graysclae in order to achieve greatter compression, it works, but after we cannot get the colors as initially we it says that the compression was not being lossless, as expected.
- 2nd attempt: we try transform the values of the residuals into positive values in order to be more easy to encode using Golomb but we do not achieve any better compression ratio, and the complexity increase

- `FINAL CONCLUSION` for this stage the we achieve not a bigger compression, but we got a lossless compression working.

<br>
<br>
<br>

---

## T2 - Intra-Frame Video Coding

### STEPS TO IMPLEMENT INTRA-FRAME VIDEO
1. Modify the imput handling for video files
    - use OpenCv's :
        - `cv::VideoCapture` to load the video file,
        - `cv::VideoCapture::read()` to extract frames and process them one by one
        - `cv::VideoWritter` to combine frames into a video
2. Encode Each frame independently
    - treat each frame as an image and apply existing image codec
3. The header must now have differences and must have for example the number of frames
    - here we can have various approuch like have info for each frame, like the `m` etc... or have a unique header!