#ifndef LOSSLESS_DECODER_H
#define LOSSLESS_DECODER_H
#include <iostream>
#include <vector>
#include "BitStream.h"

struct Metadata {
    uint32_t sampleRate;
    uint16_t channelCount;
    uint16_t bitDepth;
    uint32_t numSamples;
    int golombParam; // m parameter
};

Metadata readMetadata(BitStream &bitStream);
#endif