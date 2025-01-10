<!-- 
This is the README file for Part IV of Project 2 in the IC course. 
-->
# IC - Information and Coding (2024/25)

Return to the main README : [Project 2 - README](../../project2/README.md)

<br>
<br>

---
# Part IV – Image and Video Coding with Predictive Coding

<br>
<br>

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
            - If ***not provided***, the algorithm calculates an optimal `m` (**as explained before**) by:
                1. Calculating the mean absolute residual value.
                2. Setting \( m = 1 \) initially.
                3. Doubling \( m \) until \( m \geq \text{mean absolute residual} \).

3. **IMAGE:**
    - ***Path*** where the image is stored.

4. **NUM:**
    - ***Integer*** value.

### COMMAND EXAMPLES:

1. **Encode `image1` using a Sign and magnitude approach with optimal value for m**
```bash
./ImageCoding -ImgCoding -load "boat.ppm" -SignMag
```
2. **Encode `image1` using a Sign and magnitude approach with `m` = 6**
```bash
./ImageCoding -ImgCoding -load "boat.ppm" -SignMag -m 6
```
3. **Encode `image1` using a Positive/negative interleaving approach with optimal value for m**
```bash
./ImageCoding -ImgCoding -load "boat.ppm" -PosNeg
```
4. **Encode `image1` using a Positive/negative interleaving approach with `m` = 12**
```bash
./ImageCoding -ImgCoding -load "boat.ppm" -PosNeg -m 12
```

---
<br>

## Header Design
The header has the following format:
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
5. Additionally we created 2 tests in order to see if we could achieve better compression ratios:

    - `Huffman` and `Arithmetic` coding work best when the input data exhibits high statistical redundancy and predictable patterns. The images' data lacks these properties, the compression ratio was not improved:

    - `Huffman Encoding` : 
        - **Limited Adaptability** : Huffman coding does not adapt to varying probabilities across different parts of the image. If local regions of the image have distinct statistical properties, the compression was not optimal.
        - **Small Symbol Set** : If the residuals or pixel values have a uniform distribution or a small range of values, Huffman coding will not achieve significant compression.

    - `Arithmetic Encoding` :
        - **Ineffective Probability Models** : If the probability model used for arithmetic coding does not accurately reflect the actual distribution of the data, the coding efficiency will be reduced, and we can not achieve a pprobability model suited for all differente images.
        - **High Overhead** : Arithmetic coding involves computational overhead and precision challenge. With its use, the encoding time can be more than 5 times longer than standard compression methods, and the resulting compression ratio may also be less efficient.

### B. DECODING
1. Apply the Golomb decoding
    - After this we obtained the residuals matrix
2. Use Media Edge Prediction to reconstruct the image
    - Do the same as in encoding
    - Use the residuals matrix and predict the pixel
    - Getting the original

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
- The best compression ratio was achieved using the Positive/Negative approach in Golomb coding, with the calculated optimal `m` value.
- Interestingly, the optimal `m` value appears to be effective only for the Positive/Negative approach. For the Signal and Magnitude approach, the best compression ratio was obtained when `m = 6`, contrary to the expected outcome using the calculated optimal value.

<br>
<br>

---
## CONCLUSIONS:
- 1st attempt : We tried to convert the image to grayscale to achieve greater compression. While this worked to some extent, it failed to retain the original colors, confirming that the compression was not lossless, as expected.
- 2nd attempt: we tried to transform the residual values into positive values to facilitate encoding with Golomb coding. However, this did not yield a better compression ratio and instead increased complexity.

- `FINAL CONCLUSION` In this task, we were unable to achieve a significantly higher compression ratio (***best result: 28%***), despite various attempts, including using `Huffman` and `Arithmetic` coding, treating the image as grayscale, and other strategies. However, we successfully adhered to the primary objective of achieving ***LOSSLESS*** compression.


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