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
    // Constructor for images
    LosslessImage(const std::string& filePath);

    // Constructor for videos
    LosslessImage(const std::string& videoPath, bool isVideo);


    // Main functions
    void decodeImage();
    void calculateResiduals();
    void encodeResiduals(const std::string& outputFile, const std::string& codingType, int mValue);
    cv::Mat reconstructImage(const std::string& encodedFile);
    void saveResidualImage(const std::string& filePath);
    std::string getImageFormat() const { return imageFormat; }

    // Video processing functions
    void loadVideo(const std::string& videoFilePath);
    void decodeVideo();
    void encodeVideoResiduals(const std::string& outputFile);
    void setFrame(const cv::Mat& frame);


private:
    // Helper functions
    void writeHeader(BitStream& stream, const std::string& codingType);
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

    cv::VideoCapture video;        // For video processing
    std::vector<cv::Mat> residualFrames; // Store residuals for each frame
    int frameCount;          // Number of frames (for video)
    std::string videoFormat; // Format (e.g., "mp4")
    int totalFrames;               // Total number of frames in the video
    bool isVideo;                  // Flag to differentiate between image and video
    int calculateOptimalM(const cv::Mat& residuals);

};

#endif // LOSSLESSIMAGE_H
