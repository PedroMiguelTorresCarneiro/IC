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
        |   "-hist" ("-charFreq" arg | "-wordFreq" arg | "-nGrams" NUM arg)
		;

arg		:	"-load" TEXT ("-lower")* ("-punct")* ("-num")*
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

1. **PROBLEM**:
- inicially we only convert the ASCII chars to $lowercase$ and we have something like this:
    ```plaintext
        Inês Águia ---> converting to lowercase --> inês Águia 
    ``` 
    As you can see the non-ASCII chars weren't converted to lower case.
2. **SOLUTION**:
- There is no simple pattern to convert $uppercase$ on $lowercase$ beside for the ASCII chars.
- Boost.locale:
    1. We set the variabel locale to UTF-8
    ```c++
        locale::global(boost::locale::generator().generate("en_US.UTF-8"));
    ``` 
    - In `en_GB.UTF-8` : 
        - **en** : language
        - **US** : Country/Region
        - **UTF-8** : character encoding <br>
        - there are a lots of diferentes combinations: 
            - `locale -a` to see them all
    - for example we are working with ***GREEK*** language the lower case have two forms depending on the position of the letter on the word.
        - **Σ** (sigma) :
            - **σ** : Used when it's in the middle of a word.
            - **ς** : Used when it's at the end of a word.
        - So we can use this to get the Boost.locale more precise depending on the language we are working.
    - there are other thinks altered by this:
        - currency (`£` or `€` - pound sterling or EURO)
        - number formating (`1 000,00` instead of `1,000.00`)
        - text direction (Hebrew is `RTL` - right to left) 
        - spelling differences ( `colour` instead of `color`)
        - Date (`DD/MM/YYYY` instead of `MM/DD/YYYY`)
    2. Convert char by char:
    ```c++
        for (auto& line : fileContents[fileName]) {
            for (auto& utf8Char : line) {
                // Convert the character to lowercase
                utf8Char = boost::locale::to_lower(utf8Char);  
            }
        }
    ``` 

3. **LIMITATIONS**
- Using `en_US.UTF-8` we can convert all the basica european languages (English, French German, Spanish) is the most maintainf *locale*. 
- this will be ok for all except the following that have specific rules:
    - Greek
    - Turkish 
    - Eastern Europe ( Czech, Polish, etc..)
    - Arabic languages  

### After this solution the result
```
    Inês Águia ---> ines águia
```

---
### remove Punctuation

In the first run I only remove the punctuation and I got the following:
- **PROBLEM**
    ```plaitext
        NAME="Hannes Swoboda"> ---> remove punctuation ---> NAMEHannes Swoboda
    ```
    - This is a mistake... because were removing the punctuation only, and removing the punctuation only make us combine words.
- **SOLUTION**
    1. **Switch punctuation with `' '`** 
        - creates another problem we got a lot of duplicate,triplicated,.. `' '`
    2. **Remove duplicated `' '`**
    ```plaitext
        NAME="Hannes Swoboda"> ---> remove punctuation ---> NAME Hannes Swoboda
    ```
---
### remove Numbers
We aditionally create a function to remove the numbers in order to get stats only about letters:
- Frequency of determinated letter to understand to what is the most used letter in a language , or the most used letters. With this we can try to create words knowing the percentage in wich each letter appear in a language.


(SE TIVER TEMPO FAZER UM BRINCADEIRA PARA CONSTRUIR ALGUMAS PALAVRAS)


---

### HISTOGRAM
We got an python script to plot the histogram command with some thresholds in order to interpretate the values:
- chars :
    - appear all the values

    | Histogram of Char Frequencies (ep-11-06-22-011.txt) |
    |-------------------------------|
    | ![CharFreq](../partI/imgs/charFreq.png) |
    | `./TextDecoder -hist -charFreq -load ../../../datasets/text/pt/ep-11-06-22-011.txt -lower -punct` |

- words :
    - appear only the words above 5 of frequency 

    | Histogram of Word Frequencies (ep-11-06-22-011.txt) |
    |-------------------------------|
    | ![WordFreq](../partI/imgs/wordFreq.png) |
    | `./TextDecoder -hist -wordFreq -load ../../../datasets/text/pt/ep-11-06-22-011.txt -lower -punct` |
- nGrams :
    - apper only the ngrams above 2 of frequency
    
    | Histogram of 3Grams Frequencies (ep-11-06-22-011.txt) |
    |-------------------------------|
    | ![NGramFreq](../partI/imgs/3GramFreq.png) |
    | `./TextDecoder -hist -nGrams 3 -load ../../../datasets/text/pt/ep-11-06-22-011.txt -lower -punct` |

