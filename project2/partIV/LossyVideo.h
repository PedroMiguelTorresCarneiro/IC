#ifndef LOSSY_VIDEO_H
#define LOSSY_VIDEO_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "GolombCoding.h"
#include "BitStream.h"

class LossyVideo {
public:
    LossyVideo(const std::string& filePath, const std::string& codingType, int mValue = 0, int qLevel = 1);
    void processVideo(const std::string& outputFile, int quantizationBits);
    void reconstructVideo(const std::string& inputFile, const std::string& outputFile);
    std::string getOriginalFormat(const std::string& encodedFile);

private:
    std::string videoPath;
    std::string format;
    std::string codingType;
    int optimalM;
    int quantizationLevel;
    int width;
    int height;
    int frameCount;
    int mValue;

    void loadVideo();
    void calculateResiduals(const cv::Mat& frame, cv::Mat& residuals);
    cv::Mat quantizeResiduals(const cv::Mat& residuals, int bits);
    cv::Mat reconstructFrame(const cv::Mat& residual);
    void writeHeader(BitStream& bitStream);
    void readHeader(BitStream& bitStream);
    void encodeResiduals(const cv::Mat& residuals, BitStream& bitStream);
    void decodeResiduals(BitStream& bitStream, cv::Mat& residuals);
    std::string extractFormat(const std::string& filePath);
    int calculateOptimalMFromVideo(cv::VideoCapture& video);
    int medianEdgePredictor(const cv::Mat& frame, int x, int y, int c);
};

#endif // LOSSY_VIDEO_H
