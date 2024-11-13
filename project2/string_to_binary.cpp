#include <iostream>
#include <string>
#include "BitStream.h"

using namespace std;

// Function to convert a string to '0's and '1's using BitStream and store it as text
void convertStringToTextBinary(const string& inputString, const string& outputFilePath) {
    BitStream bitStream(outputFilePath, true); // Open BitStream in write mode

    // Loop through each character in the input string
    for (char c : inputString) {
        // Convert character to binary representation and write '0' or '1' as characters
        for (int i = 7; i >= 0; --i) { // Iterate from the most significant bit to the least
            bool bit = (c >> i) & 1; // Extract the bit
            bitStream.writeBits(bit ? '1' : '0', 8); // Write '0' or '1' as a character (8 bits)
        }
    }

    bitStream.close(); // Ensure the buffer is flushed and file is closed
    cout << "String successfully converted to text binary and written to " << outputFilePath << endl;
}

int main() {
    string inputString;
    cout << "Enter a string: ";
    getline(cin, inputString); // Read input string from terminal

    string outputFilePath = "input.txt"; // File to store the '0's and '1's representation
    convertStringToTextBinary(inputString, outputFilePath);

    return 0;
}
