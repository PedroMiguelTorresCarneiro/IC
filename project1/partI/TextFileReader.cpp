#include "TextFileReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>  // To remove_if, transform
#include <cctype>     // To tolower, ispunct
#include <filesystem> // To extract th filename from the path
#include <boost/locale.hpp>  // Include Boost.Locale for Unicode handling
#include <vector>
#include <iomanip>  // For setw

using namespace std;

/*
string TextFileReader::guessEncoding(const string& filePath) {
    UErrorCode error = U_ZERO_ERROR;
    UCharsetDetector* charsetDetector = ucsdet_open(&error);

    // Open the file in binary mode
    ifstream inputFile(filePath, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        ucsdet_close(charsetDetector);  // Clean up on error
        return "";
    }

    // Set a maximum of 4kb for imput text for analysis
    const size_t bufferSize = 4096;
    vector<char> buffer(bufferSize);

    // Read up to bufferSize bytes from the file
    inputFile.read(buffer.data(), bufferSize);
    streamsize bytesRead = inputFile.gcount();  // Get the actual number of bytes read
    inputFile.close();

    // Set the input text in the charset detector
    ucsdet_setText(charsetDetector, buffer.data(), static_cast<int32_t>(bytesRead), &error);
    
    // Generates the guess of the encoding
    const UCharsetMatch* match = ucsdet_detect(charsetDetector, &error);

    // Get the name of encoding
    const char* encoding = ucsdet_getName(match, &error);
    
    // Close the charset detector
    ucsdet_close(charsetDetector);

    if (encoding == nullptr || U_FAILURE(error)) {
        cerr << "Error: No valid encoding detected for: " << filePath << endl;
        return "";
    }

    // Return the encoding type
    return string(encoding);
}
*/

// -------------------------------------------------------------------------------------------------------------------[loadFile START]
bool TextFileReader::loadFile(const string& filePath, string& loadedFileName) {
    ifstream inputFile(filePath, ios::binary);  // Open the file in binary mode
    if (!inputFile.is_open()) {                           // Check if the file is open
        cerr << "Error: Unable to open file: " << filePath << endl;
        return false;
    }

    // Extract file name from the file path using filesystem::path
    loadedFileName = filesystem::path(filePath).filename().string();

    list<vector<string>> content;  // List to store lines, each line is a vector of UTF-8 chars

    char byte;
    vector<string> currentLine;  // Vector to store the UTF-8 characters of the current line

    while (inputFile.get(byte)) {  // Read each byte from the file
        unsigned char firstByte = static_cast<unsigned char>(byte);
        
        // Store the line in the list when newline is encountered
        if (firstByte == '\n') {
            content.push_back(currentLine);  
            currentLine.clear();
            continue;
        }

        // Determine how many bytes the character occupies
        int charLength = utf8CharBytes(firstByte);  

        if (charLength == 0) {
            cerr << "Invalid UTF-8 sequence encountered." << endl;
            return false;  // Return error if invalid UTF-8 sequence is found
        }

        // Read the rest of the bytes for the multi-byte character
        string utf8Char(1, byte);  //Start a string with one character(byte).
        for (int i = 1; i < charLength; ++i) {
            if (!inputFile.get(byte)) {
                cerr << "Unexpected end of file in UTF-8 sequence." << endl;
                return false;
            }
            utf8Char += byte; // add the next byte of this UTF-8 char
        }

        currentLine.push_back(utf8Char);  // Add the UTF-8 character to the current line
    }

    if (!currentLine.empty()) {
        content.push_back(currentLine);  // Store the last line if there’s no newline at the end
    }

    inputFile.close();
    fileContents[loadedFileName] = content;  // Store the content in the map (the file name as key)

    return true;
}

// Function to determine how many bytes a UTF-8 character occupies
int TextFileReader::utf8CharBytes(unsigned char byte) {
    if (byte < 0x80) {  // 1-byte ASCII (U+0000 to U+007F)
        return 1;
    } else if (byte < 0xE0) {  // 2-byte UTF-8 character (U+0080 to U+07FF)
        return 2;
    } else if (byte < 0xF0) {  // 3-byte UTF-8 character (U+0800 to U+FFFF)
        return 3;
    } else {  // 4-byte UTF-8 character (U+10000 to U+10FFFF)
        return 4;
    }
}

