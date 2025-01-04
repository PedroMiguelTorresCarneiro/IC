#include "BitStream.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include "losslessDecoder.h"



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
    
    

    return metadata;
}
