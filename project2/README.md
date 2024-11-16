# IC - Information and Coding (2024/25)

--
# PROJECT 2


## BitStream


## Testing Bitstream

`In order to make script exevcutable : chmod +x <scriptName>.sh`

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
- You need to create the input.txt inside `/build` or giving the the path to the file
- in this case we have 3 files with different sizes already created:
    - *input.txt* = 197 bytes
    - *input2kB.txt* = 2 kilobytes
    - *input2MB.txt* = 2 Megabytes
---
<br>
<br>

We build 4 encoders with some differencesan in order to compare them we calculate:
- `encoding time`
    - we run 100 times each encoder and calculate the meadian value for the encoding time
- `compression ratio`
    - Using the formula, $ \left( 1 - \frac{Compressed_{size}}{Original_{size}}\right) \times 100$
        - $Original_{size}$ = size of the input.txt 
        - $Compressed_{size}$ = size of the output.bin  

- ### ENCODERS:

    1.`1st` --> **HEADER** the nº of padding bits  ---| `encoder1.cpp`
    - while getting the nº padding bits, create a <u>teporary file</u> writing the code
    - write the header and the rest the temporary file

    ---

    2.`2nd` --> **HEADER** the nº of padding bits  ---| `encoder.cpp`
    - while getting the nº padding bits create store it in a <u>internal structure</u>
    - write the header and the rest the temporary file

    ---

    3.`3rd` --> **HEADER** the nº of Meaningfull bits  ---| `encoderMeaning.cpp`
    - while getting the nº padding bits create store it in a <u>internal structure</u>
    - write the header and the rest the temporary file

    ---

    4.`4th` --> **HEADER** the nº of padding bits  ---| `encoderMeaning1.cpp`
    - while getting the nº padding bits, create a <u>teporary file</u> writing the code
    - write the header and the rest the temporary file

    ---
<br>

the following table shows all the values:

| File | ENCODER | Header | step encoding | encoding time | compression ratio |
|------|---------|--------|---------------|---------------|-------------------|
| `197  bytes` | 1st | padding bytes | temporary file | $\bar{x}$ = 0.438383 miliseconds | 85,79% |
| | 2nd | padding bytes | internal structure |  $\bar{x}$ = 0.275198 miliseconds | `86,29 %` |
| | 3rd | meaningfull bytes | internal structure | $\bar{x}$ = `0.243499 miliseconds` | 85,79 % |
| | 4th | meaningfull bytes | temporary file | $\bar{x}$ = 0.587885 miliseconds | 85,28 % |
| | | | | | |
| `2 kBytes` | 1st | padding bytes | temporary file | $\bar{x}$ = 0.510059 miliseconds | 87,79 % |
| | 2nd | padding bytes | internal structure |  $\bar{x}$ = 0.455895 miliseconds | `87,84 %` |
| | 3rd | meaningfull bytes | internal structure | $\bar{x}$ = `0.297953 miliseconds` | 87,74 % |
| | 4th | meaningfull bytes | temporary file | $\bar{x}$ = 0.653767 miliseconds | 87,70 % |
| | | | | | |
| `2 MBytes` | 1st | padding bytes | temporary file | $\bar{x}$ = `55.480459 miliseconds` | `87,99 %` |
| | 2nd | padding bytes | internal structure |  $\bar{x}$ = 182,785280 miliseconds | `87,99 %` |
| | 3rd | meaningfull bytes | internal structure | $\bar{x}$ = `59,839309 miliseconds` | `87,99 %` |
| | 4th | meaningfull bytes | temporary file | $\bar{x}$ = 75,097813 miliseconds | `87,99 %`  |

<br>
<br>

As expected: 
- the encoders with best `compression ratio` are the ones who <u>encode the padding bytes</u> on the *HEADER*.
- the encoders with best `encoding time` are the ones who have <u>meaningfull bytes encoded</u> on the *HEADER*, because they dont need must iterations to be calculated.

**SUPRISINGLY**:
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

1.`FIRST ATTEMPT` --> **HEADER + BODY** count
- calculate the bits of the header
- calculate totatl bits
- get the number of meaningfull bits subtracting the padding number and header size
- time of encoding:
    - ***FILE USED***: input.txt --> 197 bytes / 27 bytes
        - we run encoder 100 times: 
            - $\bar{x} = 0,292412 \text{ miliseconds}$
    - ***FILE USED***: input.txt --> 2000 bytes /output.bin ---> 249 bytes
        - we run encoder 100 times: 
            - $\bar{x} = 0.361967 \text{ miliseconds}$

2.`SECOND ATTEMPT` --> 