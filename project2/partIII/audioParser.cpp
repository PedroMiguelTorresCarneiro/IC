#include <SFML/Audio.hpp>
#include <iostream>
#include "BitStream.h"
#include "audioParser.h"
#include <vector>
#include <cstdint>
#include <string>

WavData parseWavFile(BitStream &bitStream) {
    WavData wavData;

    // Read RIFF header
    wavData.riffHeader = readStringAux(bitStream, 4);
    if (wavData.riffHeader != "RIFF") {
        throw std::runtime_error("Invalid File!");
    }

    wavData.fileSize = bitStream.readBits(32);
    wavData.waveHeader = readStringAux(bitStream, 4);
    if (wavData.waveHeader != "WAVE") {
        throw std::runtime_error("Invalid File!");
    }

    // Read ftm chunk
    wavData.fmtChunk = readStringAux(bitStream, 4);
    if (wavData.fmtChunk != "fmt ") {
        throw std::runtime_error("Invalid File!");
    }

    wavData.fmtChunkSize = bitStream.readBits(32);  // Chunk size
    wavData.audioFormat = bitStream.readBits(16);   // Audio format (1 = PCM)
    wavData.channelCount = bitStream.readBits(16);  // Number of channels
    wavData.sampleRate = bitStream.readBits(32);    // Sample rate
    bitStream.readBits(32);                         // Byte rate (skip)
    bitStream.readBits(16);                         // Block align (skip)
    wavData.bitDepth = bitStream.readBits(16);      // Bits per sample

    if (wavData.audioFormat != 1) {
        throw std::runtime_error("Unsupported WAV file: Only PCM is supported");
    }

    std::cout<<"Header read"<<std::endl;

    // Data chunk
    std::string dataChunk;
    do {
        dataChunk = readStringAux(bitStream, 4); 
        uint32_t chunkSize = bitStream.readBits(32); 
        if (dataChunk == "data") {
            wavData.dataChunkSize = chunkSize;
            wavData.numSamples = chunkSize / (wavData.bitDepth / 8);
            wavData.samples.reserve(wavData.numSamples);

            // Get the audio samples
            for (uint32_t i = 0; i < wavData.numSamples; ++i) {
                wavData.samples.push_back(bitStream.readBits(wavData.bitDepth));
            }
            break;
        } else {
            // Skip non-data chunks
            bitStream.readBits(chunkSize * 8);
        }
    } while (!bitStream.eof());

    return wavData;
}

//auxiliary function to read strings
std::string readStringAux(BitStream &bitStream, size_t length) {
    std::string result;
    for (size_t i = 0; i < length; ++i) {
        char ch = static_cast<char>(bitStream.readBits(8));
        result.push_back(ch);
    }
    return result;
}
