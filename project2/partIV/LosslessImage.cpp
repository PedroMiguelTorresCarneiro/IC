#include "LosslessImage.h"
#include <iostream>
#include <cmath>

// Constructor: Load image and determine its properties
LosslessImage::LosslessImage(const std::string& filePath) {
    ImageDecoder decoder;
    auto [loadedImage, splitChannels] = decoder.decodeImage(filePath.c_str());

    image = loadedImage;
    channels = splitChannels;

    width = image.cols;
    height = image.rows;

    // Determine if the image is grayscale or color
    imageType = (channels.size() == 1) ? 0 : 1; // 0 for grayscale, 1 for color

    // Extract format from file extension
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos != std::string::npos) {
        imageFormat = filePath.substr(dotPos + 1); // Get the extension (e.g., "ppm", "jpeg")
    } else {
        imageFormat = "unknown"; // Default if no extension is found
    }
}


// Decode image and calculate residuals
void LosslessImage::decodeImage() {
    if (image.empty()) {
        throw std::runtime_error("Image is not loaded.");
    }

    calculateResiduals();
}

// Median Edge Predictor
/*
int LosslessImage::medianEdgePredictor(const cv::Mat& img, int x, int y) {
    int A = (x > 0) ? img.at<uchar>(y, x - 1) : 0;     // Left neighbor
    int B = (y > 0) ? img.at<uchar>(y - 1, x) : 0;     // Top neighbor
    int C = (x > 0 && y > 0) ? img.at<uchar>(y - 1, x - 1) : 0; // Top-left neighbor

    if (C >= std::max(A, B))
        return std::min(A, B);
    else if (C <= std::min(A, B))
        return std::max(A, B);
    else
        return A + B - C;
}
*/
// Median Edge Predictor
int LosslessImage::medianEdgePredictor(const cv::Mat& img, int x, int y, int c) {
    int A, B, C;

    if (c == -1) { // Grayscale
        A = (x > 0) ? img.at<uchar>(y, x - 1) : 0;     // Left neighbor
        B = (y > 0) ? img.at<uchar>(y - 1, x) : 0;     // Top neighbor
        C = (x > 0 && y > 0) ? img.at<uchar>(y - 1, x - 1) : 0; // Top-left neighbor
    } else { // Color channel
        const cv::Vec3b& pixelLeft = (x > 0) ? img.at<cv::Vec3b>(y, x - 1) : cv::Vec3b(0, 0, 0);
        const cv::Vec3b& pixelTop = (y > 0) ? img.at<cv::Vec3b>(y - 1, x) : cv::Vec3b(0, 0, 0);
        const cv::Vec3b& pixelTopLeft = (x > 0 && y > 0) ? img.at<cv::Vec3b>(y - 1, x - 1) : cv::Vec3b(0, 0, 0);

        A = pixelLeft[c];
        B = pixelTop[c];
        C = pixelTopLeft[c];
    }

    if (C >= std::max(A, B))
        return std::min(A, B);
    else if (C <= std::min(A, B))
        return std::max(A, B);
    else
        return A + B - C;
}



// Calculate residuals
void LosslessImage::calculateResiduals() {
    if (imageType == 1) { // Color image
        residuals = cv::Mat(image.size(), CV_32SC(channels.size()));
        for (size_t i = 0; i < channels.size(); ++i) {
            for (int y = 0; y < channels[i].rows; ++y) {
                for (int x = 0; x < channels[i].cols; ++x) {
                    int actual = channels[i].at<uchar>(y, x);
                    int predicted = medianEdgePredictor(channels[i], x, y);
                    residuals.at<cv::Vec<int, 3>>(y, x)[i] = actual - predicted;
                }
            }
        }
    } else { // Grayscale image
        residuals = cv::Mat(image.size(), CV_32S);
        for (int y = 0; y < image.rows; ++y) {
            for (int x = 0; x < image.cols; ++x) {
                int actual = image.at<uchar>(y, x);
                int predicted = medianEdgePredictor(image, x, y);
                residuals.at<int>(y, x) = actual - predicted;
            }
        }
    }
}

