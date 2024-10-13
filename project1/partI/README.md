# IC - Information and Coding (2024/25)

---
# PROJECT 1 : Part I
## author : Inês Águia

***WORKING README ...***

- **Installing ICU** (to work with UTF-8 encoding)
```md
#macOs:
    1. brew install icu4c
    2. Set the Environment Variables for pkg-config
        - nano ~/.zshrc (or ~/.bashrc)
        - Add the following line:
            - export PKG_CONFIG_PATH="/opt/homebrew/opt/icu4c/lib/pkgconfig"
        - source ~/.zshrc (or ~/.bashrc)
#Linux:
    - sudo apt install libicu-dev
#Windows:
    1. Install vcpkg:
        - git clone https://github.com/microsoft/vcpkg.git
        - cd vcpkg
        - ./bootstrap-vcpkg.bat
    2. Install ICU:
        - vcpkg install icu
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

### File streams
`std::ifstream` and `std::fstream` read files as raw bytes so if the file were not encoding in *ASCII* or `UTF-8`. 
- so the TExtDecoder doesnt work with differente encodings like : `UTF-16`, `ISO-8859-1`, `Shift-JIS` ...

I'm usign macOs and I cannot use c++26, because AppleClang doesnt support, so we have to get another way to read and interpret differents encodings.
After some online reading we found external libraries and choose the `ICU (International Componentes for Unicode)`. 
1. **Extract the encoding**
    - `uchardet_open`   - creates a charset detector
    - `ucsdet_setText`  - sets the imput text for analysis *(4kb)*
    - `ucsdet_detect`   - generates a guess about the encoding based on the input text
    - `ucsdet_getName`  - retrieves the encoding name

2. 