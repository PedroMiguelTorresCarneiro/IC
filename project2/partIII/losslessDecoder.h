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
    std::vector<int16_t> samples;
};

Metadata readMetadata(BitStream &bitStream);
std::vector<int16_t> decodeResiduals(int m, BitStream &bitStream, int numSamples);
void finalDecode(const std::string &inputCodec, const std::string &outputWav);
void readAndPrintWavHeader(BitStream& bitStream);
#endif