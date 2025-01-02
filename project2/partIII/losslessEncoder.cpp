#include "audioParser.h"
#include "predictModel.h"
#include "BitStream.h"
#include "losslessEncoder.h"
int main(const std::string &inputWav, const std::string &outputCodec, int m) {
    BitStream inputStream(inputWav, false); // Open WAV file for reading
    BitStream outputStream(outputCodec, true); // Open .audiocodec file for writing

    // Parse WAV file
    
    std::vector<int16_t> samples;
    WavData metadata = parseWavFile(inputStream);

    // Calculate residuals
    std::vector<int16_t> residuals = calculateResiduals(metadata.samples);
    

    // Write metadata
    writeMetadata(outputStream, metadata, m);

    // Encode and write residuals
    writeResiduals(outputStream, residuals, m);

    // Clean up
    inputStream.close();
    outputStream.close();

    return 0;
}

void writeMetadata(BitStream &bitStream, const WavData &metadata, int m) {
    // File identifier
    bitStream.writeString("AUDIOCODEC");

    // Metadata
    bitStream.writeBits(metadata.sampleRate, 32);  // Sample rate
    bitStream.writeBits(metadata.channelCount, 16); // Number of channels
    bitStream.writeBits(metadata.bitDepth, 16);     // Bit depth
    bitStream.writeBits(metadata.numSamples, 32);  // Total number of samples

    // Golomb parameter
    bitStream.writeBits(m, 16); // If using fixed m
}

void writeResiduals(BitStream &bitStream, const std::vector<int16_t> &residuals, int m) {
    for (int16_t residual : residuals) {
        //Golomb encoding here
    }
}

