#include <iostream>
#include <fstream>
#include <string>
#include "BitStream.h"

using namespace std;

// Function to write the header with meaningful bits information
void writeHeader(BitStream &bitStream, size_t meaningfulBits) {
    string header = to_string(meaningfulBits) + "#";
    bitStream.writeString(header);
}

// Function to encode the input file
void encodeFile(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Failed to open input file: " << inputFilePath << endl;
        return;
    }

    size_t meaningfulBits = 0;
    char bitChar;
    int bitBuffer = 0;
    int bitCount = 0;

    // Use a temporary file to store the encoded data
    string tempFilePath = outputFilePath + ".tmp";
    BitStream tempBitStream(tempFilePath, true);

    // Encode the data and count meaningful bits
    while (inputFile.get(bitChar)) {
        if (bitChar == '0' || bitChar == '1') {
            bitBuffer = (bitBuffer << 1) | (bitChar - '0');
            bitCount++;
            meaningfulBits++;

            if (bitCount == 8) {
                tempBitStream.writeBits(bitBuffer, 8);
                bitBuffer = 0;
                bitCount = 0;
            }
        } else {
            cerr << "Invalid character in input file (only '0' and '1' allowed)." << endl;
            inputFile.close();
            tempBitStream.close();
            return;
        }
    }

    // Handle padding if there are remaining bits (handled by BitStream class as noted)
    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount);  // Align remaining bits to byte boundary
        tempBitStream.writeBits(bitBuffer, 8);
    }

    tempBitStream.close();
    inputFile.close();

    // Write the final output file with the header and the data from the temporary file
    BitStream finalBitStream(outputFilePath, true);
    writeHeader(finalBitStream, meaningfulBits);

    // Copy data from the temporary file to the final output
    ifstream tempInputFile(tempFilePath, ios::binary);
    if (tempInputFile.is_open()) {
        char byte;
        while (tempInputFile.get(byte)) {
            finalBitStream.writeBits(static_cast<unsigned char>(byte), 8);
        }
        tempInputFile.close();
    }
    finalBitStream.close();

    // Remove the temporary file
    remove(tempFilePath.c_str());

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