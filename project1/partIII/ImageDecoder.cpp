#include "ImageDecoder.h" // ------------------- Include the header, not the .cpp file
#include <iostream> // ------------------------- for std::cout
#include <iomanip> // -------------------------- for std::setw
#include <sstream>  // -------------------------- for std::ostringstream
#include <cstdlib>  // -------------------------- for system
#include <fstream>  // Include for file handling

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
        exit(1);
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
    for(int row =0; row < image.rows; row++){ // ------------------------------------- Iterate through each pixel (row)
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
    for (int i = 0; i < 32; ++i) {  // Scale the height of the histogram to 32 lines
        for (int j = 0; j < 256; j += 8) {  // Display every 8th index
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
    for (int i = 0; i < 256; i += 8) {  // Display every 8th index to avoid clutter
        std::cout << std::setw(3) << i << " ";
    }
    std::cout << std::endl;
}


void ImageDecoder::executePythonScript(const std::string& scriptName, const std::string& mode, const std::string& data) {
    // Construct the command to call the Python script
    std::string command = "python3 ../" + scriptName + " " + mode + " \"" + data + "\"";
    system(command.c_str());  // Execute the command
}

// ------------------------------------------------------------------------------------------------
// ---------------------------------| Create Histogram | -----------------------------------------
// ------------------------------------------------------------------------------------------------
void ImageDecoder::createHistogram(const cv::Mat& image) {
    // Check if the image is valid
    if (image.empty()) {
        std::cout << "Error: Image is empty!" << std::endl;
        return;
    }

    int channels = image.channels();

    if (channels == 1) {
        // Grayscale image
        std::cout << "Creating histogram for grayscale image..." << std::endl;

        // Initialize histogram for grayscale (256 bins)
        std::vector<int> histogram(256, 0);

        // Calculate the histogram
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                uchar pixelValue = image.at<uchar>(i, j);
                histogram[pixelValue]++;
            }
        }

        std::ostringstream oss;
        for (int i = 0; i < 256; ++i) {
            oss << histogram[i] << " ";
        }
        executePythonScript("plot_histogram.py", "grayscale", oss.str());

    } else if (channels == 3) {
        // BGR image
        std::cout << "Creating histogram for color image (BGR)..." << std::endl;

        // Initialize histograms for B, G, R channels (each has 256 bins)
        std::vector<int> blueHistogram(256, 0);
        std::vector<int> greenHistogram(256, 0);
        std::vector<int> redHistogram(256, 0);

        // Calculate the histograms for each channel
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
                blueHistogram[pixel[0]]++;
                greenHistogram[pixel[1]]++;
                redHistogram[pixel[2]]++;
            }
        }

        // Write the histograms to a file
        std::ostringstream oss;
        for (int i = 0; i < 256; ++i) {
            oss << blueHistogram[i] << " ";
        }
        oss << ",";  // Separate the channels
        for (int i = 0; i < 256; ++i) {
            oss << greenHistogram[i] << " ";
        }
        oss << ",";  // Separate the channels
        for (int i = 0; i < 256; ++i) {
            oss << redHistogram[i] << " ";
        }

        executePythonScript("plot_histogram.py", "bgr", oss.str());
    } else {
        std::cout << "Error: Unsupported image type!" << std::endl;
    }
}


