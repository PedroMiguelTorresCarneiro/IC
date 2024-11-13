#include <iostream>
#include <fstream>
#include <string>
#include "BitStream.h"

using namespace std;

// Function to convert a file with '0's and '1's to a string and print it
void convertBinaryFileToString(const string& inputFilePath) {
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Failed to open input file: " << inputFilePath << endl;
        return;
    }

    string bitString;
    char bitChar;
    while (inputFile.get(bitChar)) {
        if (bitChar == '0' || bitChar == '1') {
            bitString += bitChar; // Collect bits as characters
        } else {
            cerr << "Invalid character in input file (only '0' and '1' allowed)." << endl;
            return;
        }
    }
    inputFile.close();

    // Convert the collected bits to a string
    string resultString;
    for (size_t i = 0; i < bitString.size(); i += 8) {
        if (i + 8 > bitString.size()) {
            cerr << "Warning: Incomplete byte found, ignoring remaining bits." << endl;
            break; // Ignore incomplete bytes
        }

        // Convert 8 bits to a character
        char c = 0;
        for (size_t j = 0; j < 8; ++j) {
            c = (c << 1) | (bitString[i + j] - '0'); // Shift left and add current bit
        }
        resultString += c; // Append character to result string
    }

    // Print the result string
    cout << "Converted string: " << resultString << endl;
}

int main() {
    string inputFilePath;
    cout << "Enter the input file path (containing '0's and '1's): ";
    cin >> inputFilePath;

    convertBinaryFileToString(inputFilePath);

    return 0;
}
