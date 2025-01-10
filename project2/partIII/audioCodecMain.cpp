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

        std::string inputWav = "../../../datasets/audio/sample01.wav";
        std::string outputCodec = "../audio.mac";
        
        std::string inputCodec = "../audio.mac";
        std::string outputWav = "../test.wav";
        
        
        //Encode
        finalEncode(inputWav, outputCodec);
        std::cout << "Encoding complete, output written to " << outputCodec << std::endl;

        
        //Decode
        finalDecode(inputCodec, outputWav);
        std::cout << "Decoding complete, output written to " << outputWav << std::endl;
        
       
/*
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

        BitStream input(inputWav, false);
       BitStream output(outputWav, false);
       std::cout<<"Original wav: "<<"\n";
       readAndPrintWavHeader(input);
       std::cout<<"Decoded wav: "<<"\n";
       readAndPrintWavHeader(output);
//---------------------------------------------------------------------------------------------
*/
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

}


