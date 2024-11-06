#include <iostream>
#include <fstream>
#include <string>
#include "BitStream.h"

// Function to decode the header and return the number of meaningful bits
/*
    decodeHeader() function is used to decode the header from the file.
    It reads the header until the delimiter '#' is encountered.
    It converts the header to a number and returns it as size_t.
*/
size_t decodeHeader(BitStream &bitStream) {
    std::string header;
    char currentChar;
    std::cout << "\nDecoding header..." << std::endl;
    while (true) {
        // Read one character as 8 bits
        uint64_t charBits = bitStream.readBits(8);
        currentChar = static_cast<char>(charBits);

        //std::cout << "Read character: " << currentChar << std::endl; // Debugging output

        if (currentChar == '#') {
            break; // End of header
        }
        header += currentChar;
    }

    // Convert the header to size_t
    try {
        std::cout << "Header decoded successfully.\n" << std::endl;
        return std::stoull(header);  
    } catch (const std::invalid_argument&) {
        std::cerr << "Invalid header format: unable to convert to a number.\n" << std::endl;
        throw; // Propagate the exception for handling in the calling function
    }
}

// Function to decode the input binary file to a text file
/*
    decodeFile() function is used to decode the input binary file to a text file.
    It reads the header to get the number of meaningful bits and decodes the message bits.
    It writes the decoded bits to the output text file.
*/
void decodeFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    BitStream bitStream(inputFilePath, false); // Open for reading
    std::cout << "\nDecoding " << inputFilePath << " ... " << std::endl;
    size_t meaningfulBits = 0;
    try {
        // Decode the header to get the number of meaningful bits
        meaningfulBits = decodeHeader(bitStream);
    } catch (...) {
        std::cerr << "Failed to decode header.\n" << std::endl;
        return;
    }

    // Open the output file for writing the decoded bits
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFilePath << std::endl;
        return;
    }

    // Decode the message bits according to the meaningful bits count
    size_t bitsRead = 0;
    try {
        while (bitsRead < meaningfulBits) {
            bool bit = bitStream.readBit();
            outputFile << (bit ? '1' : '0');
            bitsRead++;
        }
    } catch (const std::ios_base::failure&) {
        std::cerr << "Unexpected end of file while reading." << std::endl;
    }

    bitStream.close();
    outputFile.close();
    std::cout << "Decoding complete! \nOutput written to: " << outputFilePath <<"\n " <<std::endl;
}

int main() {
    std::string inputFilePath, outputFilePath;
    std::cout << "Enter input binary file path: ";
    std::cin >> inputFilePath;
    std::cout << "Enter output text file path (to reconstruct 0s and 1s): ";
    std::cin >> outputFilePath;

    decodeFile(inputFilePath, outputFilePath);

    return 0;
}
