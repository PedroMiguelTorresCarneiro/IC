#include "TextFileReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>  // To remove_if, transform
#include <cctype>     // To tolower, ispunct
#include <filesystem> // To extract th filename from the path
#include <unicode/ucnv.h>
#include <unicode/ucsdet.h>

using namespace std;

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

/**
 * loadFile
 * Loads the content of the file specified by filePath into memory, storing it in the `fileContents` map.
 * The file content is split into lines and stored as a list of strings.
 * 
 * @param filePath: Path to the file to load.
 * @param loadedFileName: A reference to store the name of the loaded file (extracted from the path).
 * @return: Returns true if the file was successfully loaded, otherwise false.
 */
bool TextFileReader::loadFile(const std::string& filePath, std::string& loadedFileName) {
    std::ifstream inputFile(filePath);          // Open the file for reading
    if (!inputFile.is_open()) {                 // Check if the file is open
        std::cerr << "Error: Unable to open file: " << filePath << std::endl;
        return false;
    }

    // Extract file name from the file path using std::filesystem::path
    loadedFileName = std::filesystem::path(filePath).filename().string();

    std::list<std::string> content;             // List to store the file's content line by line
    std::string line;

    while (std::getline(inputFile, line)) {
        content.push_back(line);               // Add each line to the list
        /*
            push_back() :
                - Adds a new element at the end of the list, after its current last element.
                - The content of the line is copied to the list.
        */
    }

    inputFile.close();
    fileContents[loadedFileName] = content;  // Store the content in the map (the file name as key)
    return true;
}


/**
 * printFileContent
 * Prints the content of a loaded file line by line. If the file isn't loaded, it prints an error message.
 * 
 * @param fileName: The name of the file to print.
 */
void TextFileReader::printFileContent(const std::string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {        // Check if the file is loaded
        std::cout << "\n----- CONTENT OF FILE: " << fileName << " -----\n" << std::endl;
        for (const auto& line : fileContents[fileName]) {
            /*
                auto& :
                    - auto : used to deduce the type of the variable from its initializer.
                    - & : used to capture the variable by reference and not create a copy and alter the copy.
            */
            std::cout << line << std::endl;
        }
    } else {
        std::cout << "File not found or not loaded: " << fileName << std::endl;
    }
}

/**
 * convertToLowercase
 * Converts all characters in the loaded file to lowercase.
 * 
 * @param fileName: The name of the file to process.
 */
void TextFileReader::convertToLowercase(const string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {        // Check if the file is loaded
        for (auto& line : fileContents[fileName]) {
            for (char& c : line) {
                c = tolower(c);        // Convert each character to lowercase
            }
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}


/**
 * removePunctuation
 * Removes all punctuation from the text of the loaded file.
 * 
 * @param fileName: The name of the file to process.
 */
void TextFileReader::removePunctuation(const string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {        // Check if the file is loaded
        for (auto& line : fileContents[fileName]) {

            // Remove punctuation using remove_if and ::ispunct
            line.erase(remove_if(line.begin(), line.end(), ::ispunct), line.end());
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}


/**
 * calculateCharsFrequency
 * Calculates and displays the frequency of each character in the file, ignoring case and punctuation.
 * 
 * @param fileName: The name of the file to analyze.
 */
void TextFileReader::calculateCharsFrequency(const string& fileName) {
    // First, convert all text to lowercase
    convertToLowercase(fileName);
    // Then, remove punctuation
    removePunctuation(fileName);

    if (fileContents.find(fileName) != fileContents.end()) {
        map<char, int> charFrequency;      // Map to store character frequencies
        for (const auto& line : fileContents[fileName]) {
            for (char c : line) {
                if (isalpha(c)) {          // Only count alphabetic characters
                    charFrequency[c]++;
                }
            }
        }

        // Output the character frequencies
        cout << "\n----- CHAR FREQUENCY OF FILE: " << fileName << " -----\n" << endl;
        cout << " Character : Frequency" << endl;
        for (const auto& pair : charFrequency) {
            cout << "     "<< pair.first << "     :    " << pair.second << endl;
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}


/**
 * calculateWordsFrequency
 * Calculates and displays the frequency of each word in the file.
 * 
 * @param fileName: The name of the file to analyze.
 */
void TextFileReader::calculateWordsFrequency(const string& fileName) {
    // Check if the file is loaded
    if (fileContents.find(fileName) != fileContents.end()) {
        
        // First, convert all text to lowercase
        convertToLowercase(fileName);
        
        // Then, remove punctuation
        removePunctuation(fileName);

        // Map to store word frequencies
        map<string, int> wordFrequency;

        // Tokenize the cleaned content (lowercase, no punctuation)
        for (const auto& line : fileContents[fileName]) {
            istringstream iss(line);  // Tokenize the line into words using whitespace
            string word;
            while (iss >> word) {
                // Each word is already lowercase and punctuation-free, now count its frequency
                wordFrequency[word]++;
            }
        }

        // Output word frequencies
        cout << "\n----- WORD FREQUENCY OF FILE: " << fileName << " -----\n" << endl;
        cout << " Word : Frequency" << endl;

        for (const auto& pair : wordFrequency) {
            cout << " " << pair.first << " :    " << pair.second << endl;
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}


/**
 * calculateNgramsFrequency
 * Calculates and displays the frequency of N-grams in the file.
 * N-grams are sequences of 'n' consecutive words.
 * 
 * @param fileName: The name of the file to analyze.
 * @param n: The size of the N-gram (e.g., bigrams for n=2, trigrams for n=3).
 */
void TextFileReader::calculateNgramsFrequency(const string& fileName, int n) {
    if (fileContents.find(fileName) != fileContents.end()) {    // Check if the file is loaded in fileContents
        map<string, int> nGramFrequency;              // To store n-gram frequency
        vector<string> allTokens;                     // To store all words

        // First, convert the text to lowercase and remove punctuation
        convertToLowercase(fileName);     // Convert the entire file content to lowercase
        removePunctuation(fileName);      // Remove punctuation from the file content

        // Tokenize the cleaned content and generate the n-grams
        for (const auto& line : fileContents.at(fileName)) {
            vector<string> tokens = tokenize(line);  // Tokenize each line into words
            allTokens.insert(allTokens.end(), tokens.begin(), tokens.end());  // Append tokens from this line
        }

        // Generate n-grams from the tokenized words
        for (size_t i = 0; i <= allTokens.size() - n; ++i) {
            string nGram;
            for (int j = 0; j < n; ++j) {
                if (j > 0) nGram += " ";  // Separate words by space
                nGram += allTokens[i + j];
            }
            nGramFrequency[nGram]++;  // Increment the frequency of the n-gram
        }

        // Output the n-gram frequencies
        cout << "\n----- " << n << "-GRAM FREQUENCY OF FILE: " << fileName << " -----\n" << endl;
        cout << " N-Gram            : Frequency" << endl;
        for (const auto& pair : nGramFrequency) {
            cout << " " << pair.first << "     :    " << pair.second << endl;
        }
    } else {
        cout << "File not found or not loaded: " << fileName << endl;
    }
}


/**
 * tokenize
 * Splits a line of text into individual words (tokens).
 * 
 * @param line: The line to tokenize.
 * @return: A vector of words (tokens) from the line.
 */
vector<string> TextFileReader::tokenize(const string& line) {
    vector<string> tokens;
    istringstream iss(line);           // Stream the line
    string token;
    while (iss >> token) {                  // Extract words using whitespace as a delimiter
        tokens.push_back(token);
    }
    return tokens;
}