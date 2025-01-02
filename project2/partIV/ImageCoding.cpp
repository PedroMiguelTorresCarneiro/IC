#include "LosslessImage.h"
#include <iostream>
#include <opencv2/opencv.hpp>

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
