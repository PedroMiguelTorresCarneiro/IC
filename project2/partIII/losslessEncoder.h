#ifndef LOSSLESS_ENCODER_H
#define LOSSLESS_ENCODER_H
#include <iostream>
#include <vector>
#include "BitStream.h"

void writeMetadata(BitStream &bitStream, const WavData &metadata, int m);
void writeResiduals(BitStream &bitStream, const std::vector<int16_t> &residuals, int m);
#endif