// ------------------------------------------------------------------------------------------------
// ---------------------------------| Apply Gaussian Blur to the Image | --------------------------
// ------------------------------------------------------------------------------------------------
/*
    Steps to Manually Apply Gaussian Blur to an Image:
        1 - Create a new matrix for the blurred image.
        2 - Create a Gaussian kernel.
        3 - Normalize the kernel.
        4 - Apply the Gaussian blur to each channel (B, G, R) or the grayscale image.
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

    // Check if the image is grayscale or color
    if (image.channels() == 1) {  // Grayscale image
        for (int row = halfSize; row < image.rows - halfSize; ++row) {
            for (int col = halfSize; col < image.cols - halfSize; ++col) {
                double newPixelValue = 0.0;

                // Convolve the Gaussian kernel with the grayscale image
                for (int i = -halfSize; i <= halfSize; i++) {
                    for (int j = -halfSize; j <= halfSize; j++) {
                        uchar pixel = image.at<uchar>(row + i, col + j);
                        newPixelValue += pixel * kernel[i + halfSize][j + halfSize];
                    }
                }

                // Assign the new pixel value to the blurred image
                blurredImage.at<uchar>(row, col) = cv::saturate_cast<uchar>(newPixelValue);
            }
        }
    } else if (image.channels() == 3) {  // Color image (BGR)
        for (int row = halfSize; row < image.rows - halfSize; ++row) {
            for (int col = halfSize; col < image.cols - halfSize; ++col) {
                cv::Vec3d newPixelValue(0, 0, 0);  // Temporary variable to store new pixel value

                // Convolve the Gaussian kernel with the color image (for each color channel)
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
    }

    return blurredImage;  // Return the blurred image
}

// -------------------------------------------------------------------------------------------------
// ---------------------------------| Apply High-Pass Filter | -------------------------------------
// -------------------------------------------------------------------------------------------------
/*
    Steps to Manually Apply a High-Pass Filter to an Image:
        1 - Create a low-pass (blurred) version of the image usign gaussian blur .
        --------------------------------------------------------------------- Already done in the [applyGaussianBlur]
        2 - Create a new matrix to store the high-pass filtered image.
        3 - Subtract the blurred (low-pass) image from the original image to get the high-pass result.
        4 - Ensure the pixel value remains in the valid range [0, 255].
        5 - Return the high-pass filtered image.
*/
cv::Mat ImageDecoder::applyHighPassFilter(const cv::Mat& image) {
    // Define fixed kernel size and sigma for Gaussian blur (low-pass filter)
    int kernelSize = 9;  // Fixed kernel size (must be odd)
    double sigma = 2;    // Fixed sigma value

    // 1. Create a low-pass (blurred) version of the image with proper border handling
    cv::Mat lowPassImage = cv::Mat::zeros(image.size(), image.type());
    //lowPassImage = applyGaussianBlur(image, kernelSize, sigma); // Gaussian blur with border handling
    
    cv::GaussianBlur(image, lowPassImage, cv::Size(kernelSize, kernelSize), sigma, sigma, cv::BORDER_REPLICATE);

    // 2. Create a Mat to store the high-pass filtered image
    cv::Mat highPassImage = cv::Mat::zeros(image.size(), image.type());

    // 3. Subtract the blurred (low-pass) image from the original image to get the high-pass result
    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            if (image.channels() == 1) { // Grayscale image
                int highPassValue = image.at<uchar>(row, col) - lowPassImage.at<uchar>(row, col);
                // Ensure pixel value remains in the valid range [0, 255] with saturate_cast
                highPassImage.at<uchar>(row, col) = cv::saturate_cast<uchar>(highPassValue);
            } else if (image.channels() == 3) { // Color image (BGR)
                cv::Vec3b originalPixel = image.at<cv::Vec3b>(row, col);
                cv::Vec3b blurredPixel = lowPassImage.at<cv::Vec3b>(row, col);
                cv::Vec3b highPassPixel;

                for (int channel = 0; channel < 3; ++channel) {
                    int highPassValue = originalPixel[channel] - blurredPixel[channel];
                    highPassPixel[channel] = cv::saturate_cast<uchar>(highPassValue);
                }

                highPassImage.at<cv::Vec3b>(row, col) = highPassPixel;
            }
        }
    }

    return highPassImage;
}


// ------------------------------------------------------------------------------------------------
// ---------------------------------| Rotate Image | ----------------------------------------------
// ------------------------------------------------------------------------------------------------
/*
    Steps to Manually Rotate an Image:
        1 - Calculate the center of the image.
        2 - Convert the angle to radians.
        3 - Create an output image with the same size and type.
        4 - Iterate over each pixel in the output image.
        5 - Calculate the coordinates of the original pixel after rotation.
        6 - Check if the calculated coordinates are inside the image bounds.
        7 - Assign the pixel value to the rotated image.
        8 - Return the rotated image.
*/
cv::Mat ImageDecoder::rotateImage(const cv::Mat& image, double angleDegrees) {
    printf("Rotating image by %.2f degrees\n", angleDegrees);
    // Calculate the center of the image
    cv::Point2f center(image.cols / 2.0, image.rows / 2.0);
    
    // Convert angle to radians
    double angleRadians = angleDegrees * CV_PI / 180.0;

    // Create an output image
    cv::Mat rotatedImage = cv::Mat::zeros(image.size(), image.type());

    // Iterate over each pixel in the output image
    for (int row = 0; row < rotatedImage.rows; ++row) {
        for (int col = 0; col < rotatedImage.cols; ++col) {
            // Calculate the coordinates of the original pixel
            int x = static_cast<int>((col - center.x) * cos(angleRadians) + 
                                     (row - center.y) * sin(angleRadians) + center.x);
            int y = static_cast<int>(-(col - center.x) * sin(angleRadians) + 
                                     (row - center.y) * cos(angleRadians) + center.y);
            
            // Check if the calculated coordinates are inside the image bounds
            if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                rotatedImage.at<cv::Vec3b>(row, col) = image.at<cv::Vec3b>(y, x);
            }
        }
    }
    return rotatedImage;
    
}


