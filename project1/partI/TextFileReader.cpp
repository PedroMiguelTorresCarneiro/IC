#include "TextFileReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>  // For std::remove_if, std::transform
#include <cctype>     // For std::tolower, std::ispunct
#include <filesystem>

bool TextFileReader::loadFile(const std::string& filePath, std::string& loadedFileName) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file: " << filePath << std::endl;
        return false;
    }

    // Extract file name from the file path using std::filesystem::path
    loadedFileName = std::filesystem::path(filePath).filename().string();

    std::list<std::string> content;
    std::string line;

    while (std::getline(inputFile, line)) {
        content.push_back(line);
        /*
            push_back() :
                - Adds a new element at the end of the list, after its current last element.
                - The content of the line is copied to the list.
        */
    }

    inputFile.close();
    fileContents[loadedFileName] = content;  // Store the content in the map
    return true;
}

void TextFileReader::printFileContent(const std::string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {
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

void TextFileReader::convertToLowercase(const std::string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {
        for (auto& line : fileContents[fileName]) {
            for (char& c : line) {
                c = std::tolower(c);
            }
        }
    } else {
        std::cout << "File not found or not loaded: " << fileName << std::endl;
    }
}

void TextFileReader::removePunctuation(const std::string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {
        for (auto& line : fileContents[fileName]) {
            line.erase(std::remove_if(line.begin(), line.end(), ::ispunct), line.end());
        }
    } else {
        std::cout << "File not found or not loaded: " << fileName << std::endl;
    }
}

void TextFileReader::calculateCharsFrequency(const std::string& fileName) {
    // First, convert all text to lowercase
    convertToLowercase(fileName);
    // Then, remove punctuation
    removePunctuation(fileName);

    if (fileContents.find(fileName) != fileContents.end()) {
        std::map<char, int> charFrequency;
        for (const auto& line : fileContents[fileName]) {
            for (char c : line) {
                if (std::isalpha(c)) {
                    charFrequency[c]++;
                }
            }
        }

        std::cout << "\n----- CHAR FREQUENCY OF FILE: " << fileName << " -----\n" << std::endl;
        std::cout << " Character : Frequency" << std::endl;
        for (const auto& pair : charFrequency) {
            std::cout << "     "<< pair.first << "     :    " << pair.second << std::endl;
        }
    } else {
        std::cout << "File not found or not loaded: " << fileName << std::endl;
    }
}

void TextFileReader::calculateWordsFrequency(const std::string& fileName) {
    // Check if the file is loaded
    if (fileContents.find(fileName) != fileContents.end()) {
        
        // First, convert all text to lowercase
        convertToLowercase(fileName);
        
        // Then, remove punctuation
        removePunctuation(fileName);

        // Map to store word frequencies
        std::map<std::string, int> wordFrequency;

        // Tokenize the cleaned content (lowercase, no punctuation)
        for (const auto& line : fileContents[fileName]) {
            std::istringstream iss(line);  // Tokenize the line into words using whitespace
            std::string word;
            while (iss >> word) {
                // Each word is already lowercase and punctuation-free, now count its frequency
                wordFrequency[word]++;
            }
        }

        // Output word frequencies
        std::cout << "\n----- WORD FREQUENCY OF FILE: " << fileName << " -----\n" << std::endl;
        std::cout << " Word : Frequency" << std::endl;

        for (const auto& pair : wordFrequency) {
            std::cout << " " << pair.first << " :    " << pair.second << std::endl;
        }
    } else {
        std::cout << "File not found or not loaded: " << fileName << std::endl;
    }
}

void TextFileReader::calculateNgramsFrequency(const std::string& fileName, int n) {
    // Check if the file is loaded in fileContents
    if (fileContents.find(fileName) != fileContents.end()) {
        std::map<std::string, int> nGramFrequency;  // To store n-gram frequency
        std::vector<std::string> allTokens;         // To store all words

        // First, convert the text to lowercase and remove punctuation
        convertToLowercase(fileName);     // Convert the entire file content to lowercase
        removePunctuation(fileName);      // Remove punctuation from the file content

        // Tokenize the cleaned content and generate the n-grams
        for (const auto& line : fileContents.at(fileName)) {
            std::vector<std::string> tokens = tokenize(line);  // Tokenize each line into words
            allTokens.insert(allTokens.end(), tokens.begin(), tokens.end());  // Append tokens from this line
        }

        // Generate n-grams from the tokenized words
        for (size_t i = 0; i <= allTokens.size() - n; ++i) {
            std::string nGram;
            for (int j = 0; j < n; ++j) {
                if (j > 0) nGram += " ";  // Separate words by space
                nGram += allTokens[i + j];
            }
            nGramFrequency[nGram]++;  // Increment the frequency of the n-gram
        }

        // Output the n-gram frequencies
        std::cout << "\n----- " << n << "-GRAM FREQUENCY OF FILE: " << fileName << " -----\n" << std::endl;
        std::cout << " N-Gram            : Frequency" << std::endl;
        for (const auto& pair : nGramFrequency) {
            std::cout << " " << pair.first << "     :    " << pair.second << std::endl;
        }
    } else {
        std::cout << "File not found or not loaded: " << fileName << std::endl;
    }
}

std::vector<std::string> TextFileReader::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}