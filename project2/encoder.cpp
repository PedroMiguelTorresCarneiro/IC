#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

// Function to encode the input file and store the bits in a vector
void encodeFile(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Failed to open input file: " << inputFilePath << endl;
        return;
    }

    vector<bool> encodedBits; // Vector to store the encoded data as individual bits
    char bitChar;

    // Read and encode the data into the vector
    while (inputFile.get(bitChar)) {
        if (bitChar == '0') {
            encodedBits.push_back(false); // Store 0 as false
        } else if (bitChar == '1') {
            encodedBits.push_back(true);  // Store 1 as true
        } else {
            cerr << "Invalid character in input file (only '0' and '1' allowed)." << endl;
            inputFile.close();
            return;
        }
    }

    inputFile.close();

    // Calculate padding bits needed to align the data to a byte boundary
    int paddingBits = 0;
    if (encodedBits.size() % 8 != 0) {
        paddingBits = 8 - (encodedBits.size() % 8);
        for (int i = 0; i < paddingBits; ++i) {
            encodedBits.push_back(false); // Add padding bits (0) to align to a byte
        }
    }

    // Create a BitStream object for the final output
    BitStream bitStream(outputFilePath, true);

    // Write the header with the calculated padding bits
    writeHeader(bitStream, paddingBits);

    // Write the encoded data from the vector to the output file bit by bit
    int bitBuffer = 0;
    int bitCount = 0;
    for (bool bit : encodedBits) {
        bitBuffer = (bitBuffer << 1) | bit; // Add bit to buffer
        bitCount++;

        // If we have 8 bits, write the byte to the BitStream
        if (bitCount == 8) {
            bitStream.writeBits(bitBuffer, 8);
            bitBuffer = 0;
            bitCount = 0;
        }
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