// ------------------------------------------------------------------------------------------------
// ---------------------------------| Invert Colors | ---------------------------------------------
// ------------------------------------------------------------------------------------------------
/*
    Steps to Manually Invert Colors in an Image:
        1 - Create a new matrix to store the inverted image.
        2 - Iterate over each pixel in the image.
        3 - Check if the image is grayscale or RGB.
        4 - Invert the pixel values for each channel (B, G, R).
        5 - Assign the inverted pixel value to the new image.
        6 - Return the inverted image.
*/
cv::Mat ImageDecoder::invertColors(const cv::Mat& image) {
    cv::Mat invertedImage = cv::Mat::zeros(image.size(), image.type());

    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            if (image.channels() == 1) {
                // Grayscale image
                uchar pixel = image.at<uchar>(row, col);
                invertedImage.at<uchar>(row, col) = 255 - pixel;
            } else if (image.channels() == 3) {
                // Color image (BGR)
                cv::Vec3b pixel = image.at<cv::Vec3b>(row, col);
                invertedImage.at<cv::Vec3b>(row, col)[0] = 255 - pixel[0];  // Blue channel
                invertedImage.at<cv::Vec3b>(row, col)[1] = 255 - pixel[1];  // Green channel
                invertedImage.at<cv::Vec3b>(row, col)[2] = 255 - pixel[2];  // Red channel
            }
        }
    }

    return invertedImage;
}




// ------------------------------------------------------------------------------------------------
// ---------------------------------| Calculate Absolute Difference | ----------------------------
// ------------------------------------------------------------------------------------------------
/*
    Steps to Manually Calculate the Absolute Difference between Two Images:
        1 - Check if the input images have the same size and type.
        2 - Create a new matrix to store the result.
        3 - Check if the images are grayscale or RGB.
        4 - Calculate the absolute difference for each pixel.
        5 - Return the image with the calculated differences.
*/
cv::Mat ImageDecoder::calculateAbsoluteDifference(const cv::Mat& image1, const cv::Mat& image2) {
    if (image1.size() != image2.size()) { // ----------------------------------------- Check if the input images have the same size
        std::cout << "Error: Images must have the same size." << std::endl;
        return cv::Mat();
    }

    if (image1.type() != image2.type()) { // ----------------------------------------- Check if the input images have the same type
        std::cout << "Error: Images must have the same type." << std::endl;
        return cv::Mat();
    }

    cv::Mat differenceImage = cv::Mat::zeros(image1.size(), image1.type()); // ------- Create a new matrix to store the result

    if (image1.channels() == 1) { // ------------------------------------------------- Check for GRAYSCALE image
        for (int row = 0; row < image1.rows; ++row) {
            for (int col = 0; col < image1.cols; ++col) {
                uchar pixel1 = image1.at<uchar>(row, col); // ------------------------- Get the pixel value image1
                uchar pixel2 = image2.at<uchar>(row, col); // ------------------------- Get the pixel value image2

                uchar diffPixel = std::abs(pixel1 - pixel2); // ----------------------- Calculate the absolute difference
                differenceImage.at<uchar>(row, col) = diffPixel; // ------------------- Assign the difference to the new image
            }
        }
    } 
    else if (image1.channels() == 3) { // -------------------------------------------- Check for RGB image
        for (int row = 0; row < image1.rows; ++row) {
            for (int col = 0; col < image1.cols; ++col) {
                cv::Vec3b pixel1 = image1.at<cv::Vec3b>(row, col); // ---------------- Get the pixel vector image1 (B, G, R)
                cv::Vec3b pixel2 = image2.at<cv::Vec3b>(row, col); // ---------------- Get the pixel vector image2 (B, G, R)

                cv::Vec3b diffPixel; // ---------------------------------------------- Create a new pixel vector for the differences
                diffPixel[0] = std::abs(pixel1[0] - pixel2[0]); // ------------------- Calculate the absolute difference : BLUE channel
                diffPixel[1] = std::abs(pixel1[1] - pixel2[1]); // ------------------- Calculate the absolute difference : GREEN channel
                diffPixel[2] = std::abs(pixel1[2] - pixel2[2]); // ------------------- Calculate the absolute difference : RED channel

                differenceImage.at<cv::Vec3b>(row, col) = diffPixel; // -------------- Assign the difference vector to the new image
            }
        }
    } 
    else {
        std::cout << "Error: Unsupported number of channels in the image!" << std::endl;
        return cv::Mat();
    }

    return differenceImage; // ------------------------------------------------------ Return the image with the calculated differences
}


