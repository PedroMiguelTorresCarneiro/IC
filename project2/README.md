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

1. `Running the encoder`:
```bash
./run_encoder.sh
```
- Then will appear the following prompt:
    ```
    Enter input text file path (containing 0s and 1s): <input.txt>
    Enter output binary file path: <output.bin>
    ```
    The value for binary file path is a sugestion

2. `Running the decoder`:
```bash
./run_decoder.sh
```
- Then will appear the following prompt:
    ```
    Enter input binary file path: <output.bin>
    Enter output text file path (to reconstruct 0s and 1s): <decoded.txt>
    ```
    The values for output text file path is a sugestion


## ENCODER

1.`FIRST ATTEMPT` --> **HEADER** the nº of padding bits  
- while getting the nº padding bits, create a teporary file writing the code
- write the header and the rest the temporary file
- time of encoding:
    - 0.000820213 seconds

2.`SECOND ATTEMPT` --> **HEADER** the nº of padding bits  
- while getting the nº padding bits create store it in a internal structure
- write the header and the rest the temporary file
- time of encoding:
    - 0.000493798 seconds

## DECODER

1.`FIRST ATTEMPT` --> **HEADER + BODY** count
- calculate the bits of the header
- calculate totatl bits
- get the number of meaningfull bits subtracting the padding number and header size
- time of encoding:
    - 0.00124619 seconds

2.`SECOND ATTEMPT` --> 