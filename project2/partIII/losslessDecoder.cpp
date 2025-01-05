#include "BitStream.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include "losslessDecoder.h"
#include "GolombCoding.h"
#include "predictModel.h"



Metadata readMetadata(BitStream &bitStream) {
    Metadata metadata;

    //Validate file type
    std::string fileIdentifier = bitStream.readStringAux(10);
    if (fileIdentifier != "AUDIOCODEC") {
        throw std::runtime_error("Invalid file format: Expected AUDIOCODEC");
    }
    

    //read the metadata
    metadata.sampleRate = bitStream.readBits(32);
    metadata.channelCount = bitStream.readBits(16);
    metadata.bitDepth = bitStream.readBits(16);
    metadata.numSamples = bitStream.readBits(32);
    metadata.golombParam = bitStream.readBits(16);
    

    std::vector<int16_t> samples;

    //Read and golomb decode the residuals
    samples = decodeResiduals(metadata.golombParam, bitStream, metadata.numSamples);

    samples = reconstructSamples(samples);
    /*
    std::vector<int16_t> samplesAux;
    samplesAux.push_back(0);
    metadata.samples = samplesAux;
    */  
    metadata.samples = samples;
    return metadata;
}

//read and decode residuals using golomb
std::vector<int16_t> decodeResiduals(int m, BitStream &bitStream, int numSamples){
    std::vector<int16_t> residuals;
    GolombCoding golombDecoder(m, bitStream, GolombCoding::POS_NEG);

    for (uint32_t i = 0; i < numSamples; ++i) {
        residuals.push_back(golombDecoder.decode());
    }

    return residuals;
}


void writeWavFile(BitStream& bitStream, const Metadata& metadata) {

    uint32_t subchunk2Size = metadata.numSamples * metadata.channelCount * (metadata.bitDepth / 8);
    uint32_t chunkSize = 4 + (8 + 16) + (8 + subchunk2Size); // 4 (RIFF) + 8+16 (fmt) + 8+subchunk2Size (data)
    
    
    // Write RIFF header
    bitStream.writeStringLittleEndian("RIFF");                                  
    bitStream.writeBitsLittleEndian(chunkSize, 32);                             
    bitStream.writeStringLittleEndian("WAVE");                                  

    // Write fmt subchunk
    uint32_t subchunk1Size = 16;                                    
    uint16_t audioFormat = 1;                                       
    uint32_t byteRate = metadata.sampleRate * metadata.channelCount * (metadata.bitDepth / 8);
    uint16_t blockAlign = metadata.channelCount * (metadata.bitDepth / 8);

    bitStream.writeStringLittleEndian("fmt ");                                  
    bitStream.writeBitsLittleEndian(subchunk1Size, 32);                         
    bitStream.writeBitsLittleEndian(audioFormat, 16);                          
    bitStream.writeBitsLittleEndian(metadata.channelCount, 16);                 
    bitStream.writeBitsLittleEndian(metadata.sampleRate, 32);                   
    bitStream.writeBitsLittleEndian(byteRate, 32);                              
    bitStream.writeBitsLittleEndian(blockAlign, 16);                        
    bitStream.writeBitsLittleEndian(metadata.bitDepth, 16);                     
    bitStream.writeStringLittleEndian("data");                                  
    bitStream.writeBitsLittleEndian(subchunk2Size, 32);                         

    // Write audio data (samples)
    for (int16_t sample : metadata.samples) {
        bitStream.writeBitsLittleEndian(sample, metadata.bitDepth);
    }
}

void finalDecode(const std::string &inputCodec, const std::string &outputWav){
    Metadata metadata;
    BitStream inputStream(inputCodec, false); 
    BitStream outputStream(outputWav, true); 

    metadata = readMetadata(inputStream);

    writeWavFile(outputStream, metadata);

    inputStream.close();
    outputStream.close();
}
