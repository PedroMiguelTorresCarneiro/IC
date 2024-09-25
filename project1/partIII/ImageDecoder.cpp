#include <opencv2/opencv.hpp>
#include <iostream>

class ImageDecoder {
public:
    // Constructor
    ImageDecoder() {
        // Initialization if necessary
    }

    // Destructor
    ~ImageDecoder() {
        // Cleanup if necessary
    }

    // Load and decode image
    void decodeImage(const char* filename) {
        image = cv::imread(filename);  // Use OpenCV to load the image

        if (image.empty()) {
            std::cout << "Error: Could not open or find the image!" << std::endl;
            exit(1);  // Exit if the image is not loaded
        } else {
            std::cout << "Image successfully loaded: " << filename << std::endl;
        }
    }

    // Display the image in a window
    void displayImage() {
        if (image.empty()) {
            std::cout << "No image loaded to display!" << std::endl;
            return;
        }
        cv::imshow("Display Window", image);  // Show the image in a window
        cv::waitKey(0);  // Wait for a keystroke in the window
    }

    // Save the image to a file
    void saveImage(const char* filename) {
        if (image.empty()) {
            std::cout << "No image loaded to save!" << std::endl;
            return;
        }
        cv::imwrite(filename, image);  // Save the image to the specified path
        std::cout << "Image successfully saved to " << filename << std::endl;
    }

private:
    cv::Mat image;  // OpenCV image object
};
