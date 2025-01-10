# IC - Information and Coding (2024/25)

Return to the main README : [Project 2 - README](../../project2/README.md)

<br>
<br>


# Part II – Golomb Coding

<br>
<br>

## T1 - Implementing the Golomb coding class

### Overview

The **Golomb Coding** class implements an efficient method for encoding and decoding integers based on the Golomb coding scheme. It leverages the **BitStream** class for precise bit-level manipulation, ensuring high efficiency. The implementation supports both positive and negative integers and offers flexibility through configurable encoding modes and the adjustable parameter `m`.

---

### Features

1. **Bit Efficiency**

    The class uses the `BitStream` class for bit-level operations, enabling precise control and efficient encoding/decoding processes. 

2. **Parameterization**

    The parameter `m` determines the length of the encoding. It can be adjusted dynamically, allowing the Golomb coding scheme to adapt to varying data distributions.

3. **Handling Negative Values**

    The class supports two methods for encoding negative values:
    - **Sign and Magnitude (SIGN_MAG):** Separates the sign from the magnitude during encoding.
    - **Positive/Negative Interleaving (POS_NEG):** Maps positive and negative integers into a single non-negative sequence using a zigzag mapping.

---

### Specifications

1. **Encoding**
  
    The `encode` function implements the following:
    - **Sign and Magnitude:**
        - The sign is encoded as a single bit.
        - The magnitude is divided into a quotient (encoded with unary coding) and a remainder (encoded with fixed-length binary).
    - **Positive/Negative Interleaving:**
        - Maps integers to non-negative values using the formula:
            - $ \text{mappedValue} = 2 \times \text{value} $ --> ***for positive values***
            - $ \text{mappedValue} = -2 \times \text{value} - 1 $ --> ***for negative values***
        - Encodes the quotient with unary coding and the remainder with binary coding.

2. **Decoding** :

    The `decode` function performs the reverse of encoding:
    - Reads the quotient using unary coding.
    - Reads the remainder as a fixed-length binary value.
    - Decodes the final value based on the encoding mode:
        - **Sign and Magnitude:** Combines the magnitude and sign to reconstruct the original value.
        - **Positive/Negative Interleaving:** Reverses the mapping to retrieve the original integer.
---

<br>
<br>

### Considerations:
- The GolombCoding class provides an adaptable and efficient mechanism for encoding and decoding integers. With support for both positive and negative values and configurable parameters, it is well-suited for various data distributions. Leveraging the BitStream class ensures precise bit-level manipulation, making it an essential component for applications requiring efficient compression and encoding.

---
<br>
<br>


## T2 - Testing the Golomb Encoder and Decoder

### OPTION SELECTION:

```java
COMMAND :   "-Golomb" arg  
        ;

arg     :    "-load" TXT ("-SignMag"|"-PosNeg") ("-m" NUM)* 
        ;

TXT     :  path_to_txt_file
        ; 

NUM     :    [0 9]+;
```
1. **COMMAND:**
A command can be:
    - `-Golomb` ***arg*** --> Command to load the Image Coding algorithm

2. **arg:**
    - `-load` ***TXT*** `-SignMag | -PosNeg`--> load the *IMAGE* and choose the type of encoding for **Golomb coding**:
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

3. **TXT:**
    - ***Path*** where the text file is stored.

4. **NUM:**
    - ***Integer*** value.

### COMMAND EXAMPLES:

1. **Encode `t1.txt` using a Sign and magnitude approuch with optimal value for m**
```bash
./golomgMain -Golomb -load "inputINT.txt" -SignMag
```
2. **Encode `t1.txt` using a Sign and magnitude approuch with `m` = 6**
```bash
./golomgMain -Golomb -load "inputINT.txt" -SignMag -m 6
```
3. **Encode `t1.txt` using a Positive/negative interleaving approuch with optimal value for m**
```bash
./golomgMain -Golomb -load "inputINT.txt" -PosNeg
```
4. **Encode `t1.txt` using a Positive/negative interleaving approuch with `m` = 12**
```bash
./golomgMain -Golomb -load "inputINT.txt" -PosNeg -m 12
```
---

### **Testing Workflow**

The testing program performs the following steps:

1. **Read Input:**
   - A text file containing integers (both positive and negative) is read.
   - Each integer is processed one at a time.

2. **Encoding:**
   - Each integer is encoded using the Golomb encoder, leveraging the `BitStream` class for efficient bit-level writing.
   - The encoding process uses a configurable Golomb coding parameter (`m`) and mode (`-SignMag` or `-PosNeg`).
   - Encoded bits are written to a binary file.

3. **Decoding:**
   - The binary file is read using the Golomb decoder.
   - Each encoded integer is decoded back to its original value.

4. **Validation:**
   - The decoded values are compared with the original integers from the input file.
   - Any mismatches between the original and decoded values are highlighted.

5. **Summary Output:**
   - A summary of the encoding and decoding results is printed to the terminal.
   - If all values are correctly decoded, the program confirms the implementation's accuracy.

---

### EXPECTED OUTPUT
- `./golongMain -Golomb -load "inputINT.txt" -PosNeg`

```shell
    Encoding values from inputINT.txt:
    Encoding value: 10
    Encoding value: 10
    Encoding value: 1
    Encoding value: 101
    Encoding value: -10
    Encoding value: -1
    Encoding value: -101
    Encoded data in encoded_golomb.dat (as binary):
    00101000 01010000 00010111 00010100 01001100 00001111 00010010 
    Decoded values:
    10
    10
    1
    101
    -10
    -1
    -101
```

