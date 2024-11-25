#include "GolombCoding.h"
#include <iostream>

GolombCoding::GolombCoding(int m, BitStream& bs, EncodingMode mode)
    : m(m), bitStream(bs), mode(mode) {}

void GolombCoding::encode(int value) {
    int quotient, remainder;
    if (mode == SIGN_MAG) {
        int signBit = (value < 0) ? 1 : 0;
        int magnitude = std::abs(value);
        quotient = magnitude / m;
        remainder = magnitude % m;

        // Encode quotient using unary coding (e.g., 5 as 111110)
        for (int i = 0; i < quotient; ++i) {
            bitStream.writeBit(1);
        }
        bitStream.writeBit(0); // Terminating zero

        // Encode remainder in binary (fixed-length)
        int numBits = static_cast<int>(std::ceil(std::log2(m)));
        bitStream.writeBits(remainder, numBits);

        // Write the sign bit
        bitStream.writeBit(signBit);
    } else if (mode == POS_NEG) {
        int mappedValue = (value >= 0) ? 2 * value : -2 * value - 1;
        quotient = mappedValue / m;
        remainder = mappedValue % m;

        // Encode quotient using unary coding (e.g., 5 as 111110)
        for (int i = 0; i < quotient; ++i) {
            bitStream.writeBit(1);
        }
        bitStream.writeBit(0); // Terminating zero

        // Encode remainder in binary (fixed-length)
        int numBits = static_cast<int>(std::ceil(std::log2(m)));
        bitStream.writeBits(remainder, numBits);
    }
}

int GolombCoding::decode() {
    int quotient = 0;
    while (bitStream.readBit() == 1) {
        ++quotient; // Counting unary 1s
    }
    // Read remainder
    int numBits = static_cast<int>(std::ceil(std::log2(m)));
    int remainder = bitStream.readBits(numBits);

    if (mode == SIGN_MAG) {
        // Decode magnitude
        int magnitude = quotient * m + remainder;

        // Read and apply the sign bit
        int signBit = bitStream.readBit();
        return (signBit == 1) ? -magnitude : magnitude;
    } else if (mode == POS_NEG) {
        // Decode mapped value
        int mappedValue = quotient * m + remainder;
        return (mappedValue % 2 == 0) ? (mappedValue / 2) : -(mappedValue / 2) - 1;
    }
    return 0; // Default return to avoid warnings
}
