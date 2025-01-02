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

    wavData.fileSize = bitStream.readBitsLittleEndian(32);
    wavData.waveHeader = readStringAux(bitStream, 4);
    if (wavData.waveHeader != "WAVE") {
        throw std::runtime_error("Invalid File!");
    }

    // Read ftm chunk
    wavData.fmtChunk = readStringAux(bitStream, 4);
    if (wavData.fmtChunk != "fmt ") {
        throw std::runtime_error("Invalid File!");
    }

    wavData.fmtChunkSize = bitStream.readBitsLittleEndian(32);  // Chunk size
    wavData.audioFormat = bitStream.readBitsLittleEndian(16);   // Audio format (1 = PCM)
    wavData.channelCount = bitStream.readBitsLittleEndian(16);  // Number of channels
    wavData.sampleRate = bitStream.readBitsLittleEndian(32);    // Sample rate
    bitStream.readBitsLittleEndian(32);                         // Byte rate (skip)
    bitStream.readBitsLittleEndian(16);                         // Block align (skip)
    wavData.bitDepth = bitStream.readBitsLittleEndian(16);      // Bits per sample

    std::cout << "Audio Format: " << wavData.audioFormat << std::endl;

    if (wavData.audioFormat != 1) {
        throw std::runtime_error("Unsupported WAV file: Only PCM is supported");
    }

    std::cout<<"Header read"<<std::endl;

    // Data chunk
    std::string dataChunk;
    do {
        dataChunk = readStringAux(bitStream, 4); 
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
    std::cout<<"Data read"<<std::endl;

    return wavData;
}

//auxiliary function to read strings
std::string readStringAux(BitStream &bitStream, size_t length) {
    std::string result;
    for (size_t i = 0; i < length; ++i) {
        char ch = static_cast<char>(bitStream.readBitsLittleEndian(8));
        result.push_back(ch);
    }
    return result;
}
