#include "audioParser.h"
#include "predictModel.h"
#include "BitStream.h"
#include "losslessEncoder.h"
#include "GolombCoding.h"

void finalEncode(const std::string &inputWav, const std::string &outputCodec) {

    BitStream inputStream(inputWav, false); // Open WAV file for reading
    BitStream outputStream(outputCodec, true); // Open .audiocodec file for writing

    
    // Parse WAV file
    
    std::vector<int16_t> samples;
    WavData metadata = parseWavFile(inputStream);

    // Calculate residuals
    std::vector<int16_t> residuals;

    
    if(metadata.channelCount == 1){
        residuals = calculateResidualsMono(metadata.samples); //for mono audio
    }else{
        residuals = calculateResidualsStereo(metadata.samples); //for stereo audio
    }
    
    int m = calculateOptimalM(residuals);

    // Write metadata
    writeMetadata(outputStream, metadata, m);

    // Encode and write residuals
    writeResiduals(outputStream, residuals, m);

    // Clean up
    inputStream.close();
    outputStream.close();

}

void writeMetadata(BitStream &bitStream, const WavData &metadata, int m) {
    // File identifier
    bitStream.writeString("AUDIOCODEC");

    // Metadata
    
    bitStream.writeBits(metadata.sampleRate, 32);
    bitStream.writeBits(metadata.channelCount, 16);
    bitStream.writeBits(metadata.bitDepth, 16);
    bitStream.writeBits(metadata.numSamples, 32);
    bitStream.writeBits(m, 16); // Golomb parameter
    
    
}

//encode and write residuals to the file
void writeResiduals(BitStream &bitStream, const std::vector<int16_t> &residuals, int m) {

    
    GolombCoding golombCoder(m, bitStream, GolombCoding::POS_NEG);

    //Encode and write the reisduals
    for (int residual : residuals) {
        golombCoder.encode(residual);
    }
}

//calculate optimal m for golomb encoding
int calculateOptimalM(std::vector<int16_t> vector){
    int sum, mean, num, m = 0;

    for(int16_t sample : vector){
        sum += abs(sample);
        num++;
    }

    mean = sum/num;

    m = ceil((mean+1)/2);
    return m;
}
