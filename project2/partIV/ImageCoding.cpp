#include "LosslessImage.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <stdexcept>

void printUsage() {
    std::cout << "Usage:\n"
              << "-ImgCoding -load <IMAGE_PATH> -SignMag|-PosNeg [-m <NUM>]\n"
              << "Example:\n"
              << "-ImgCoding -load \"../.../img.ppm\" -SignMag\n"
              << "-ImgCoding -load \"../.../img.ppm\" -PosNeg -m 6\n";
}

/*
int main() {
    try {
        // Input and output file paths
        std::string inputImage = "boat.ppm";
        std::string encodedFile = "encoded_residuals.dat";
        std::string residualImage = "residuals.png";
        std::string reconstructedImageFile = "reconstructed.png";

        // Initialize lossless image processing
        std::cout << "Initializing lossless image processing for: " << inputImage << std::endl;
        LosslessImage losslessImage(inputImage);
        std::cout << "Image loaded successfully." << std::endl;

        // Decode image and calculate residuals
        std::cout << "Decoding image and calculating residuals..." << std::endl;
        losslessImage.decodeImage();
        std::cout << "Image decoded and residuals calculated." << std::endl;

        // Save residuals as an image for debugging purposes
        std::cout << "Saving residuals to file: " << residualImage << std::endl;
        losslessImage.saveResidualImage(residualImage);
        std::cout << "Residuals saved successfully." << std::endl;

        // Encode residuals using Golomb coding
        std::cout << "Encoding residuals and saving to file: " << encodedFile << std::endl;
        losslessImage.encodeResiduals(encodedFile);
        std::cout << "Residuals encoded and saved successfully." << std::endl;

        // Reconstruct the image from residuals
        std::cout << "Reconstructing the image from residuals..." << std::endl;
        cv::Mat reconstructedImage = losslessImage.reconstructImage(encodedFile);
        // Save the reconstructed image in the original format
        std::string reconstructedFile = "reconstructed." + losslessImage.getImageFormat();
        if (!cv::imwrite(reconstructedFile, reconstructedImage)) {
            std::cerr << "Error saving reconstructed image to: " << reconstructedFile << std::endl;
        } else {
            std::cout << "Reconstructed image saved to: " << reconstructedFile << std::endl;
        }

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
*/

int main(int argc, char* argv[]) {
    try {
        if (argc < 4 || std::string(argv[1]) != "-ImgCoding" || std::string(argv[2]) != "-load") {
            printUsage();
            return 1;
        }

        // Parse arguments
        std::string imagePath = argv[3];
        std::string codingType;
        int mValue = 0;

        if (argc >= 5) {
            codingType = argv[4];
            if (codingType != "-SignMag" && codingType != "-PosNeg") {
                std::cerr << "Error: Invalid coding type. Use -SignMag or -PosNeg.\n";
                printUsage();
                return 1;
            }
        } else {
            std::cerr << "Error: Coding type is required.\n";
            printUsage();
            return 1;
        }

        if (argc == 7 && std::string(argv[5]) == "-m") {
            try {
                mValue = std::stoi(argv[6]);
                if (mValue <= 0) {
                    throw std::invalid_argument("mValue must be positive.");
                }
            } catch (const std::exception&) {
                std::cerr << "Error: Invalid value for m.\n";
                printUsage();
                return 1;
            }
        } else if (argc > 5) {
            std::cerr << "Error: Unexpected arguments.\n";
            printUsage();
            return 1;
        }

        // Initialize lossless image processing
        std::cout << "Initializing lossless image processing for: " << imagePath << std::endl;
        LosslessImage losslessImage(imagePath);
        std::cout << "Image loaded successfully." << std::endl;

        // Decode image and calculate residuals
        std::cout << "Decoding image and calculating residuals..." << std::endl;
        losslessImage.decodeImage();
        std::cout << "Image decoded and residuals calculated." << std::endl;

        // Encode residuals using specified parameters
        std::string encodedFile = "encoded_residuals.dat";
        losslessImage.encodeResiduals(encodedFile, codingType, mValue);
        std::cout << "Residuals encoded and saved to: " << encodedFile << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}