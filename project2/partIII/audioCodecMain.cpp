#include "losslessEncoder.h"
#include "losslessDecoder.h"
#include "BitStream.h"

int main(){
    try {
        /*
        std::string inputWav, outputCodec;
        int m;

        std::cout << "Enter the input WAV file path: ";
        std::getline(std::cin, inputWav);

        std::cout << "Enter the output codec file path: ";
        std::getline(std::cin, outputCodec);

        std::cout << "Enter the prediction order (m): ";
        std::cin >> m;

        if (m < 1) {
            throw std::invalid_argument("Prediction order (m) must be greater than 0.");
        }
        */

        std::string inputWav = "../dataSet/sample01.wav";
        std::string outputCodec = "../audio.mac";
        int m = 16; //fixed golomb parameter for now
        // Call the encode function
        finalEncode(inputWav, outputCodec, m);
        std::cout << "Encoding complete, output written to " << outputCodec << std::endl;

//--------------------------TESTING DECODER--------------------------
        // Open the .mycodec file in read mode
        BitStream bitStreamDec("../audio.mac", false);

        // Read the metadata
        Metadata metadata;
        metadata = readMetadata(bitStreamDec);

        // Display the metadata
        std::cout << "Metadata:\n";
        std::cout << "  Sample Rate: " << metadata.sampleRate << " Hz\n";
        std::cout << "  Channels: " << metadata.channelCount << "\n";
        std::cout << "  Bit Depth: " << metadata.bitDepth << " bits\n";
        std::cout << "  Number of Samples: " << metadata.numSamples << "\n";
        std::cout << "  Golomb Parameter (m): " << metadata.golombParam << "\n";
        
        
        bitStreamDec.close();
//---------------------------------------------------------------------------------------------

    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

}