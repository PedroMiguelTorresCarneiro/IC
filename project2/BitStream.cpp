#include "BitStream.h"

// Constructor
BitStream::BitStream(const std::string& filePath, bool writeMode) : buffer(0), bitPos(0), mode(writeMode) {
    if (writeMode) {
        file.open(filePath, std::ios::out | std::ios::binary);
    } else {
        file.open(filePath, std::ios::in | std::ios::binary);
        if (file.is_open()) {
            file.read(&buffer, 1); // Read initial byte
        }
    }

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file");
    }
}

// Destructor
BitStream::~BitStream() {
    if (mode && bitPos > 0) {
        flushBuffer();
    }
    if (file.is_open()) {
        file.close();
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
        throw std::ios_base::failure("Cannot write in read mode");
    }

    buffer = (buffer << 1) | bit; // -----------> Add the bit to the buffer
    bitPos++;

    if (bitPos == 8) { // -----------> If the buffer is full (8 bits), write it to the file
        flushBuffer();
    }
}

// Flushing the buffer to file
/*
    flushBuffer() function is used to flush the buffer to the file.
    It aligns the remaining bits in the buffer and writes the buffer to the file.
*/
void BitStream::flushBuffer() {
    if (bitPos > 0) {
        buffer <<= (8 - bitPos); // Align remaining bits
        file.write(&buffer, 1);
        buffer = 0;
        bitPos = 0;
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
        throw std::ios_base::failure("Cannot read in write mode");
    }

    bool bit = (buffer & (1 << (7 - bitPos))) != 0; // Extract bit
    bitPos++;

    if (bitPos == 8) {
        file.read(&buffer, 1);
        bitPos = 0;
    }

    return bit;
}

// Writing multiple bits (N bits)
/*
    writeBits() function is used to write multiple bits to the file.
    It takes a value and the number of bits to write as input.
    It writes the bits one by one by shifting the value and calling writeBit().
*/
void BitStream::writeBits(uint64_t value, int numBits) {
    for (int i = numBits - 1; i >= 0; --i) {
        writeBit((value >> i) & 1);
    }
}

// Reading multiple bits (N bits)
/*
    readBits() function is used to read multiple bits from the file.
    It takes the number of bits to read as input and returns the value read.
    It reads the bits one by one by calling readBit() and shifting the value.
*/
uint64_t BitStream::readBits(int numBits) {
    uint64_t value = 0;
    for (int i = 0; i < numBits; ++i) {
        value = (value << 1) | readBit();
    }
    return value;
}

// Writing a string as bits
/*
    writeString() function is used to write a string to the file as bits.
    It iterates over each character in the string and writes it as 8 bits.
*/
void BitStream::writeString(const std::string& str) {
    for (char c : str) {
        writeBits(static_cast<uint64_t>(c), 8); // Write each character as 8 bits
    }
}

// Reading a string from bits
/*
    readString() function is used to read a string from the file as bits.
    It reads characters one by one until the end of the file is reached.
*/
std::string BitStream::readString() {
    std::string result;
    while (file.peek() != EOF) {
        char c = static_cast<char>(readBits(8));
        result += c;
    }
    return result;
}

// Closing the file explicitly
/*
    close() function is used to close the file explicitly.
    It flushes the buffer if in write mode and closes the file.
*/
void BitStream::close() {
    if (mode && bitPos > 0) {
        flushBuffer();
    }
    if (file.is_open()) {
        file.close();
    }
}
