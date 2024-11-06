#include <iostream>
#include <fstream>
#include <string>
#include "BitStream.h"

// Function to write the header
/*
    writeHeader() function is used to write the header to the file.
    It takes a BitStream object and a string as input.
    It writes the header followed by a delimiter '#'.
*/
void writeHeader(BitStream &bitStream, const std::string &header) {
    bitStream.writeString(header); // Write the header (number of meaningful bits)
    bitStream.writeString("#");    // Use '#' as a delimiter to mark the end of the header
}

// Function to encode the input file
/*
    encodeFile() function is used to encode the input file containing '0's and '1's.
    It reads the input file, counts the meaningful bits, and encodes the data.
    It writes the encoded data to a temporary file using a BitStream object.
    Finally, it writes the header and copies the temporary file contents to the final output file.
*/
void encodeFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file: " << inputFilePath << std::endl;
        return;
    }

    size_t meaningfulBits = 0; // Count of meaningful bits
    char bitChar;
    int bitBuffer = 0; // Buffer to hold bits before writing them as a byte
    int bitCount = 0;  // Counter for number of bits in the buffer

    // First count the meaningful bits and encode the data
    std::string tempOutputFilePath = outputFilePath + ".tmp";
    BitStream bitStream(tempOutputFilePath, true); // Temporary stream for writing bit data

    while (inputFile.get(bitChar)) {
        if (bitChar == '0' || bitChar == '1') {
            bitBuffer = (bitBuffer << 1) | (bitChar - '0'); // Add bit to buffer
            bitCount++;
            meaningfulBits++;

            // If we have 8 bits, write the byte to the BitStream
            if (bitCount == 8) {
                bitStream.writeBits(bitBuffer, 8);
                bitBuffer = 0;
                bitCount = 0;
            }
        } else {
            std::cerr << "Invalid character in input file (only '0' and '1' allowed)." << std::endl;
            inputFile.close();
            bitStream.close();
            return;
        }
    }

    // Handle padding if there are remaining bits
    /*
        If there are remaining bits in the buffer, we need to add 0s to complete the byte.
        We shift the bits to the left to align them and write the byte to the BitStream.
    */
    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount); // Add 0s to complete the byte
        bitStream.writeBits(bitBuffer, 8);
    }

    bitStream.close();
    inputFile.close();

    // Write the final output file with the header
    BitStream finalStream(outputFilePath, true);
    std::string meaningfulBitsStr = std::to_string(meaningfulBits);
    writeHeader(finalStream, meaningfulBitsStr);

    // Copy the temporary file contents (bit data) to the final stream
    std::ifstream tempInput(tempOutputFilePath, std::ios::binary);
    if (tempInput.is_open()) {
        char byte;
        while (tempInput.get(byte)) {
            finalStream.writeBits(static_cast<unsigned char>(byte), 8);
        }
        tempInput.close();
    }
    finalStream.close();

    // Remove the temporary file
    /*
        PROVIROSY:
        We remove the temporary file after copying its contents to the final output file.
    */
    std::remove(tempOutputFilePath.c_str());

    std::cout << "Encoding complete. Output written to: " << outputFilePath << std::endl;
}


int main() {
    std::string inputFilePath, outputFilePath;
    std::cout << "Enter input text file path (containing 0s and 1s): ";
    std::cin >> inputFilePath;
    std::cout << "Enter output binary file path: ";
    std::cin >> outputFilePath;

    encodeFile(inputFilePath, outputFilePath);

    return 0;
}
