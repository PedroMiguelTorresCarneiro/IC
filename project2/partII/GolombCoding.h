#ifndef GOLOMB_CODING_H
#define GOLOMB_CODING_H

#include "BitStream.h" // Ensure this file is available and correct
#include <cmath>

class GolombCoding {
public:
    enum EncodingMode { SIGN_MAG, POS_NEG };

    GolombCoding(int m, BitStream& bs, EncodingMode mode = SIGN_MAG);
    
    // Function to encode an integer
    void encode(int value);

    // Function to decode an integer
    int decode();

private:
    int m; // Parameter m for Golomb coding
    BitStream& bitStream; // Reference to a BitStream object
    EncodingMode mode; // Current mode
};

#endif // GOLOMB_CODING_H
