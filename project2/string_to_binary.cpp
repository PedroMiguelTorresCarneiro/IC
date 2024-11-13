#include <iostream>
#include <string>
#include "BitStream.h"

using namespace std;

// Function to convert a string to '0's and '1's using BitStream and store in a file
void convertStringToBinaryWithBitStream(const string& inputString, const string& outputFilePath) {
    BitStream bitStream(outputFilePath, true); // Open BitStream in write mode

    // Loop through each character in the input string
    for (char c : inputString) {
        bitStream.writeBits(static_cast<uint64_t>(c), 8); // Write each character as 8 bits using BitStream
    }

    bitStream.close();
    cout << "String successfully converted to binary and written to " << outputFilePath << endl;
}

int main() {
    string inputString;
    cout << "Enter a string: ";
    getline(cin, inputString); // Read input string from terminal

    string outputFilePath = "input.txt";
    convertStringToBinaryWithBitStream(inputString, outputFilePath);

    return 0;
}
