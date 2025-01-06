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
    wavData.riffHeader = bitStream.readStringAux(4);
    if (wavData.riffHeader != "RIFF") {
        throw std::runtime_error("Invalid File! 1");
    }

    wavData.fileSize = bitStream.readBitsLittleEndian(32);
    wavData.waveHeader = bitStream.readStringAux(4);
    if (wavData.waveHeader != "WAVE") {
        throw std::runtime_error("Invalid File! 2");
    }

    // Read ftm chunk
    wavData.fmtChunk = bitStream.readStringAux(4);
    if (wavData.fmtChunk != "fmt ") {
        throw std::runtime_error("Invalid File! 3");
    }
    
    wavData.fmtChunkSize = bitStream.readBitsLittleEndian(32);  // Chunk size
    wavData.audioFormat = bitStream.readBitsLittleEndian(16);   // Audio format (1 = PCM)
    wavData.channelCount = bitStream.readBitsLittleEndian(16);  // Number of channels
    wavData.sampleRate = bitStream.readBitsLittleEndian(32);    // Sample rate
    bitStream.readBitsLittleEndian(32);                         // Byte rate (skip)
    bitStream.readBitsLittleEndian(16);                         // Block align (skip)
    wavData.bitDepth = bitStream.readBitsLittleEndian(16);      // Bits per sample


    if (wavData.audioFormat != 1) {
        throw std::runtime_error("Unsupported WAV file: Only PCM is supported");
    }


    // Data chunk
    std::string dataChunk;
    do {
        dataChunk = bitStream.readStringAux(4); 
        uint32_t chunkSize = bitStream.readBitsLittleEndian(32); 
        if (dataChunk == "data") {
            wavData.dataChunkSize = chunkSize;
            wavData.numSamples = chunkSize / (wavData.bitDepth / 8);
            wavData.samples.reserve(wavData.numSamples);

            // Get the audio samples
            for (uint32_t i = 0; i < wavData.numSamples; ++i) {
                wavData.samples.push_back(bitStream.readBitsLittleEndian(wavData.bitDepth));
            }
            break;
        } else {
            // Skip non-data chunks
            bitStream.readBitsLittleEndian(chunkSize * 8);
        }
    } while (!bitStream.eof());

    return wavData;
}


