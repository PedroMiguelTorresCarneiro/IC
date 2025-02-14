#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cmath> // For ceil()
#include "BitStream.h"

using namespace std;

// Function to write the header with padding information
void writeHeader(BitStream &bitStream, int paddingBits) {
    string paddingBitsStr = to_string(paddingBits);
    bitStream.writeString(paddingBitsStr); // Write the number of padding bits as a string
    bitStream.writeString("#");           // Use '#' as a delimiter to mark the end of the header
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

    // Create a BitStream object for the final output
    BitStream bitStream(outputFilePath, true);

    // Write a placeholder for the header (to be replaced later)
    string headerPlaceholder = "00#"; // Example placeholder, length must match the expected header
    bitStream.writeString(headerPlaceholder);

    // Encode the data and calculate meaningful bits
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
            cerr << "Invalid character in input file (only '0' and '1' allowed)." << endl;
            inputFile.close();
            bitStream.close();
            return;
        }
    }

    // Handle padding if there are remaining bits
    int paddingBits = 0;
    if (bitCount > 0) {
        paddingBits = 8 - bitCount; // Calculate the number of padding bits
        bitBuffer <<= paddingBits;  // Add 0s to complete the byte
        bitStream.writeBits(bitBuffer, 8);
    }

    inputFile.close();
    bitStream.close();

    // Rewrite the header with the actual padding bits
    fstream finalStream(outputFilePath, ios::in | ios::out | ios::binary);
    if (finalStream.is_open()) {
        finalStream.seekp(0); // Move to the beginning of the file
        string paddingBitsStr = to_string(paddingBits);
        finalStream.write(paddingBitsStr.c_str(), paddingBitsStr.size());
        finalStream.put('#'); // Add delimiter
        finalStream.close();
    }

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
