<!-- 
This is the README file for Part IV of Project 2 in the IC course. 
-->
# Project 2 - Part IV


### Example Header Design
The header could have the following format:
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
1. We convert all images to a greyscale image (1 channel)
    - reducing from 3channels to only 1channel
2. Use a Median Edge Predicition
    - In the single channel, it works like the simple neighboring exploration to predict
    - keep the difference between the original and the predited pixel
    - store it as a residuals matrix
3. Use golomb encoding:
    - on the residuals matrix apply the golomb coding

### B. DECODING
1. Apply the Golomb decoding
    - after this we got the residuals matrix
2. Use Media Edge Prediction
    - Do the same as in encoding
    - use the residuals matrix and predict the pixel,
    - getting the original (greyscale image)
3. Convert grey image to color
    - after getting the image, convert in color usign the same ratio 


--- 

- 1st attempt : we try to convert the 3 channels (green- blue, green, green-red) and we do not see any differences, we actually got less compression
- 2nd attempt : we try to add a DELTA ENCODING , but we not achieve greater differences in compression!
- 3rd attempt: we try transform the values of the residuals into positive values in order to be more easy to encode using Golomb but we do not achieve any better compression ratio, and the complexity increase
- 4 th attempt : use Huffman encoding!