// Write header to BitStream
void LosslessImage::writeHeader(BitStream& stream) {
    stream.writeBits(imageType, 8); // Image type: 0 or 1
    stream.writeBits(1, 8);         // Golomb coding type (fixed as POS_NEG for now)
    stream.writeBits(optimalM, 32); // Optimal m
    stream.writeBits(width, 32);    // Image width
    stream.writeBits(height, 32);   // Image height

    // Write image format
    for (char ch : imageFormat) {
        stream.writeBits(static_cast<int>(ch), 8); // Write each character of the format
    }

    stream.writeBits('#', 8); // End of header delimiter
}


// Read header from BitStream
void LosslessImage::readHeader(BitStream& stream) {
    imageType = stream.readBits(8);
    int golombType = stream.readBits(8); // Read Golomb coding type (unused here)
    optimalM = stream.readBits(32);
    width = stream.readBits(32);
    height = stream.readBits(32);

    // Read image format until the delimiter '#'
    imageFormat.clear();
    char ch;
    while ((ch = static_cast<char>(stream.readBits(8))) != '#') {
        imageFormat += ch;
    }
}


// Encode residuals using GolombCoding
void LosslessImage::encodeResiduals(const std::string& outputFile) {
    // Calculate optimal m
    double sum = 0;
    int count = width * height * (imageType == 1 ? 3 : 1);

    for (int y = 0; y < residuals.rows; ++y) {
        for (int x = 0; x < residuals.cols; ++x) {
            if (imageType == 1) {
                for (int c = 0; c < 3; ++c) {
                    sum += std::abs(residuals.at<cv::Vec<int, 3>>(y, x)[c]);
                }
            } else {
                sum += std::abs(residuals.at<int>(y, x));
            }
        }
    }

    double mean = sum / count;
    optimalM = 1;
    while (optimalM < mean) {
        optimalM *= 2;
    }

    BitStream writeStream(outputFile, true);

    // Write header
    writeHeader(writeStream);

    // Encode residuals
    GolombCoding encoder(optimalM, writeStream, GolombCoding::POS_NEG);

    for (int y = 0; y < residuals.rows; ++y) {
        for (int x = 0; x < residuals.cols; ++x) {
            if (imageType == 1) {
                for (int c = 0; c < 3; ++c) {
                    encoder.encode(residuals.at<cv::Vec<int, 3>>(y, x)[c]);
                }
            } else {
                encoder.encode(residuals.at<int>(y, x));
            }
        }
    }

    writeStream.close();
}


// Reconstruct the image
cv::Mat LosslessImage::reconstructImage(const std::string& encodedFile) {
    BitStream readStream(encodedFile, false);

    // Read header
    readHeader(readStream);

    cv::Mat reconstructed;
    if (imageType == 1) { // Color image
        // Create a 3-channel matrix directly
        reconstructed = cv::Mat(height, width, CV_8UC3);
        GolombCoding decoder(optimalM, readStream, GolombCoding::POS_NEG);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                cv::Vec3b& pixel = reconstructed.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; ++c) {
                    int residual = decoder.decode();
                    
                    // Modify the predictor call to work with a 3-channel matrix
                    int predicted = medianEdgePredictor(reconstructed, x, y, c);
                    int value = predicted + residual;
                    
                    pixel[c] = std::clamp(value, 0, 255);
                }
            }
        }
    } else { // Grayscale image
        reconstructed = cv::Mat(height, width, CV_8UC1);
        GolombCoding decoder(optimalM, readStream, GolombCoding::POS_NEG);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int residual = decoder.decode();
                int predicted = medianEdgePredictor(reconstructed, x, y);
                int value = predicted + residual;
                reconstructed.at<uchar>(y, x) = std::clamp(value, 0, 255);
            }
        }
    }

    return reconstructed;
}



// Save residual image for debugging
void LosslessImage::saveResidualImage(const std::string& filePath) {
    cv::Mat displayResiduals;
    double minVal, maxVal;
    cv::minMaxLoc(residuals, &minVal, &maxVal);
    residuals.convertTo(displayResiduals, CV_8U, 255.0 / (maxVal - minVal), -minVal);
    cv::imwrite(filePath, displayResiduals);
}
