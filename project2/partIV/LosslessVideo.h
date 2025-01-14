#ifndef LOSSLESS_VIDEO_H
#define LOSSLESS_VIDEO_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp> // For OpenCV video and image processing
#include "GolombCoding.h"
#include "BitStream.h"

class LosslessVideo {
public:
    // Constructor
    LosslessVideo(const std::string& filePath, const std::string& codingType, int mValue = 0);

    // Main processing function
    void processVideo(const std::string& outputFile);
    std::string getOriginalFormat(const std::string& encodedFile);
    void reconstructVideo(const std::string& encodedFile, const std::string& outputFile);

private:
    // Video-related attributes
    std::string videoPath;
    std::string format;     // File format (e.g., mp4)
    std::string codingType; // Encoding type (SIGN_MAG or POS_NEG)
    int optimalM;           // Optimal value for Golomb parameter m
    int width;              // Frame width
    int height;             // Frame height
    int frameCount;         // Total number of frames
    int mValue;             // User-defined m value (0 if automatic calculation is required)
    
    // Residual data storage
    std::vector<cv::Mat> residuals;

    // Helper functions
    void calculateResiduals(const cv::Mat& frame, cv::Mat& residual);
    int medianEdgePredictor(const cv::Mat& frame, int x, int y, int c);
    //int calculateOptimalM(const std::vector<cv::Mat>& residuals);
    int calculateOptimalMFromVideo(cv::VideoCapture& video);
    void writeHeader(BitStream& bitStream);
    void encodeResiduals(const cv::Mat& residual, BitStream& bitStream);
    std::string extractFormat(const std::string& filePath);
    cv::Mat reconstructFrame(const cv::Mat& residual);
    void readHeader(BitStream& bitStream);

};

#endif // LOSSLESS_VIDEO_H
