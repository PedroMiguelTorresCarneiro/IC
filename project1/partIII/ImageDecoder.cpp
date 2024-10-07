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

// Save the image to a file
void ImageDecoder::saveImage() {
    std::cout << "NOT WORKING...\n";
}
