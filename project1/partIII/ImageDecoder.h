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
    void displayImage(const char* filename);

    // ----------------------------------------------- Split the image into RGB channels
    std::vector<cv::Mat> splitChannels(const cv::Mat& image);

    // ----------------------------------------------- Convert the image to grayscale
    void convertToGrayscale(const std::vector<cv::Mat>& channels);

    // ----------------------------------------------- Save the image
    void saveImage();
private:

};

#endif
