#ifndef AUDIO_PARSER_H
#define AUDIO_PARSER_H
#include <SFML/Audio.hpp>
#include <iostream>
#include "BitStream.h"

struct WavData {
    std::string riffHeader;   
    uint32_t fileSize;        
    std::string waveHeader;   
    std::string fmtChunk;     
    uint32_t fmtChunkSize;    
    uint16_t audioFormat;     
    uint16_t channelCount;    
    uint32_t sampleRate;      
    uint16_t bitDepth;        
    uint32_t dataChunkSize;  
    uint32_t numSamples;     
    std::vector<int16_t> samples; 
};

WavData parseWavFile(BitStream &bitStream);
std::string readStringAux(BitStream &bitStream, size_t length);

#endif