| Command | output |
|---------|--------|
| `./golongMain -Golomb -load "inputINT.txt" -PosNeg` | 00101000 01010000 00010111 00010100 01001100 00001111 00010010 |
| `./golongMain -Golomb -load "inputINT.txt" -PosNeg -m 6` | 11100101 11001000 10111111 11111111 11111111 11111111 11101001 11000100 01111111 11111111 11111111 11111111 11100110  |
| `./golongMain -Golomb -load "inputINT.txt" -PosNeg -m 12` | 10100010 10000001 01111111 11111111 10101010 01110000 11111111 11111111 10100100   |

- As we can see in this case the method to calculate the `optimal m` worked well the output is much smaller than `m=6` or `m=12`.

---

### **Optimal `m` Calculation in Golomb Coding**

In Golomb coding, the parameter `m` plays a critical role in determining the efficiency of the encoding process. Choosing the right value for `m` is essential to achieve optimal compression, as it balances the trade-off between the unary and binary parts of the encoding.


#### ***How `m` is Calculated***

The calculation of the optimal `m` is based on the distribution of the absolute values of the data being encoded. Here's the step-by-step process:

1. **Input Data**
   - A vector of integers (both positive and negative) is provided for encoding.

2. **Sum of Absolute Values**
   - Calculate the sum of the absolute values of all integers in the vector.
   ```cpp
   for (int value : values) {
       sum += std::abs(value);
   }
3. **Mean Absolute Value**
    - Compute the mean of the absolute values by dividing the sum by the total number of integers.
    ```cpp
    double mean = sum / values.size();
    ```
4. **Determine m**
    - Start with 𝑚 = 1 and iteratively double it until 𝑚 ≥ mean.
    ```cpp
    int optimalM = 1;
    while (optimalM < mean) {
        optimalM *= 2;
    }
    ```

#### ***Why This Method?***
- ***Adaptation to Data Distribution***: Golomb coding performs optimally when the parameter m reflects the typical magnitude of the input data. The mean absolute value provides a good estimate of the "average" data size, ensuring the unary and binary components of the encoding are balanced.
- ***Efficiency in Encoding***: By doubling m iteratively, we align it with the nearest power of 2 greater than or equal to the mean. This alignment ensures that the binary representation of the remainder is compact, reducing the overall bit usage
- ***Generality***: This method does not assume a specific probability distribution, making it applicable to a wide range of input data sets
- ***Minimizing Overhead***: Using the mean as the basis for m minimizes the length of the unary part for most values, which contributes to better compression ratios.

<br>

### PRATICAL EXAMPLE:
Consider an input vector : `[10, -5, 20, -15]`
1. ***SUM of Absolute Values***
    - $sum = |10| + |-5| + |10| + |-15| = 50$
2. ***MEAN Absolute Values***
    - $mean = \frac{\text{sum}}{\text{cost}} = \frac{50}{4} = 12,5$
3. ***Optimal `m`*** : Start with $m = 1$ and $double$:
- $m = 1, 2, 4, 8, 16$ (stop at $m ≥ 12,5$ )
    - then : $m = 16$

By calculating `optimalM` in this manner, Golomb coding dynamically adapts to the characteristics of the input data, providing a robust and flexible compression mechanism.

#### ENCODING 

1. $m = 16$ and using Poitive/Negative interleaving: 

    | Value | Mapped Value | `q` | `r` | encoding |
    |-------|--------------|-----|-----|----------|
    | 10 | 20 | $\frac{20}{16} = 1$ | $20 \text{ mod } 16 = 4$ | 1000100 |
    | -5 | 9 | $\frac{9}{16} = 0$ | $9 \text{ mod } 16 = 9$ | 001001 |
    | 20 | 40 | $\frac{40}{16} = 2$ | $40 \text{ mod } 16 = 8$ | 11001000 |
    | -15 | 29 | $\frac{29}{16} = 1$ | $29 \text{ mod } 16 = 13$ | 1001101 |

2. $m = 6$ and using Poitive/Negative interleaving: 

    | Value | Mapped Value | `q` | `r` | encoding |
    |-------|--------------|-----|-----|----------|
    | 10 | 20 | $\frac{20}{6} = 3$ | $20 \text{ mod } 6 = 2$ | 11100010 |
    | -5 | 9 | $\frac{9}{6} = 1$ | $9 \text{ mod } 6 = 3$ | 100011 |
    | 20 | 40 | $\frac{40}{6} = 6$ | $40 \text{ mod } 6 = 4$ | 11111100100 |
    | -15 | 29 | $\frac{29}{6} = 4$ | $29 \text{ mod } 6 = 5$ | 111100101 |

### Comparison of Bit Lengths
For $m = 16$ :
- ***Quotients (q)***: are smaller, requiring fewer unary bits.
- ***Remainders (r)***: are encoded in fixed-length binary, but as 𝑚 increases, fewer bits are needed for remainders.

For $m = 6$ :
- ***Quotients (q)***: are larger due to a smaller 𝑚, resulting in longer unary encodings.
- ***Remainders (r)***: are encoded in fixed-length binary, which requires slightly fewer bits for each remainder, but the savings are negligible compared to the longer unary encoding.


### Conclusion
- $m = 16$ produces shorter encodings overall, as it balances the lengths of unary (quotient) and binary (remainder) encodings effectively.
- A smaller $m$, like $m = 6$, increases the length of the unary encoding disproportionately, leading to less efficient compression.
---