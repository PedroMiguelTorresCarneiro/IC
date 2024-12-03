#include "ArithmeticEncoding.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <map>

int main() {
    ArithmeticEncoding encoder;

    // Load an image
    cv::Mat image = cv::imread("residuals.png", cv::IMREAD_GRAYSCALE); // Load as grayscale
    if (image.empty()) {
        std::cerr << "Error: Could not load image!" << std::endl;
        return -1;
    }

    // Flatten the image into a vector
    std::vector<int> pixelData;
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            pixelData.push_back(image.at<uchar>(y, x)); // Add pixel intensity
        }
    }

    // Calculate the original size
    size_t originalSizeBytes = pixelData.size() * sizeof(int); // Size in bytes
    std::cout << "Original size: " << originalSizeBytes << " bytes" << std::endl;

    // Step 1: Compute frequency table
    std::map<int, int> frequencyTable = encoder.computeFrequencyTable(pixelData);

    // Step 2: Build probability ranges
    std::map<int, ArithmeticEncoding::ProbabilityRange> probabilities =
        encoder.buildProbabilityRanges(frequencyTable, pixelData.size());

    // Step 3: Print the probability ranges
    //encoder.printProbabilityRanges(probabilities);

    // Step 4: Encode the pixel data
    double encodedValue = encoder.encode(pixelData, probabilities);
    std::cout << "Encoded Value: " << encodedValue << std::endl;

    // Calculate the size of the encoded data
    size_t encodedSizeBytes = sizeof(encodedValue); // Encoded value is a double
    std::cout << "Encoded size: " << encodedSizeBytes << " bytes" << std::endl;

    // Step 5: Decode the pixel data
    std::vector<int> decodedPixelData = encoder.decode(encodedValue, probabilities, pixelData.size());
    std::cout << "Decoded size: " << decodedPixelData.size() * sizeof(int) << " bytes" << std::endl;

    // Verify the result
    if (pixelData == decodedPixelData) {
        std::cout << "Decoding successful! Decoded data matches original.\n";
    } else {
        std::cerr << "Decoding failed! Decoded data does not match original.\n";
    }

    return 0;
}
