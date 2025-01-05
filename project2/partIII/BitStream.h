#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <bitset>

class BitStream {
private:
    std::fstream file;
    char buffer; // Buffer to hold bits
    int bitPos;  // Current bit position within the buffer
    bool mode;   // true for write mode, false for read mode

    void flushBuffer(); // Helper function to flush the buffer when writing

public:
    // Constructor & Destructor
    BitStream(const std::string& filePath, bool writeMode);
    ~BitStream();

    // Core methods
    void writeBit(bool bit);
    bool readBit();
    void writeBits(uint64_t value, int numBits);
    uint64_t readBits(int numBits);
    void writeString(const std::string& str);
    std::string readString();
    bool eof();
    uint64_t readBitsLittleEndian(int numBits);
    std::string readStringAux(size_t length);
    void writeBitsLittleEndian(uint64_t value, int numBits);
    void writeStringLittleEndian(const std::string& str);

    // Utility method for closing the file
    void close();
};

#endif // BITSTREAM_H
