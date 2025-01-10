# IC - Information and Coding (2024/25)

--
# PROJECT 2


## BitStream


## Testing Bitstream

`In order to make the script executable : chmod +x <scriptName>.sh`

| Extra | Convertion | info |
|-------|------------|----------|
| `./stb` | String -> Binary | convert a String in their binary representation and store it in `input.txt` |
| `./bts` | Binary -> String | convert a Binary code from a file in their string representation showned in the **terminal** | 

<br>
<br>

1. **`Build and Run`**
    ```bash
        mkdir build
        cd build
        cmake ..
        make
    ```

## ENCODER
```bash
./encoder <input_string_file> <output_binary_file>
```
- The input.txt must be created inside `/build` or  by giving the path to the file
- In this case we have 3 files with different sizes already created:
    - *input.txt* = 197 bytes
    - *input2kB.txt* = 2 kilobytes
    - *input2MB.txt* = 2 Megabytes
---
<br>
<br>

We built 4 encoders with some differences, in order to compare them we calculated:
- `encoding time`
    - We ran each encoder 100 times and calculated the median value of the encoding time
- `compression ratio`
    - Using the formula, $ \left( 1 - \frac{Compressed_{size}}{Original_{size}}\right) \times 100$
        - $Original_{size}$ = size of the input.txt 
        - $Compressed_{size}$ = size of the output.bin  

- ### ENCODERS:

    `1st` --> **HEADER** the number of padding bits  ---| `encoder1.cpp`
    - While getting the number of padding bits, creates a <u>temporary file</u> writing the code
    - Writes the header and the rest the temporary file

    ---

    `2nd` --> **HEADER** the number of padding bits  ---| `encoder.cpp`
    - While getting the number of padding bits stores it in a <u>internal structure</u>
    - Writes the header and the rest the temporary file

    ---

    `3rd` --> **HEADER** the number of Meaningful bits  ---| `encoderMeaning.cpp`
    - While getting the number of padding bits stores it in a <u>internal structure</u>
    - Writes the header and the rest the temporary file

    ---

    `4th` --> **HEADER** the number of padding bits  ---| `encoderMeaning1.cpp`
    - While getting the number of padding bits, creates a <u>temporary file</u> writing the code
    - Writes the header and the rest the temporary file

    ---
<br>

The following table shows all the values:

| File | ENCODER | Header | step encoding | encoding time | compression ratio |
|------|---------|--------|---------------|---------------|-------------------|
| `197  bytes` | 1st | padding bytes | temporary file | $\bar{x}$ = 0.438383 miliseconds | 85,79% |
| | 2nd | padding bytes | internal structure |  $\bar{x}$ = 0.275198 miliseconds | `86,29 %` |
| | 3rd | meaningful bytes | internal structure | $\bar{x}$ = `0.243499 miliseconds` | 85,79 % |
| | 4th | meaningful bytes | temporary file | $\bar{x}$ = 0.587885 miliseconds | 85,28 % |
| | | | | | |
| `2 kBytes` | 1st | padding bytes | temporary file | $\bar{x}$ = 0.510059 miliseconds | 87,79 % |
| | 2nd | padding bytes | internal structure |  $\bar{x}$ = 0.455895 miliseconds | `87,84 %` |
| | 3rd | meaningful bytes | internal structure | $\bar{x}$ = `0.297953 miliseconds` | 87,74 % |
| | 4th | meaningful bytes | temporary file | $\bar{x}$ = 0.653767 miliseconds | 87,70 % |
| | | | | | |
| `2 MBytes` | 1st | padding bytes | temporary file | $\bar{x}$ = `55.480459 miliseconds` | `87,99 %` |
| | 2nd | padding bytes | internal structure |  $\bar{x}$ = 182,785280 miliseconds | `87,99 %` |
| | 3rd | meaningful bytes | internal structure | $\bar{x}$ = `59,839309 miliseconds` | `87,99 %` |
| | 4th | meaningful bytes | temporary file | $\bar{x}$ = 75,097813 miliseconds | `87,99 %`  |

<br>
<br>

As expected: 
- the encoders with best `compression ratio` are the ones who <u>encode the padding bytes</u> on the *HEADER*.
- the encoders with best `encoding time` are the ones who have <u>meaningful bytes encoded</u> on the *HEADER*, since they do not need most iterations to be calculated.

**SURPRISINGLY**:
- 

### Examples:

| ex | file | content | `Nbits % 8 = 0`| Padding bits | HEADER |
|---------|------|---------|----------------------|--------------|----------|
| 1 | input.txt | 01010(...)101111 | yes | - | - |
| 1 | output.bin | `0#PedroMiguelTorresCarneiro` | - | 0 | `0#` |
| 2 | input.txt | 01010(...)101 | No | - | - |
| 2 | output.bin | `3#PedroMiguelTorresCarneirh` | - | 3 | `3#` | 

<br>
<br>

## DECODER
```bash
./decoder <input_binary_file> <output_string_file>
```
---
<br>
<br>

1.`ATTEMPT` --> **HEADER + BODY** count
- calculate the bits of the header
- calculate total bits
- get the number of meaningful bits by subtracting the padding number and header size
- time of encoding:
    - ***FILE USED***: input.txt --> 197 bytes / 27 bytes
        - we ran encoder 100 times: 
            - $\bar{x} = 0,292412 \text{ miliseconds}$
    - ***FILE USED***: input.txt --> 2000 bytes /output.bin ---> 249 bytes
        - we ran encoder 100 times: 
            - $\bar{x} = 0.361967 \text{ miliseconds}$

