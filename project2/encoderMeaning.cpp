#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cmath> // For ceil()
#include "BitStream.h"

using namespace std;

// Function to write the header with meaningful bits information
void writeHeader(BitStream &bitStream, size_t meaningfulBits) {
    string meaningfulBitsStr = to_string(meaningfulBits);
    bitStream.writeString(meaningfulBitsStr); // Write the number of meaningful bits as a string
    bitStream.writeString("#");              // Use '#' as a delimiter to mark the end of the header
}

// Function to encode the input file
void encodeFile(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Failed to open input file: " << inputFilePath << endl;
        return;
    }

    size_t meaningfulBits = 0; // Count of meaningful bits
    char bitChar;
    int bitBuffer = 0; // Buffer to hold bits before writing them as a byte
    int bitCount = 0;  // Counter for number of bits in the buffer

    // Create a BitStream object for the final output (for header and data)
    BitStream bitStream(outputFilePath, true);

    // Encode the data and count meaningful bits
    string encodedData;
    while (inputFile.get(bitChar)) {
        if (bitChar == '0' || bitChar == '1') {
            bitBuffer = (bitBuffer << 1) | (bitChar - '0'); // Add bit to buffer
            bitCount++;
            meaningfulBits++;

            // If we have 8 bits, write the byte to the encoded data string
            if (bitCount == 8) {
                encodedData += static_cast<char>(bitBuffer); // Store encoded byte
                bitBuffer = 0;
                bitCount = 0;
            }
        } else {
            cerr << "Invalid character in input file (only '0' and '1' allowed)." << endl;
            inputFile.close();
            bitStream.close();
            return;
        }
    }

    inputFile.close();

    // Write the header with the actual meaningful bits count
    writeHeader(bitStream, meaningfulBits);

    // Write the encoded data
    for (char byte : encodedData) {
        bitStream.writeBits(static_cast<uint64_t>(byte), 8);
    }

    bitStream.close();

    cout << "Encoding complete. Output written to: " << outputFilePath << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_text_file> <output_binary_file>" << endl;
        return 1;
    }

    string inputFilePath = argv[1];
    string outputFilePath = argv[2];

    // Start timing
    auto start = chrono::high_resolution_clock::now();

    // Call your existing function to process the files
    encodeFile(inputFilePath, outputFilePath);

    // End timing
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> elapsed = end - start;
    cout << "\nEncoding completed in " << elapsed.count() << " milliseconds.\n" << endl;

    return 0;
}
