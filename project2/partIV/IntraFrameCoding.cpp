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
    // Check for minimum arguments
    if (argc < 4 || std::string(argv[1]) != "-Video") {
        printUsage();
        return 1;
    }

    std::string videoPath = argv[2];  // Path to input video
    std::string codingType = argv[3]; // Encoding type
    int mValue = 0;                   // Default mValue

    // Validate coding type
    if (codingType != "-SignMag" && codingType != "-PosNeg") {
        std::cerr << "Error: Invalid coding type. Use -SignMag or -PosNeg." << std::endl;
        printUsage();
        return 1;
    }

    // Check if m is provided
    if (argc == 6 && std::string(argv[4]) == "-m") {
        try {
            mValue = std::stoi(argv[5]);
            if (mValue <= 0) throw std::invalid_argument("mValue must be positive.");
        } catch (const std::exception&) {
            std::cerr << "Error: Invalid value for m." << std::endl;
            printUsage();
            return 1;
        }
    }

    try {
        // Initialize the LosslessVideo encoder
        LosslessVideo videoEncoder(
            videoPath,
            codingType == "-SignMag" ? "SIGN_MAG" : "POS_NEG",
            mValue
        );

        // Process the video and generate output
        videoEncoder.processVideo("output_video.dat");

        std::cout << "Video processed successfully. Encoded file: output_video.dat" << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