// ------------------------------------------------------------------------------------------------
// ---------------------------------| Calculate Mean Squared Error (MSE) | ------------------------
// ------------------------------------------------------------------------------------------------
/*
    Steps to Manually Calculate the Mean Squared Error (MSE) between Two Images:
        1 - Check if the input images have the same size and type.
        2 - Initialize the sum of squared differences.
        3 - Get the absolute difference matrix between the images.
        4 - Calculate the squared difference for each channel (B, G, R).
        --------------------------------------------------------------------- Already done in the [calculateAbsoluteDifference]
        5 - Divide by the total number of pixels and channels.
        6 - Return the MSE value.
*/
double ImageDecoder::calculateMSE(const cv::Mat& image1, const cv::Mat& image2) {
    // ------------------------------------------------------------------------------ Check if the input images have the same size and type
    if (image1.size() != image2.size() || image1.type() != image2.type()) {
        std::cout << "Error: Images must have the same size and type to calculate MSE." << std::endl;
        return -1.0; // ------------------------------------------------------------- Return -1 ERROR
    }

    cv::Mat absoluteDifference = calculateAbsoluteDifference(image1, image2); // ---- Calculate the absolute difference matrix

    double mse = 0.0; // ------------------------------------------------------------ Initialize MSE

    if (absoluteDifference.channels() == 1) { // ------------------------------------ Check for GRAYSCALE image
        for (int row = 0; row < absoluteDifference.rows; ++row) {
            for (int col = 0; col < absoluteDifference.cols; ++col) {
                uchar diff = absoluteDifference.at<uchar>(row, col); // ------------- Get the pixel value
                mse += static_cast<double>(diff * diff); // ------------------------- Calculate the squared difference
            }
        }
    } else if (absoluteDifference.channels() == 3) { // ----------------------------- Check for RGB image
        for (int row = 0; row < absoluteDifference.rows; ++row) {
            for (int col = 0; col < absoluteDifference.cols; ++col) {
                cv::Vec3b diff = absoluteDifference.at<cv::Vec3b>(row, col); // ----- Get the pixel value
                for (int channel = 0; channel < 3; ++channel) { // ------------------ Iterate over each channel
                    mse += static_cast<double>(diff[channel] * diff[channel]); // --- Calculate the squared difference
                }
            }
        }
    } else {
        std::cout << "Unsupported number of channels!" << std::endl;
        return -1.0;
    }

    mse /= (absoluteDifference.total() * absoluteDifference.channels()); // --------- Divide by the total number of pixels and channels

    return mse; // ------------------------------------------------------------------ Return the MSE value
}


