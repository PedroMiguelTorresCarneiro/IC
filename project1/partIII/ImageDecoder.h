#ifndef IMAGE_DECODER_H
#define IMAGE_DECODER_H

#include <opencv2/opencv.hpp> // ------------------------- for OpenCV
#include <vector> // ------------------------------------- for std::vector
#include <utility> // ------------------------------------ for std::pair

class ImageDecoder {
public:
    ImageDecoder();
    ~ImageDecoder();

    // ----------------------------------------------- Load and decode the image, return the image and its channels
    std::pair<cv::Mat, std::vector<cv::Mat>> decodeImage(const char* filename); 

    // ----------------------------------------------- Display the image in a window
    void displayImage(const cv::Mat& image);

    // ----------------------------------------------- Split the image into RGB channels
    std::vector<cv::Mat> splitChannels(const cv::Mat& image);

    // ----------------------------------------------- Convert the image to grayscale
    cv::Mat convertToGrayscale(const std::vector<cv::Mat>& channels);

    // ----------------------------------------------- Display the channels
    void displayChannels(const std::vector<cv::Mat>& colorChannels, const cv::Mat& grayscaleImage);

    // ----------------------------------------------- Create Grayscale Histogram
    void createGrayscaleHistogram(const std::vector<std::vector<uchar>>& grayscaleImage);

    // ----------------------------------------------- Apply Gaussian Blur to the Image
    cv::Mat applyGaussianBlur(const cv::Mat& image, int kernelSize, double sigma);

    // ----------------------------------------------- Save the image
    void saveImage();
private:

};

#endif
