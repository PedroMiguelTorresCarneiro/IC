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
---
<br>
<br>

1.`FIRST ATTEMPT` --> **HEADER** the nº of padding bits  
- while getting the nº padding bits, create a <u>teporary file</u> writing the code
- write the header and the rest the temporary file
- time of encoding:
    1. ### -| 197 Bytes |-
        - `Compression rate`
            - Using the formula, $ \left( 1 - \frac{Compressed_{size}}{Original_{size}}\right) \times 100$
                - $Original_{size}$ = 197 bytes 
                - $Compressed_{size}$ = 28 Bytes,  
            - $Compression_{rate} = 85,79 \%$
        - `Compression time`
            - we run encoder 100 times: 
                - $\bar{x} = 0.421627 \text{ miliseconds}$
        ---
    2. ### -| 2 kBytes |-
        - `Compression rate`
            - Using the formula, $ \left( 1 - \frac{Compressed_{size}}{Original_{size}}\right) \times 100$
                - $Original_{size}$ = 2 kbytes 
                - $Compressed_{size}$ = 250 Bytes,  
            - $Compression_{rate} = 87,5 \%$
        - `Compression time`
            - we run encoder 100 times: 
                - $\bar{x} = 0.510059 \text{ miliseconds}$
        ---
    3. ### -| 2 MBytes |- 
        - `Compression rate`
            - Using the formula, $ \left( 1 - \frac{Compressed_{size}}{Original_{size}}\right) \times 100$
                - $Original_{size}$ = 2 Mbytes 
                - $Compressed_{size}$ = 246KBytes,  
            - $Compression_{rate} = 87,7 \%$
        - `Compression time`
            - we run encoder 100 times: 
                - $\bar{x} = 55.480459 \text{ miliseconds}$
---
<br>

---

2.`SECOND ATTEMPT` --> **HEADER** the nº of padding bits  
- while getting the nº padding bits create store it in a <u>internal structure</u>
- write the header and the rest the temporary file
- time of encoding:
    1. ### -| 197 Bytes |-
        - `Compression rate`
            - Using the formula, $ \left( 1 - \frac{Compressed_{size}}{Original_{size}}\right) \times 100$
                - $Original_{size}$ = 197 bytes 
                - $Compressed_{size}$ = 27 Bytes,  
            - $Compression_{rate} = 85,29 \%$
        - `Compression time`
            - we run encoder 100 times: 
                - $\bar{x} = 0.275198 \text{ miliseconds}$
        ---
    2. ### -| 2 kBytes |-
        - `Compression rate`
            - Using the formula, $ \left( 1 - \frac{Compressed_{size}}{Original_{size}}\right) \times 100$
                - $Original_{size}$ = 2 kbytes 
                - $Compressed_{size}$ = 249 Bytes,  
            - $Compression_{rate} = 87,55 \%$
        - `Compression time`
            - we run encoder 100 times: 
                - $\bar{x} = 0.455895 \text{ miliseconds}$
        ---
    3. ### -| 2 MBytes |- 
        - `Compression rate`
            - Using the formula, $ \left( 1 - \frac{Compressed_{size}}{Original_{size}}\right) \times 100$
                - $Original_{size}$ = 2 Mbytes 
                - $Compressed_{size}$ = 246KBytes,  
            - $Compression_{rate} = 87,7 \%$
        - `Compression time`
            - we run encoder 100 times: 
                - $\bar{x} = 182.785280 \text{ miliseconds}$

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