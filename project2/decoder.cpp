#include <iostream>
#include <fstream>
#include <string>
#include "BitStream.h"

using namespace std;

// Function to decode the header and return the number of padding bits
size_t decodeHeader(BitStream &bitStream) {
    string header;
    char currentChar;
    cout << "\nDecoding header..." << endl;
    while (true) {
        // Read one character as 8 bits
        uint64_t charBits = bitStream.readBits(8);
        currentChar = static_cast<char>(charBits);

        if (currentChar == '#') {
            break; // End of header
        }
        header += currentChar;
    }

    // Convert the header to size_t (number of padding bits)
    try {
        cout << "Header decoded successfully.\n" << endl;
        return stoull(header);  
    } catch (const invalid_argument&) {
        cerr << "Invalid header format: unable to convert to a number.\n" << endl;
        throw; // Propagate the exception for handling in the calling function
    }
}

// Function to get the total bits in the file (excluding header)
size_t calculateTotalBits(const string& filePath) {
    ifstream file(filePath, ios::binary | ios::ate);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filePath << endl;
        return 0;
    }
    streamsize fileSize = file.tellg(); // Get size in bytes
    file.close();
    return static_cast<size_t>(fileSize * 8); // Convert bytes to bits
}

// Function to decode the input binary file to a text file
void decodeFile(const string& inputFilePath, const string& outputFilePath) {
    BitStream bitStream(inputFilePath, false); // Open for reading
    cout << "\nDecoding " << inputFilePath << " ... " << endl;
    size_t paddingBits = 0;
    try {
        // Decode the header to get the number of padding bits
        paddingBits = decodeHeader(bitStream);
    } catch (...) {
        cerr << "Failed to decode header.\n" << endl;
        return;
    }

    // Calculate the total number of bits in the file
    size_t totalBits = calculateTotalBits(inputFilePath);
    if (totalBits == 0) {
        cerr << "Failed to determine the total number of bits in the file.\n" << endl;
        return;
    }

    // Calculate the number of bits used by the header
    size_t headerBits = (paddingBits > 9) ? 24 : 16; // Assume header length is 2 or 3 characters plus '#'

    // Calculate the number of meaningful bits
    size_t meaningfulBits = totalBits - headerBits - paddingBits;

    // Open the output file for writing the decoded bits
    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        cerr << "Failed to open output file: " << outputFilePath << endl;
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
    } catch (const ios_base::failure&) {
        cerr << "Unexpected end of file while reading." << endl;
    }

    bitStream.close();
    outputFile.close();
    cout << "Decoding complete! \nOutput written to: " << outputFilePath << "\n" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_binary_file> <output_text_file>" << endl;
        return 1;
    }

    string inputFilePath = argv[1];
    string outputFilePath = argv[2];

    // Start timing
    auto start = chrono::high_resolution_clock::now();

    // Call your existing function to process the files
    decodeFile(inputFilePath, outputFilePath);

    // End timing
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "\nDecoding completed in " << elapsed.count() << " seconds.\n" << endl;


    return 0;
}