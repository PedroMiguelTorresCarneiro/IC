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
5. Additionally we create 2 tests in order to see if we can achieve better compression ratios:
    - `Huffman` and `Arithmetic` coding work best when the input data exhibits high statistical redundancy and predictable patterns. The images' data lacks these properties, the compression ratio were not improved:
    - `Huffman Encoding` : 
        - **Limited Adaptability** : Huffman coding does not adapt to varying probabilities across different parts of the image. If local regions of the image have distinct statistical properties, the compression wasn't optimal.
        - **Small Symbol Set** : If the residuals or pixel values have a uniform distribution or a small range of values, Huffman coding won't achieve significant compression.
    - `Arithmetic Encoding` :
        - **Ineffective Probability Models** : If the probability model used for arithmetic coding does not accurately reflect the actual distribution of the data, the coding efficiency will be reduced, and we cannot achieve a pprobability model suited for all differente images.
        - **High Overhead** : Arithmetic coding involves computational overhead and precision challenge. With use the encoding could be more than 5x the normal time of compression and the compression ratio were poorer.

### B. DECODING
1. Apply the Golomb decoding
    - after this we got the residuals matrix
2. Use Media Edge Prediction to reconstruct the image
    - Do the same as in encoding
    - use the residuals matrix and predict the pixel,
    - getting the original

--- 
### COMPRESSION RATIOS:

| Encoding Approuch |  m  | coded file size(KB) | original size(KB) | encoding ratio(%) |
|-------------------|-----|-----------------|---------------|----------------|
| ***Positive Negative*** | ***optimal*** |  ***563***  | ***786*** | **28** |
| Signal and Magnitude | optimal | 612 | 786 | 22 |
| Positive Negative | 6 | 650 | 786 | 17 |
| ***Signal and Magnitude*** | ***6*** | ***605*** | ***786*** | **23** |
| Positive Negative | 12 | 599 | 786 | 24 |
| Signal and Magnitude | 12 | 629 | 786 | 20 |

### ***Considerations***:
- As we can see the best *compression ratio* were achieved using the Positive/Negative approuch in Golomb coding and using the calculated optimal m value.
- Another thing we can see on this table is that the optimal value is working for the Positive/Negative approuch and not for Signal and Magnitude, as we can see that the best compression ration for Signal and Magnitude approuch is when we choose the m = 6 and not the optimal as previouslly expected.

<br>
<br>

---
## CONCLUSIONS:
- 1st attempt : we try to convert the image to a graysclae in order to achieve greatter compression, it works, but after we cannot get the colors as initially we it says that the compression was not being lossless, as expected.
- 2nd attempt: we try transform the values of the residuals into positive values in order to be more easy to encode using Golomb but we do not achieve any better compression ratio, and the complexity increase

- `FINAL CONCLUSION` in this task we not achieve a bigger compression ratio (***28% best***) even with all tentatives we have made, like using `Huffman, Àrithmetic`, treat the image as grayscale, etc ... but we stick with the principal objective make a ***LOSSLESS*** compression.


#### CURIOSITIES:
The following image is the representation of the residuals for :
```shell
./ImageCoding -ImgCoding -load "boat.ppm" -PosNeg
```
![optimal](../partIV/img/posneg_optimal.png) 

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