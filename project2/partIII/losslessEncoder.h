#ifndef LOSSLESS_ENCODER_H
#define LOSSLESS_ENCODER_H
#include <iostream>
#include <vector>
#include "BitStream.h"
#include "audioParser.h"

void writeMetadata(BitStream &bitStream, const WavData &metadata, int m);
void writeResiduals(BitStream &bitStream, const std::vector<int16_t> &residuals, int m);
void finalEncode(const std::string &inputWav, const std::string &outputCodec);
int calculateOptimalM(std::vector<int16_t> vector);
std::vector<int16_t> quantize(std::vector<int16_t> samples, int level);
void lossyEncode(const std::string &inputWav, const std::string &outputCodec);
#endif