// ------------------------------------------------------------------------------------------------
// ---------------------------------| Calculate Peak Signal-to-Noise Ratio (PSNR) | ---------------
// ------------------------------------------------------------------------------------------------
/*
    Steps to Manually Calculate the Peak Signal-to-Noise Ratio (PSNR) between Two Images:
        1 - Calculate the Mean Squared Error (MSE) between the images.
        --------------------------------------------------------------------- Already done in the [calculateMSE]
        2 - Check if the MSE is 0 or negative:
            - MSE == 0: The images are identical, and PSNR is infinite.
            - MSE < 0: PSNR is undefined, because MSE cannot be negative.
        3 - Calculate the PSNR using the formula:
            PSNR = 10 * log10((255^2) / MSE)
        4 - Return the PSNR value.
*/
double ImageDecoder::calculatePSNR(const cv::Mat& image1, const cv::Mat& image2) {
    double mse = calculateMSE(image1, image2);
    if (mse <= 0.0) { // ------------------------------------------------------------ Check if the MSE is 0 or negative
        std::cout << "Error: MSE is 0 or negative!" << std::endl;
        return -1.0; // ------------------------------------------------------------- Return -1 if MSE is 0 or negative, to indicate an error
    }

    double psnr = 10.0 * std::log10((255.0 * 255.0) / mse); // ---------------------- Calculate the PSNR using the MATH FORMULA
    return psnr; // ----------------------------------------------------------------- Return the PSNR value
}


// ------------------------------------------------------------------------------------------------
// ---------------------------------| Image Quantization | ----------------------------------------
// ------------------------------------------------------------------------------------------------
/*
    Steps to Manually Quantize an Image:
        1 - Check if the input image is valid.
        2 - Check if the number of quantization levels is within the range [2, 256].
        3 - Apply quantization to the image based on the number of levels.
        4 - For grayscale images, quantize the intensity values directly.
        5 - For color images, quantize each channel separately.
        6 - Return the quantized image.
*/
cv::Mat ImageDecoder::imageQuantization(const cv::Mat& image, int bits) {
    if (image.empty()) { // --------------------------------------------------------- Check if the input image is valid
        std::cout << "Error: Image is empty!" << std::endl;
        return cv::Mat(); // -------------------------------------------------------- Return an empty image
    }

    // Calculate the number of quantization levels from bits
    int quantizationLevels = std::pow(2, bits);

    if (quantizationLevels < 2 || quantizationLevels > 256) { // -------------------- Assert quantization range of values
        std::cout << "Error: Number of quantization levels must be between 2 and 256!" << std::endl;
        return cv::Mat();  // ------------------------------------------------------- Return an empty image
    }

    cv::Mat quantizedImage; // ------------------------------------------------------ Create a new Mat for the quantized image
    
    // Calculate the step size based on quantization levels
    int stepSize = 256 / quantizationLevels;

    if (image.channels() == 1) { // ------------------------------------------------- Check for GRAYSCALE image
        quantizedImage = cv::Mat::zeros(image.size(), image.type()); // ------------- Create an empty image for quantization

        for (int row = 0; row < image.rows; ++row) {
            for (int col = 0; col < image.cols; ++col) {
                uchar pixelValue = image.at<uchar>(row, col);

                // Apply the quantization formula
                uchar quantizedValue = std::round(pixelValue / stepSize) * stepSize;

                quantizedImage.at<uchar>(row, col) = quantizedValue;
            }
        }
    } else if (image.channels() == 3) { // ------------------------------------------ Check for RGB image
        std::vector<cv::Mat> channels(3); // ---------------------------------------- Create a vector for the 3 channels
        cv::split(image, channels);  // --------------------------------------------- Split the image into 3 BGR channels

        for (int i = 0; i < 3; ++i) { // -------------------------------------------- Iterate over each channel
            for (int row = 0; row < channels[i].rows; ++row) {
                for (int col = 0; col < channels[i].cols; ++col) { 
                    uchar pixelValue = channels[i].at<uchar>(row, col);

                    // Apply the quantization formula
                    uchar quantizedValue = std::round(pixelValue / stepSize) * stepSize;

                    channels[i].at<uchar>(row, col) = quantizedValue; // ------------ Assign the quantized value to the channel
                }
            }
        }

        cv::merge(channels, quantizedImage); // ------------------------------------- Merge the quantized channels back into an image
    } else {
        std::cout << "Error: Unsupported number of channels in the image!" << std::endl;
        return cv::Mat();  // ------------------------------------------------------- Return an empty image
    }

    return quantizedImage; // ------------------------------------------------------- Return the quantized image
}


// Save the image to a file
void ImageDecoder::saveImage() {
    std::cout << "NOT WORKING...\n";
}
