#include "LossyVideo.h"
#include <iostream>
#include <chrono>

void printUsage() {
    std::cout << "Usage:\n"
              << "./IntraCoding -Video <VIDEO_PATH> -SignMag|-PosNeg [-m <NUM>] -q <NUM>\n"
              << "Example:\n"
              << "./IntraCoding -Video \"input.mp4\" -SignMag -q 4\n"
              << "./IntraCoding -Video \"input.mp4\" -PosNeg -m 8 -q 4\n";
}

int main(int argc, char* argv[]) {
    if (argc < 6 || std::string(argv[1]) != "-Video") {
        printUsage();
        return 1;
    }

    std::string videoPath = argv[2];
    std::string codingType = argv[3];
    int mValue = 0; // Optional parameter
    int quantizationBits = 0; // Mandatory parameter

    if (codingType != "-SignMag" && codingType != "-PosNeg") {
        std::cerr << "Error: Invalid coding type. Use -SignMag or -PosNeg." << std::endl;
        printUsage();
        return 1;
    }

    for (int i = 4; i < argc; ++i) {
        std::string option = argv[i];
        if (option == "-m" && i + 1 < argc) {
            try {
                mValue = std::stoi(argv[++i]);
                if (mValue <= 0) throw std::invalid_argument("mValue must be positive.");
            } catch (...) {
                std::cerr << "Error: Invalid value for m." << std::endl;
                printUsage();
                return 1;
            }
        } else if (option == "-q" && i + 1 < argc) {
            try {
                quantizationBits = std::stoi(argv[++i]);
                if (quantizationBits <= 0 || quantizationBits > 8) throw std::invalid_argument("Quantization bits must be between 1 and 8.");
            } catch (...) {
                std::cerr << "Error: Invalid value for quantization bits." << std::endl;
                printUsage();
                return 1;
            }
        } else {
            std::cerr << "Error: Unknown or malformed option " << option << std::endl;
            printUsage();
            return 1;
        }
    }

    if (quantizationBits == 0) {
        std::cerr << "Error: Quantization bits (-q) must be specified." << std::endl;
        printUsage();
        return 1;
    }

    try {
        LossyVideo videoEncoder(videoPath, codingType == "-SignMag" ? "SIGN_MAG" : "POS_NEG", mValue, quantizationBits);

        // Encode the video
        auto startEncoding = std::chrono::high_resolution_clock::now();

        std::string encodedFile = "output_video.dat";
        videoEncoder.processVideo(encodedFile, quantizationBits);

        auto endEncoding = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> encodingTime = endEncoding - startEncoding;
        std::cout << "Encoding completed in: " << encodingTime.count() << " seconds.\n";

        // Decode and reconstruct the video
        auto startDecoding = std::chrono::high_resolution_clock::now();
        std::string reconstructedFile = "reconstructed_video";
        std::string originalFormat = videoEncoder.getOriginalFormat(encodedFile);
        reconstructedFile += "." + originalFormat;

        videoEncoder.reconstructVideo(encodedFile, reconstructedFile);

        auto endDecoding = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> decodingTime = endDecoding - startDecoding;
        std::cout << "Decoding completed in: " << decodingTime.count() << " seconds.\n";

        std::cout << "Encoding completed: " << encodedFile << "\n";
        std::cout << "Decoding and reconstruction completed: " << reconstructedFile << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}