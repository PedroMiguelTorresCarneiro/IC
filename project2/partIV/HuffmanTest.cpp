#include "ImageDecoder.h"
#include "HuffmanCoding.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <map>

int main() {
    // Load an image
    cv::Mat image = cv::imread("airplane.ppm");
    if (image.empty()) {
        std::cerr << "Error: Could not load image!" << std::endl;
        return -1;
    }

    // Create a frequency table from the image
    ImageDecoder decoder;
    std::map<int, int> frequencyTable = decoder.createFrequencyTable(image);

    // Print the frequency table
    std::cout << "Frequency Table:" << std::endl;
    for (const auto& [value, frequency] : frequencyTable) {
        std::cout << "Value: " << value << ", Frequency: " << frequency << std::endl;
    }

    // Initialize Huffman coding
    HuffmanCoding huffman;

    // Build Huffman tree using the frequency table
    huffman.buildTree(frequencyTable);


    // Encode the image data
    std::vector<int> pixelData;
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            if (image.channels() == 1) { // Grayscale image
                pixelData.push_back(image.at<uchar>(y, x));
            } else { // Color image (BGR)
                cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
                pixelData.push_back(pixel[0]); // Blue channel
                pixelData.push_back(pixel[1]); // Green channel
                pixelData.push_back(pixel[2]); // Red channel
            }
        }
    }

    // Encode the pixel data
    std::string encodedData = huffman.encode(pixelData);
    //std::cout << "\nEncoded Data: " << encodedData << std::endl;

    // Decode the data back
    std::vector<int> decodedPixelData = huffman.decode(encodedData);

    // Verify if the decoding matches the original data
    if (pixelData == decodedPixelData) {
        std::cout << "\nDecoding successful! Original data matches decoded data." << std::endl;
    } else {
        std::cerr << "\nDecoding failed! Original data does not match decoded data." << std::endl;
    }

    // Compare the sizes
    size_t originalSize = image.total() * image.elemSize(); // Original size in bytes
    size_t encodedSize = encodedData.size() / 8.0; // Encoded size in bytes
    std::cout << "\nOriginal Image Size: " << originalSize << " bytes" << std::endl;
    std::cout << "Encoded Data Size: " << encodedSize << " bytes" << std::endl;

    return 0;
}
