#include "ImageDecoder.h" // ----------------- Include the header, not the .cpp file
#include <iostream> // ------------------------- for std::cout

// Constructor
ImageDecoder::ImageDecoder() {
    // Initialization if necessary
}

// Destructor
ImageDecoder::~ImageDecoder() {
    // Cleanup if necessary
}

// ------------------------------------------------------------------------------------------------
// ---------------------------------| Load and decode the image | ---------------------------------
// ------------------------------------------------------------------------------------------------
std::pair<cv::Mat, std::vector<cv::Mat>> ImageDecoder::decodeImage(const char* filename) {
    cv::Mat image = cv::imread(filename); // ---------------------------------------  Use OpenCV to load the image

    if (image.empty()) {
        std::cout << "Error: Could not open or find the image!" << std::endl;
        exit(1); // ----------------------------------------------------------------- Exit if the image is not loaded
    } else {
        std::cout << "Image successfully loaded: " << filename << std::endl;
    }

    std::vector<cv::Mat> channels = splitChannels(image); // ------------------------ Split the image into channels

    return {image, channels}; // ---------------------------------------------------- Return the image and its channels
}

// ----------------------------------------------------------------------------------------------------
// ---------------------------------| Display the image in a window | ---------------------------------
// ----------------------------------------------------------------------------------------------------
void ImageDecoder::displayImage(const char* filename) {
    cv::Mat image = cv::imread(filename); // ------------------------------------------------- Load the image using OpenCV
    
    if (image.empty()) {
        std::cout << "No image loaded to display!" << std::endl;
        return;
    }

    cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE); // ---------------------- Create a window
    cv::imshow("Display Window", image); // ----------------------------------------- Display the image
    std::cout << "Press 'q' to close the image and return to the menu." << std::endl;

    while (true) {
        int key = cv::waitKey(10);  // ---------------------------------------------- Wait for a key press
        if (key == 'q' || key == 113 || key == 27) {  // ---------------------------- Check if 'q' or 'ESC' is pressed
            cv::destroyWindow("Display Window");
            cv::waitKey(1);  // ----------------------------------------------------- Ensure the window is closed
            break; // --------------------------------------------------------------- Go back to the menu
        }
    }
}


// ----------------------------------------------------------------------------------------------------
// ---------------------------------| Split the image into channels | ---------------------------------
// ----------------------------------------------------------------------------------------------------
std::vector<cv::Mat> ImageDecoder::splitChannels(const cv::Mat& image) {
    if(image.empty()){
        std::cout << "No image loaded to split";
        return {};
    }

    std::vector<cv::Mat> channels; // ----------------------------------------------- Vector to store the channels
    cv::split(image, channels); // --------------------------------------------------- Split the image into channels

    return channels; // ------------------------------------------------------------- Return the splited channels
}

// -----------------------------------------------------------------------------------------------------
// ---------------------------------| Convert the image to grayscale | ---------------------------------
// -----------------------------------------------------------------------------------------------------
/*
    Average method: 
        The Average method takes the average value of R, G, and B as the grayscale value. 
            > Grayscale = (R + G + B ) / 3
        
        There may be some overflow error occurred if the sum of R, G, and B is greater than 255. 
        To avoid the exception, R, G, and B should be calculated respectively.
            > Grayscale = R / 3 + G / 3 + B / 3
    
        The average method is simple but doesn’t work as well as expected. 
        The reason is that human eyeballs react differently to RGB. 
        Eyes are most sensitive to green light, less sensitive to red light, 
        and the least sensitive to blue light. Therefore, 
        the three colours should have different weights in the distribution. 
        
    That brings us to the weighted method. 
    Weighted method: 
        The Weighted Method, also called the luminosity method, 
        weighs red, green and blue according to their wavelengths.
        The improved formula is as follows:
            > Grayscale = 0.299R + 0.587G + 0.114B


 */
void ImageDecoder::convertToGrayscale(const std::vector<cv::Mat>& channels) {
    if (channels.size() != 3) {
        std::cout << "Error: Expected 3 channels (B, G, R)." << std::endl;
        return;
    }

    // Ensure all channels are of type CV_8U (unsigned 8-bit integer)
    cv::Mat blueChannel, greenChannel, redChannel;
    channels[0].convertTo(blueChannel, CV_8U);
    channels[1].convertTo(greenChannel, CV_8U);
    channels[2].convertTo(redChannel, CV_8U);

    cv::Mat grayscaleImage = cv::Mat::zeros(blueChannel.size(), CV_8UC1);  // Create an empty grayscale image

    for (int row = 0; row < grayscaleImage.rows; row++) {
        for (int col = 0; col < grayscaleImage.cols; col++) {
            uchar blue = blueChannel.at<uchar>(row, col);   // Blue channel
            uchar green = greenChannel.at<uchar>(row, col);  // Green channel
            uchar red = redChannel.at<uchar>(row, col);    // Red channel

            // Apply the grayscale conversion formula
            uchar gray = static_cast<uchar>(0.299 * red + 0.587 * green + 0.114 * blue);

            // Set the pixel in the grayscale image
            grayscaleImage.at<uchar>(row, col) = gray;
        }
    }

    // Display the grayscale image
    cv::imshow("Manual Grayscale Image", grayscaleImage);
    cv::waitKey(0);  // Wait for a key press
}

// -----------------------------------------------------------------------------------------------------
// ---------------------------------| Get grayscale channels | ---------------------------------
// -----------------------------------------------------------------------------------------------------
std::vector<cv::Mat> ImageDecoder::getGrayscaleChannels(const std::vector<cv::Mat>& channels){


    return {};
}

// Save the image to a file
void ImageDecoder::saveImage() {
    std::cout << "NOT WORKING...\n";
}