// -------------------------------------------------------------------------------------------------------------------[loadFile END]










// -------------------------------------------------------------------------------------------------------------------[printFileContent START]
/**
 * printFileContent
 * Prints the content of a loaded file line by line. If the file isn't loaded, it prints an error message.
 * 
 * @param fileName: The name of the file to print.
 */
void TextFileReader::printFileContent(const string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {        // Check if the file is loaded
        cout << "\n----- CONTENT OF FILE: " << fileName << " -----\n" << endl;
        for (const auto& line : fileContents[fileName]) {
            /*
                auto& :
                    - auto : used to deduce the type of the variable from its initializer.
                    - & : used to capture the variable by reference and not create a copy and alter the copy.
            */
            for (const auto& utf8Char : line) {
                cout << utf8Char;  // Print each UTF-8 character
            }
            cout << endl;
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}
// -------------------------------------------------------------------------------------------------------------------[printFileContent END]










// -------------------------------------------------------------------------------------------------------------------[convertToLowercase START]
/**
 * convertToLowercase
 * Converts all characters in the loaded file to lowercase.
 * 
 * @param fileName: The name of the file to process.
 */
void TextFileReader::convertToLowercase(const string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {  // Check if the file is loaded
        for (auto& line : fileContents[fileName]) {
            for (auto& utf8Char : line) {
                utf8Char = boost::locale::to_lower(utf8Char);  // Convert the character to lowercase
            }
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}
// -------------------------------------------------------------------------------------------------------------------[convertToLowercase END]










// -------------------------------------------------------------------------------------------------------------------[removePunctuation START]
/**
 * removePunctuation
 * Removes all punctuation from the text of the loaded file.
 * 
 * @param fileName: The name of the file to process.
*/
void TextFileReader::removePunctuation(const string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {  // Check if the file is loaded
        for (auto& line : fileContents[fileName]) {           // Iterate over each line
            // Use an iterator to safely replace punctuation with space while iterating
            for (auto& utf8Char : line) {
                // Check if the character is punctuation
                if (utf8Char.size() == 1 && ::ispunct(static_cast<unsigned char>(utf8Char[0]))) {
                    utf8Char = " ";  // Replace punctuation with space
                }
            }

            // Remove duplicated spaces from the line (vector of strings)
            line = removeDuplicatedSpaces(line);
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}

vector<string> TextFileReader::removeDuplicatedSpaces(const vector<string>& line) {
    vector<string> result;
    bool inSpace = false;  // Track if the previous character was a space

    for (const auto& utf8Char : line) {
        if (utf8Char == " ") {
            if (!inSpace) {
                result.push_back(utf8Char);  // Only add a single space
            }
            inSpace = true;  // We're currently in a space sequence
        } else {
            result.push_back(utf8Char);  // Add non-space characters
            inSpace = false;  // Reset the space sequence flag
        }
    }

    return result;
}
// -------------------------------------------------------------------------------------------------------------------[removePunctuation END]











// -------------------------------------------------------------------------------------------------------------------[removeNumbers START]
void TextFileReader::removeNumbers(const string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {  // Check if the file is loaded
        for (auto& line : fileContents[fileName]) {           // Iterate over each line
            // Use an iterator to safely replace numbers with space while iterating
            for (auto& utf8Char : line) {
                // Check if the character is a digit (number)
                if (utf8Char.size() == 1 && ::isdigit(static_cast<unsigned char>(utf8Char[0]))) {
                    utf8Char = " ";  // Replace the number with a space
                }
            }

            // Remove duplicated spaces from the line (vector of strings)
            line = removeDuplicatedSpaces(line);
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}
// -------------------------------------------------------------------------------------------------------------------[removeNumbers END]








// -------------------------------------------------------------------------------------------------------------------[calculateCharsFrequency START]
/**
 * calculateCharsFrequency
 * Calculates and displays the frequency of each character in the file, ignoring case and punctuation.
 * 
 * @param fileName: The name of the file to analyze.
 */
void TextFileReader::calculateCharsFrequency(const string& fileName, bool histogram) {
    // Ensure the file content is loaded
    if (fileContents.find(fileName) != fileContents.end()) {
        map<string, int> charFrequency;  // Map to store character frequencies

        // Iterate over each line
        for (const auto& line : fileContents[fileName]) {
            // Iterate over each UTF-8 character (stored as a string in the vector)
            for (const auto& utf8Char : line) {
                charFrequency[utf8Char]++;  // Count all characters (including punctuation)
            }
        }

        if (histogram){
            // Output the character frequencies as a histogram
            cout << "\n----- Ploting the Histogram CHAR FREQUENCY " << fileName << " -----\n" << endl;
            string freqData = mapToString(charFrequency);
            executePythonScript("plot_histogram.py", "char", freqData);
        }else{
            // Sort and print the word frequencies
            vector<pair<string, int>> sortedChars = sortMapByValue(charFrequency);
            printAlignedMap(sortedChars);
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}
// -------------------------------------------------------------------------------------------------------------------[calculateCharsFrequency END]










// -------------------------------------------------------------------------------------------------------------------[calculateWordsFrequency START]
/**
 * calculateWordsFrequency
 * Calculates and displays the frequency of each word in the file.
 * 
 * @param fileName: The name of the file to analyze.
 */
void TextFileReader::calculateWordsFrequency(const string& fileName, bool histogram) {
    if (fileContents.find(fileName) != fileContents.end()) {
        // First, convert all text to lowercase and remove punctuation
        //removePunctuation(fileName);
        //convertToLowercase(fileName);

        // Map to store word frequencies
        map<string, int> wordFrequency;

        // Tokenize the content
        for (const auto& line : fileContents[fileName]) {
            vector<string> tokens = tokenize(line);
            for (const auto& word : tokens) {
                wordFrequency[word]++;
            }
        }
         if (histogram){ 
            // Output the character frequencies as a histogram
            cout << "\n----- Ploting the Histogram WORD FREQUENCY " << fileName << " -----\n" << endl;
            string freqData = mapToString(wordFrequency);
            executePythonScript("plot_histogram.py", "word", freqData);

        }else{
            // Sort and print the word frequencies
            vector<pair<string, int>> sortedWords = sortMapByValue(wordFrequency);
            printAlignedMap(sortedWords);
        }
        
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}

// -------------------------------------------------------------------------------------------------------------------[calculateWordsFrequency END]










// -------------------------------------------------------------------------------------------------------------------[calculateNgramsFrequency START]
/**
 * calculateNgramsFrequency
 * Calculates and displays the frequency of N-grams in the file.
 * N-grams are sequences of 'n' consecutive words.
 * 
 * @param fileName: The name of the file to analyze.
 * @param n: The size of the N-gram (e.g., bigrams for n=2, trigrams for n=3).
 */
void TextFileReader::calculateNgramsFrequency(const string& fileName, int n, bool histogram) {
    if (fileContents.find(fileName) != fileContents.end()) {
        map<string, int> nGramFrequency;  // To store n-gram frequency
        vector<string> allTokens;

        // Remove punctuation and convert to lowercase
        removePunctuation(fileName);
        convertToLowercase(fileName);

        // Tokenize the content and generate the n-grams
        for (const auto& line : fileContents[fileName]) {
            vector<string> tokens = tokenize(line);
            allTokens.insert(allTokens.end(), tokens.begin(), tokens.end());
        }

        // Generate n-grams from the tokenized words
        for (size_t i = 0; i <= allTokens.size() - n; ++i) {
            string nGram;
            for (int j = 0; j < n; ++j) {
                if (j > 0) nGram += " ";  // Separate words by space
                nGram += allTokens[i + j];
            }
            nGramFrequency[nGram]++;
        }
        
        if (histogram){
            // Output the character frequencies as a histogram
            cout << "\n----- Ploting the Histogram of "<< n << " GRAMS FREQUENCY " << fileName << " -----\n" << endl;
            string freqData = mapToString(nGramFrequency);
            executePythonScript("plot_histogram.py", "ngrams", freqData);
        } else {
            // Sort and print the n-gram frequencies
            vector<pair<string, int>> sortedNgrams = sortMapByValue(nGramFrequency);
            printAlignedMap(sortedNgrams);
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}

/**
 * tokenize
 * Splits a line of text into individual words (tokens) separated by spaces.
 * Handles multi-byte UTF-8 characters correctly.
 *
 * @param line: The line to tokenize.
 * @return: A vector of words (tokens) from the line.
 */
vector<string> TextFileReader::tokenize(const vector<string>& line) {
    vector<string> tokens;
    string currentWord;

    // Iterate through each UTF-8 character in the line
    for (const auto& utf8Char : line) {
        if (utf8Char == " ") {
            if (!currentWord.empty()) {
                tokens.push_back(currentWord);  // Add the current word to the token list
                currentWord.clear();  // Reset the word accumulator
            }
        } else {
            currentWord += utf8Char;  // Add character to the current word
        }
    }

    // Add the last word in the line if the line doesn't end with a space
    if (!currentWord.empty()) {
        tokens.push_back(currentWord);
    }

    return tokens;
}

// -------------------------------------------------------------------------------------------------------------------[calculateNgramsFrequency END]









// -------------------------------------------------------------------------------------------------------------------[sortMapByValue START]
vector<pair<string, int>> TextFileReader::sortMapByValue(const map<string, int>& wordMap) {
    // Copy the map into a vector
    vector<pair<string, int>> sortedVec(wordMap.begin(), wordMap.end());

    // Sort the vector by value in descending order
    sort(sortedVec.begin(), sortedVec.end(), 
              [](const pair<string, int>& a, const pair<string, int>& b) {
                  return b.second < a.second;  // Sort in descending order of frequency
              });

    return sortedVec;  // Return the sorted vector
}
// -------------------------------------------------------------------------------------------------------------------[sortMapByValue END]









// -------------------------------------------------------------------------------------------------------------------[printAlignedMap START]
void TextFileReader::printAlignedMap(const vector<pair<string, int>>& sortedMap) {
    // Header
    cout << "\n Frequency : Char/Word/Expression " << endl;
    cout << "-----------------------------------" << endl;

    // Print each word/expression and its frequency, with proper formatting
    for (const auto& pair : sortedMap) {
        // Print the frequency with right alignment in a width of 5
        cout << setw(6) << right << pair.second
                  << "     : " 
                  // Print the word/expression after the colon
                  << pair.first 
                  << endl;
    }
    cout << "-----------------------------------\n" << endl;
}

// -------------------------------------------------------------------------------------------------------------------[printAlignedMap END]










// -------------------------------------------------------------------------------------------------------------------[mapToString START]
string TextFileReader::mapToString(const map<string, int>& frequencyMap) {
    string result;
    for (const auto& pair : frequencyMap) {
        string key = pair.first;

        // Escape or replace problematic characters for shell
        replace(key.begin(), key.end(), ',', ' ');  // Replace commas with spaces
        replace(key.begin(), key.end(), ':', ' ');  // Replace colons with spaces
        replace(key.begin(), key.end(), '"', '\''); // Replace double quotes with single quotes

        result += key + ":" + to_string(pair.second) + ",";
    }

    if (!result.empty()) {
        result.pop_back();  // Remove trailing comma
    }
    return result;
}
// -------------------------------------------------------------------------------------------------------------------[mapToString END]










// -------------------------------------------------------------------------------------------------------------------[executePythonScript START]
void TextFileReader::executePythonScript(const string& scriptName, const string& mode, const string& data) {
    // Wrap the entire data string in double quotes to escape special characters
    string command = "python3 ../" + scriptName + " " + mode + " \"" + data + "\"";
    system(command.c_str());  // Execute the command
}
// -------------------------------------------------------------------------------------------------------------------[executePythonScript END]
