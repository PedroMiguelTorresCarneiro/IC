#include "LosslessVideo.h"
#include <iostream>

void printUsage() {
    std::cout << "Usage:\n"
              << "./IntraCoding -Video <VIDEO_PATH> -SignMag|-PosNeg [-m <NUM>]\n"
              << "Example:\n"
              << "./IntraCoding -Video \"input.mp4\" -SignMag\n"
              << "./IntraCoding -Video \"input.mp4\" -PosNeg -m 8\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4 || std::string(argv[1]) != "-Video") {
        printUsage();
        return 1;
    }

    std::string videoPath = argv[2];
    std::string codingType = argv[3];
    int mValue = 0;

    if (codingType != "-SignMag" && codingType != "-PosNeg") {
        std::cerr << "Error: Invalid coding type. Use -SignMag or -PosNeg." << std::endl;
        printUsage();
        return 1;
    }

    if (argc == 6 && std::string(argv[4]) == "-m") {
        try {
            mValue = std::stoi(argv[5]);
            if (mValue <= 0) throw std::invalid_argument("mValue must be positive.");
        } catch (...) {
            std::cerr << "Error: Invalid value for m." << std::endl;
            printUsage();
            return 1;
        }
    }

    try {
        LosslessVideo videoEncoder(videoPath, codingType == "-SignMag" ? "SIGN_MAG" : "POS_NEG", mValue);

        // Encode the video
        auto startEncoding = std::chrono::high_resolution_clock::now();
        
        std::string encodedFile = "output_video.dat";
        videoEncoder.processVideo(encodedFile);
        
        auto endEncoding = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> encodingTime = endEncoding - startEncoding;
        std::cout << "Encoding completed in: " << encodingTime.count() << " seconds.\n";


        // Decode and reconstruct the video
        auto startDecoding = std::chrono::high_resolution_clock::now();
        std::string reconstructedFile = "reconstructed_video"; // Start with base name
        std::string originalFormat = videoEncoder.getOriginalFormat(encodedFile); // Retrieve format from header
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

