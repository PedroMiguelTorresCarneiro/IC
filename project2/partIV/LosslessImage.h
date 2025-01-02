#ifndef LOSSLESSIMAGE_H
#define LOSSLESSIMAGE_H

#include "ImageDecoder.h"
#include "GolombCoding.h"
#include "BitStream.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class LosslessImage {
public:
    // Constructor
    LosslessImage(const std::string& filePath);

    // Main functions
    void decodeImage();
    void calculateResiduals();
    void encodeResiduals(const std::string& outputFile);
    cv::Mat reconstructImage(const std::string& encodedFile);
    void saveResidualImage(const std::string& filePath);
    std::string getImageFormat() const { return imageFormat; }

private:
    // Helper functions
    void writeHeader(BitStream& stream);
    void readHeader(BitStream& stream);

    // Prediction function (supports both grayscale and color)
    int medianEdgePredictor(const cv::Mat& img, int x, int y, int c = -1);
    std::string imageFormat; // To store the original image format (e.g., "PPM", "JPEG")
    cv::Mat grayscaleImage;       // Grayscale image for encoding

    // Member variables
    cv::Mat image;                 // Original image
    std::vector<cv::Mat> channels; // Split channels of the image
    cv::Mat residuals;             // Residuals matrix
    int imageType;                 // 0 for grayscale, 1 for color
    int optimalM;                  // Optimal value for Golomb coding
    int width;                     // Image width
    int height;                    // Image height
};

#endif // LOSSLESSIMAGE_H
