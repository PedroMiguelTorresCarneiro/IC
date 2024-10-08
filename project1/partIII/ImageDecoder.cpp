#include "ImageDecoder.h" // ------------------- Include the header, not the .cpp file
#include <iostream> // ------------------------- for std::cout
#include <iomanip> // -------------------------- for std::setw

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
void ImageDecoder::displayImage(const cv::Mat& image) {
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
/*
    Steps to Manually Split an RGB Image into Channels:
        1 - Access the pixel data of the image.
        2 - Iterate through each pixel.
        3 - Extract the Red, Green, and Blue values for each pixel and store them in 
            separate matrices or containers for the respective channels.

*/
std::vector<cv::Mat> ImageDecoder::splitChannels(const cv::Mat& image) {
    if(image.empty()){
        std::cout << "No image loaded to split";
        return {};
    }

    // ------------------------------------------------------------------------------- Create empty matrices for each channel
    cv::Mat blueChannel = cv::Mat::zeros(image.size(), CV_8UC1); // ------------------ CV_8UC1: 8-bit unsigned integer, single channel
    cv::Mat greenChannel = cv::Mat::zeros(image.size(), CV_8UC1); 
    cv::Mat redChannel = cv::Mat::zeros(image.size(), CV_8UC1); 
   
    // ------------------------------------------------------------------------------- Access the pixel data of the image
    for(int row =0; row < image.rows; row++){ // --------------------------------------- Iterate through each pixel (row)
        for(int col = 0; col < image.cols; col++){ // -------------------------------- Iterate through each pixel (column)
            cv::Vec3b pixel = image.at<cv::Vec3b>(row, col); // ---------------------- Point to the pixel (row, col)

            // ----------------------------------------------------------------------- Extract the Red, Green, and Blue values
            blueChannel.at<uchar>(row, col) = pixel[0]; // --------------------------- Blue channel
            greenChannel.at<uchar>(row, col) = pixel[1]; // -------------------------- Green channel
            redChannel.at<uchar>(row, col) = pixel[2]; // ---------------------------- Red channel
        }
    }

    return {blueChannel, greenChannel, redChannel}; // ------------------------------- Return the channels
}

// -----------------------------------------------------------------------------------------------------
// ---------------------------------| Convert the image to grayscale | ---------------------------------
// -----------------------------------------------------------------------------------------------------
/*
    Steps to Manually Convert an RGB Image to Grayscale:
        1 - Access the pixel data of the image.
        2 - Iterate through each pixel.
        3 - Extract the Red, Green, and Blue values for each pixel.
        ---------------------------------------------------------------- Already done in the splitChannels function
        4 - Calculate the grayscale value using the formula:
            Gray = 0.299 * Red + 0.587 * Green + 0.114 * Blue
        5 - Set the grayscale value for each pixel in the grayscale image.
        6 - Display the grayscale image.
 */
cv::Mat ImageDecoder::convertToGrayscale(const std::vector<cv::Mat>& channels) {
    if (channels.size() != 3) {
        std::cout << "Error: Expected 3 channels (B, G, R)." << std::endl;
        return cv::Mat();  // Return an empty matrix if error
    }

    // Assign the channels from the vector
    cv::Mat blueChannel = channels[0];
    cv::Mat greenChannel = channels[1];
    cv::Mat redChannel = channels[2];

    // Create an empty grayscale image with the same size as the channels
    cv::Mat grayscaleImage = cv::Mat::zeros(blueChannel.size(), CV_8UC1);  // Create an empty grayscale image

    // Iterate over each pixel and calculate the grayscale value
    for (int row = 0; row < grayscaleImage.rows; ++row) {
        for (int col = 0; col < grayscaleImage.cols; ++col) {
            // Get the pixel values from each channel
            uchar blue = blueChannel.at<uchar>(row, col);
            uchar green = greenChannel.at<uchar>(row, col);
            uchar red = redChannel.at<uchar>(row, col);

            // Apply the grayscale conversion formula
            uchar gray = static_cast<uchar>(0.299 * red + 0.587 * green + 0.114 * blue);

            // Set the grayscale value
            grayscaleImage.at<uchar>(row, col) = gray;
        }
    }

    return grayscaleImage;  // Return the grayscale image
}

// ---------------------------------------------------------------------------------------
// ---------------------------------| Display Channels | ---------------------------------
// ---------------------------------------------------------------------------------------
void ImageDecoder::displayChannels(const std::vector<cv::Mat>& colorChannels, const cv::Mat& grayscaleImage) {
    if (colorChannels.size() != 3) {
        std::cout << "Error: Expected 3 color channels (R, G, B)." << std::endl;
        return;
    }

    // Header
    std::cout << "\n----------------| COLOR CHANNELS |----------------" << std::endl;
    std::cout << "Printing the first 5 rows (representative of the full image)\n";
    std::cout << "R : Red channel\n";
    std::cout << "G : Green channel\n";
    std::cout << "B : Blue channel\n\n";

    // Display pixel values for a few points in the image to compare
    int rowsToPrint = std::min(5, grayscaleImage.rows);  // Limit the number of rows to print
    int colsToPrint = std::min(5, grayscaleImage.cols);  // Limit the number of columns to print

    // Iterate through the selected rows and columns
    for (int row = 0; row < rowsToPrint; ++row) {
        for (int col = 0; col < colsToPrint; ++col) {
            uchar blue = colorChannels[0].at<uchar>(row, col);
            uchar green = colorChannels[1].at<uchar>(row, col);
            uchar red = colorChannels[2].at<uchar>(row, col);
            uchar gray = grayscaleImage.at<uchar>(row, col);

            // Print pixel values for R, G, B, and Grayscale with alignment
            std::cout << std::left;  // Align text to the left
            std::cout << "- Pixel (" << row << "," << col << "): ";
            std::cout << "R: " << std::setw(4) << (int)red << ", ";  // Width 4 for consistency
            std::cout << "G: " << std::setw(4) << (int)green << ", ";
            std::cout << "B: " << std::setw(4) << (int)blue << " | ";
            std::cout << "Gray: " << std::setw(4) << (int)gray;  // Align the Gray column
            std::cout << std::endl;  // New line after each pixel
        }
    }

    std::cout << "\n----------------| END OF CHANNELS |----------------\n";
}


// -----------------------------------------------------------------------------------------
// ---------------------------------| Create Grayscale Histogram | -------------------------
// -----------------------------------------------------------------------------------------
void ImageDecoder::createGrayscaleHistogram(const std::vector<std::vector<uchar>>& grayscaleImage) {
    // Initialize histogram with 256 bins (for values 0 to 255)
    int histogram[256] = {0};

    // Calculate the histogram by counting each pixel's intensity
    for (size_t row = 0; row < grayscaleImage.size(); ++row) {
        for (size_t col = 0; col < grayscaleImage[row].size(); ++col) {
            uchar intensity = grayscaleImage[row][col];
            histogram[intensity]++;
        }
    }

    // Find the maximum value in the histogram (for scaling purposes)
    int maxHistogramValue = 0;
    for (int i = 0; i < 256; ++i) {
        if (histogram[i] > maxHistogramValue) {
            maxHistogramValue = histogram[i];
        }
    }

    // Display histogram horizontally
    std::cout << "\nGrayscale Histogram (Horizontal):\n";

    // Print bars
    for (int i = 0; i < 32; ++i) {  // Scale the height of the histogram to 50 lines
        for (int j = 0; j < 256; j += 8) {  // Display every 5th index
            int barHeight = static_cast<int>(32.0 * histogram[j] / maxHistogramValue);
            if (barHeight >= 32 - i) {
                std::cout << " *  ";
            } else {
                std::cout << "    ";
            }
        }
        std::cout << std::endl;
    }

    // Print intensity labels (0 to 255)
    for (int i = 0; i < 256; i += 8) {  // Display every 16th index to avoid clutter
        std::cout << std::setw(3) << i << " ";
    }
    std::cout << std::endl;
}

// ------------------------------------------------------------------------------------------------
// ---------------------------------| Apply Gaussian Blur to the Image | --------------------------
// ------------------------------------------------------------------------------------------------
/*
    Steps to Manually Apply Gaussian Blur to an Image:
        1 - Create a new matrix for the blurred image.
        2 - Create a Gaussian kernel.
        3 - Normalize the kernel.
        4 - Apply the Gaussian blur to each channel (B, G, R).
        5 - Convolve the Gaussian kernel with the image.
        6 - Assign the new pixel value to the blurred image.
        7 - Return the blurred image.
 */
cv::Mat ImageDecoder::applyGaussianBlur(const cv::Mat& image, int kernelSize, double sigma) {
    // Create a new Mat for the blurred image
    cv::Mat blurredImage = cv::Mat::zeros(image.size(), image.type());

    // Create Gaussian kernel
    int halfSize = kernelSize / 2;
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double sum = 0.0;

    // Generate the Gaussian kernel
    for (int i = -halfSize; i <= halfSize; i++) {
        for (int j = -halfSize; j <= halfSize; j++) {
            kernel[i + halfSize][j + halfSize] = (1 / (2 * M_PI * sigma * sigma)) * 
                exp(-(i * i + j * j) / (2 * sigma * sigma));
            sum += kernel[i + halfSize][j + halfSize];
        }
    }

    // Normalize the kernel (so that the sum of its elements equals 1)
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] /= sum;
        }
    }

    // Apply Gaussian blur to each channel (B, G, R)
    for (int row = halfSize; row < image.rows - halfSize; ++row) {
        for (int col = halfSize; col < image.cols - halfSize; ++col) {
            cv::Vec3d newPixelValue(0, 0, 0);  // Temporary variable to store new pixel value

            // Convolve the Gaussian kernel with the image (for each color channel)
            for (int i = -halfSize; i <= halfSize; i++) {
                for (int j = -halfSize; j <= halfSize; j++) {
                    cv::Vec3b pixel = image.at<cv::Vec3b>(row + i, col + j);
                    newPixelValue[0] += pixel[0] * kernel[i + halfSize][j + halfSize]; // Blue
                    newPixelValue[1] += pixel[1] * kernel[i + halfSize][j + halfSize]; // Green
                    newPixelValue[2] += pixel[2] * kernel[i + halfSize][j + halfSize]; // Red
                }
            }

            // Assign the new pixel value to the blurred image
            blurredImage.at<cv::Vec3b>(row, col) = newPixelValue;
        }
    }

    return blurredImage;  // Return the blurred image
}



// Save the image to a file
void ImageDecoder::saveImage() {
    std::cout << "NOT WORKING...\n";
}
