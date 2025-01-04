#include "BitStream.h"

using namespace std;

// Constructor
BitStream::BitStream(const string& filePath, bool writeMode) : buffer(0), bitPos(0), mode(writeMode) {
    if (writeMode) {
        file.open(filePath, ios::out | ios::binary);
    } else {
        file.open(filePath, ios::in | ios::binary);
        if (file.is_open()) {
            file.read(&buffer, 1); // Read initial byte
        }
    }

    if (!file.is_open()) {
        throw ios_base::failure("Failed to open the file");
    }
}

// Destructor
BitStream::~BitStream() {
    if (mode && bitPos > 0) {
        flushBuffer(); // -----------> Flush the buffer before closing
    }
    if (file.is_open()) {
        file.close(); // -----------> Close the file
    }
}

// Writing a single bit
/*
    writeBit() function is used to write a single bit to the file.
    It takes a boolean value as input and appends it to the buffer.
    If the buffer is full (8 bits), it writes the buffer to the file.
*/
void BitStream::writeBit(bool bit) {
    if (!mode) {
        throw ios_base::failure("Cannot write in read mode");
    }

    buffer = (buffer << 1) | bit; // -----------> Add the bit to the buffer
    bitPos++; // -----------> Increment the bit position

    if (bitPos == 8) { // -----------> If the buffer is full (8 bits), write it to the file
        flushBuffer(); // -----------> Flush the buffer
    }
}

// Flushing the buffer to file
/*
    flushBuffer() function is used to flush the buffer to the file.
    It aligns the remaining bits in the buffer and writes the buffer to the file.

    For example, if the buffer contains 3 bits (101), it will be aligned to the left
    (10100000) and written to the file.

*/
void BitStream::flushBuffer() {
    if (bitPos > 0) {
        buffer <<= (8 - bitPos); // -----------> Align remaining bits to the left (e.g., 101 -> 10100000)
        file.write(&buffer, 1); // -----------> Write the buffer to the file
        buffer = 0; // -----------> Reset the buffer
        bitPos = 0; // -----------> Reset the bit position
    }
}

// Reading a single bit
/*
    readBit() function is used to read a single bit from the file.
    It extracts the bit from the buffer and updates the bit position.
    If the buffer is empty, it reads the next byte from the file.
*/
bool BitStream::readBit() {
    if (mode) {
        throw ios_base::failure("Cannot read in write mode"); // -----------> Ensure the stream is not in write mode;
    }

    // -----------> Extract the bit at the current position (bitPos)
    bool bit = (buffer & (1 << (7 - bitPos))) != 0; 
    bitPos++;

    if (bitPos == 8) { // -----------> If we have read all 8 bits from the buffer (full byte),
        file.read(&buffer, 1); // -----------> Read the next byte from the file into the buffer
        bitPos = 0; // -----------> Reset the bit position counter to start reading from the first bit of the new byte
    }

    return bit; // -----------> Return the extracted bit (true for 1, false for 0)
}

// Writing multiple bits (N bits)
/*
    writeBits() function is used to write multiple bits to the file.
    It takes a value and the number of bits to write as input.
    It writes the bits one by one by shifting the value and calling writeBit().
*/
void BitStream::writeBits(uint64_t value, int numBits) {
    for (int i = numBits - 1; i >= 0; --i) { // -----------> Iterate over the number of bits to write
        writeBit((value >> i) & 1); // -----------> Write each bit by shifting the value
    }
}

// Reading multiple bits (N bits)
/*
    readBits() function is used to read multiple bits from the file.
    It takes the number of bits to read as input and returns the value read.
    It reads the bits one by one by calling readBit() and shifting the value.
*/
uint64_t BitStream::readBits(int numBits) {
    uint64_t value = 0; // -----------> Initialize the value to store the read bits
    for (int i = 0; i < numBits; ++i) { // -----------> Iterate over the number of bits to read
        value = (value << 1) | readBit(); // -----------> Read a bit and add it to the value
    }
    return value; // -----------> Return the final value
}

// Writing a string as bits
/*
    writeString() function is used to write a string to the file as bits.
    It iterates over each character in the string and writes it as 8 bits.
*/
void BitStream::writeString(const string& str) {
    for (char c : str) {
        writeBits(static_cast<uint64_t>(c), 8); // -----------> Write each character as 8 bits
    }
}

// Reading a string from bits
/*
    readString() function is used to read a string from the file as bits.
    It reads characters one by one until the end of the file is reached.
*/
string BitStream::readString() {
    string result;
    while (file.peek() != EOF) { // -----------> Check if the end of the file is reached
        char c = static_cast<char>(readBits(8)); // -----------> Read a character (8 bits)
        result += c; // -----------> Append the character to the result string
    }
    return result; // -----------> Return the final string
}


/*
    // Using '\0' as a delimiter for readString()
    string BitStream::readString() {
        string result;
        while (file.peek() != EOF) { // Check if the end of the file is reached
            char c = static_cast<char>(readBits(8)); // Read a character (8 bits)
            if (c == '\0') { // Stop reading if a null character is encountered
                break;
            }
            result += c; // Append the character to the result string
        }
        return result; // Return the final string
    }

*/


// Closing the file explicitly
/*
    close() function is used to close the file explicitly.
    It flushes the buffer if in write mode and closes the file.
*/
void BitStream::close() {
    if (mode && bitPos > 0) {
        flushBuffer(); // -----------> Flush the buffer before closing
    }
    if (file.is_open()) {
        file.close(); // -----------> Close the file
    }
}

//Check if the end of the file has been reached
bool BitStream::eof(){
    return file.eof();
}

//Read bits accounting for little endian
uint64_t BitStream::readBitsLittleEndian(int numBits) {
    uint64_t value = 0;
    for (int i = 0; i < numBits / 8; ++i) {
        uint64_t byte = readBits(8);     
        value |= (byte << (i * 8));       
    }
    return value;
}


//auxiliary function to read strings
std::string BitStream::readStringAux(size_t length) {
    std::string result;
    for (size_t i = 0; i < length; ++i) {
        char ch = static_cast<char>(readBitsLittleEndian(8));
        result.push_back(ch);
    }
    return result;
}

void BitStream::writeBitsLittleEndian(uint64_t value, int numBits) {
    // Write the bits from least significant to most significant
    for (int i = 0; i < numBits; ++i) {
        writeBit((value >> i) & 1); // Extract the i-th bit (from LSB to MSB)
    }
}

