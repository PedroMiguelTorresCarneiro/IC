#include "audioParser.h"
#include "predictModel.h"
#include "BitStream.h"
#include "losslessEncoder.h"
void encode(const std::string &inputWav, const std::string &outputCodec, int m) {

    std::cout<<"Hello!"<<std::endl;
    BitStream inputStream(inputWav, false); // Open WAV file for reading
    BitStream outputStream(outputCodec, true); // Open .audiocodec file for writing

    std::cout<<"Hello!"<<std::endl;
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
        std::string outputCodec = "../audio.mycodec";
        int m = 1;
        // Call the encode function
        encode(inputWav, outputCodec, m);

        std::cout << "Encoding complete. Output written to " << outputCodec << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

}
