# IC - Information and Coding (2024/25)

---
# PROJECT 1 : Part I
## author : Inês Águia

***WORKING README ...***

- **Installing Boost.locale** (to work with UTF-8 encoding)
```bash
#macOs:
    brew install boost
#Linux:
    sudo apt-get update
    sudo apt-get install libboost-all-dev
#Windows:
    Download the Boost libraries from Boost's official website
```

- **Build and Run**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ./TextDecoder
    ```

### Navigation selection
In order to transform the option selection be more easy do understand we adopt a grammar-like structure:

```java
COMMAND	:	arg ("-display")*
		|	"-charFreq" arg 
		|	"-wordFreq" arg
		|	"-nGrams" NUM arg
		;

arg		:	"-load" TEXT ("-lower")* ("-punct")* 
        ;

TEXT	:	path_to_text 
        ;

NUM     :	[0 9]+
        ;	   
```

1. **Load and Display a file**
    ```bash
    ./TextDecoder -load ../../../datasets/text/pt/ep-11-06-22-011.txt -display
    ```
2. **Load a File, Convert to Lowercase, and Display**
    ```bash
    ./TextDecoder -load ../../../datasets/text/pt/ep-11-06-22-011.txt -lower -display
    ```
3. **Load a File, Convert to Lowercase, Remove Punctuation, and Display**
    ```bash
    ./TextDecoder -load ../../../datasets/text/pt/ep-11-06-22-011.txt -lower -punct -display
    ```
4. **Load a File and Calculate Character Frequency**
    ```bash
    ./TextDecoder -charFreq -load ../../../datasets/text/pt/ep-11-06-22-011.txt 
    ```

---
---
# NOTES:

### **UTF-8 Encoding Structure**:
- **1 byte (7 bits)**: <br>
`ASCII` chars, the first 128 Unicode code points *[U+0000 to U+007F]*.  <br>
Includes: 
    - letters (A-Z, a-z) 
    - digits (0-9)
    - punctuation
    - control chars 
    ```java 
    Example:
        'A' (Unicode U+0041) → 01000001 (1 byte)
    ```

- **2 bytes (11 bits)**: <br>
For chars with code points *[U+0080 to U+07FF]*.  <br>
Includes: 
    - accented Latin chars
    - chars from various other scripts. 
    ```java
    Example:
        'é' (Unicode U+00E9) → 11000011 10101001 (2 bytes)
    ```

- **3 bytes (16 bits)**:  <br>
For chars with code points *[U+0800 to U+FFFF]*. <br>
Includes:
    - Asian chars and symbols
    ```java
    Example:
        'ह' (Devanagari character, Unicode U+0939) → 11100000 10100100 10011001 (3 bytes)
    ```

- **4 bytes (21 bits)**:  <br>
For characters with code points *[U+10000 to U+10FFFF]*. <br>
Includes: 
    - various historic chars
    - emoji
    - symbols
    ```java
    Example:
        '𝄞' (musical symbol G clef, Unicode U+1D11E) → 11110000 10011101 10000100 10111110 (4 bytes)
    ```
<br>

The ***<u>first byte of a UTF-8 sequence</u>*** tells you how many bytes the character occupies:
- **1 byte** --> `0xxxxxxx`
- **2 bytes** -> `110xxxxx`
- **3 bytes** -> `1110xxxx`
- **4 bytes** -> `11110xxx`

---

### loadFile
1. **Read file in binary mode:** To ensure we capture the raw bytes
2. **Determine how many bytes each UTF-8 char occupies:** Handle multi-byte chars<br>
    We get the first byte of the of the and check if it is between the range we expect:
    - 1 byte_ `0xxxxxxx` -> range from `00000000` to `011111111` (0 to 127)
    - 2 bytes `110xxxxx` -> range from `11000000` to `11011111` (192 to 223)
    - 3 bytes `1110xxxx` -> range from `11100000` to `11101111` (224 to 239)
    - 4 bytes `11100xxx` -> range from `11100000` to `11100111` (240 to 247)
    
    ```c++
    int TextFileReader::utf8CharBytes(unsigned char byte) {
        if (byte < 0x80) {          // 1-byte ASCII(0 to 127) -> if byte < 128(0x80)
            return 1;
        } else if (byte < 0xE0) {   // 2-byte UTF-8 char(192 to 223) -> if byte < 224(0xE0)
            return 2;
        } else if (byte < 0xF0) {   // 3-byte UTF-8 char(224 to 239) -> if byte < 240(0xF0)
            return 3;
        } else {                    // 4-byte UTF-8 char(240 to 247) -> if byte >= 240
            return 4;
        }
    }
    ```
    After this depending of the calculated number join the bytes in a *string* to make a *multi-byte char*.
3. **Store the result:**  in the appropriate structure for further processing

    ```math
    \text{map<string,list<string>>} \Leftrightarrow \text{map<fileName, list<lines>>}
    ```
    To store the file name , and all the lines of that file, giving the suport to add various files to this structure
---
### convertToLowercase
There is no simple pattern to covert upper cases in lower cases beside ASCII chars. So we use the fucntion `toLower()`in ASCII chars and the external libraries like **ICU** aor **Boost.locale** for the non-ASCII chars.
Boost.locale is simpler to use, so we choose this!
1. Set the global locale to UTF-8 using Boost.Locale
2. Convert line by line from the content stored like UTF-8
    ```c++
    for (auto& line : fileContents[fileName]) {
        // Handles both ASCII and UTF-8 multi-byte characters
        line = boost::locale::to_lower(line);  
    }
    ```

---
I'm usign macOs and I cannot use c++26, because AppleClang doesnt support, so we have to get another way to read and interpret differents encodings.
After some online reading we found external libraries and choose the `ICU (International Componentes for Unicode)`. 
1. **Extract the encoding**
    - `uchardet_open`   -> creates a charset detector
    - `ucsdet_setText`  -> sets the imput text for analysis *(4kb)*
    - `ucsdet_detect`   -> generates a guess about the encoding based on the input text
    - `ucsdet_getName`  -> retrieves the encoding name
