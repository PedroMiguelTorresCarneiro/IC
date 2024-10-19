#include "ImageDecoder.h"  // -------------------- Include the header, not the .cpp file
#include <filesystem> // ------------------------- for std::filesystem
#include <iostream> // --------------------------- for std::cout
#include <map> // -------------------------------- for std::map
#include <string> // ----------------------------- for std::string
#include <vector> // ----------------------------- for std::vector

namespace fs = std::filesystem; // --------------- Namespace alias for std::filesystem

// ----------------------------------------------- Dictionary-like structure to store image details (name, path, image, split channels)
std::map<std::string, std::pair<std::string, std::pair<cv::Mat, std::vector<cv::Mat>>>> imageStore;
// dictionary structure:
//      imageStore["my_image"] = { "../dataset/my_image.ppm", {image, channels} };
//


/*
// --------------------------------------------------------------------------------
// ---------------------------------| Show Menu | ---------------------------------
// --------------------------------------------------------------------------------
void showMenu(const std::string& loadedImage) {
    std::cout << "\n----- IMAGE PROCESSING MENU -----" << std::endl;
    if (!loadedImage.empty()) {
        std::cout << "\n----| LOADED Image : " << loadedImage << "|----\n" << std::endl;  // Display the loaded image name
    }
    std::cout << "1. Load Image" << std::endl;
    std::cout << "2. Display image" << std::endl;
    std::cout << "3. Display image in grey scale" << std::endl;
    std::cout << "4. Display RBG and Grey Scale Channels" << std::endl;
    std::cout << "5. Histogram of grayscale" << std::endl;
    std::cout << "6. Apply Gaussian blur filter" << std::endl;
    std::cout << "7. Calculate the absolute difference between boat.ppm and girl.ppm and display the resulting image" << std::endl;
    std::cout << "8. Calculate the Mean Squared Error (MSE) and Peak Signal-to-Noise Ratio (PSNR) between boat.ppm and girl.ppm" << std::endl;
    std::cout << "9. Quantization of the image boat.ppm and after dysplay the quantized image" << std::endl;
    std::cout << "10. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}
*/

/*
// ---------------------------------------------------------------------------------------------
// ---------------------------------| List Images in Dataset | ---------------------------------
// ---------------------------------------------------------------------------------------------
std::vector<std::string> listImagesInDataset(const std::string& datasetPath) {
    std::vector<std::string> images;
    for (const auto& entry : fs::directory_iterator(datasetPath)) {
        if (entry.path().extension() == ".ppm") {
            images.push_back(entry.path().filename().stem());  // -------------------------- Add the image name to the list
        }
    }
    
    return images; // ---------------------------------------------------------------------- Return the list of images with .ppm extension
}
*/

/*

// -------------------------------------------------------------------------------------
// ---------------------------------| Get Image Path | ---------------------------------
// -------------------------------------------------------------------------------------
std::string getImagePath(std::string& imageName) {
    std::string datasetPath = "../../../datasets/image/"; // ----------------------------------------- Path to the dataset

    std::vector<std::string> images = listImagesInDataset(datasetPath); // -------------- List images in the dataset

    if (images.empty()) {
        std::cout << "No images found in the dataset directory.\n";
        return "";  // ------------------------------------------------------------------ Return empty string if no images found
    }

    std::cout << "\nAvailable images in the dataset:\n"; // ----------------------------- Display the list of images
    for (size_t i = 0; i < images.size(); ++i) {
        std::cout << i + 1 << ". " << images[i] << std::endl;
    }

    int choice;
    std::cout << "Select an image by number: ";  // ------------------------------------ Ask the user to select an image from the list
    std::cin >> choice; 

    if (choice < 1 || choice > images.size()) { 
        std::cout << "Invalid selection.\n";
        return "";  // ----------------------------------------------------------------- Return empty string if invalid selection
    }

    imageName = images[choice - 1]; 
    std::string imagePath = datasetPath + imageName + ".ppm";

    return imagePath; // --------------------------------------------------------------- Return the path to the selected image
}
*/

// Function to parse args for commands starting with "-load"
std::vector<std::vector<std::string>> parseARG(std::vector<std::string> args) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> loadArgs;

    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == "-load") {
            loadArgs.push_back(args[i]);
            loadArgs.push_back(args[++i]);  // The path to the image
        } else if (args[i] == "-grayscale" || args[i] == "-gaussian" || args[i] == "-quantization" ||
                   args[i] == "-channels" || args[i] == "-highPass" || args[i] == "-rotate" || args[i] == "-invert") {
            loadArgs.push_back(args[i]);

            // If -gaussian or -quantization or -rotate requires values, parse them
            if (args[i] == "-gaussian") {
                loadArgs.push_back(args[++i]);  // Kernel size
                loadArgs.push_back(args[++i]);  // Sigma
            } else if (args[i] == "-quantization") {
                loadArgs.push_back(args[++i]);  // Quantization level
            } else if (args[i] == "-rotate") {
                loadArgs.push_back(args[++i]);  // Angle
            }
        } else if (args[i] == "-display") {
            loadArgs.push_back(args[i]);
        }
    }

    result.push_back(loadArgs);
    return result;
}

// Function to parse args for commands starting with "-mse", "-psnr", "-diff", or "-hist"
std::vector<std::vector<std::string>> parseCOMMAND(std::vector<std::string> args) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> command;
    std::vector<std::string> image1Args;
    std::vector<std::string> image2Args;
    std::vector<std::string> displayArgs = {"-display", "false"};  // Default display flag to false
    bool foundComma = false;

    // First argument is the command (-mse, -psnr, -diff, or -hist)
    command.push_back(args[0]);

    for (size_t i = 1; i < args.size(); i++) {
        if (args[i] == "(" || args[i] == ")") continue;

        if (args[i] == ",") {
            foundComma = true;  // Found the separator between two images
        } else if (args[i] == "-display") {
            // Handle display and set it to true
            displayArgs[1] = "true";
        } else if (!foundComma) {
            image1Args.push_back(args[i]);  // Collect args for the first image
        } else {
            image2Args.push_back(args[i]);  // Collect args for the second image
        }
    }

    result.push_back(command);
    result.push_back(image1Args);

    if (!image2Args.empty()) {
        result.push_back(image2Args);
    }

    // Always include the display flag (either true or false)
    result.push_back(displayArgs);

    return result;
}


cv::Mat applyTransformations(cv::Mat image, const std::vector<std::string>& transformations) {
    ImageDecoder decoder;  // Assume you have an instance of ImageDecoder class to handle operations
    bool imageLoaded = false;  // Track whether the image has been loaded

    for (size_t i = 0; i < transformations.size(); ++i) {
        if (transformations[i] == "-load") {
            std::string imagePath = transformations[++i];
            image = cv::imread(imagePath);  // Load the image
            if (image.empty()) {
                std::cerr << "Error: Could not load image from path: " << imagePath << std::endl;
                return image;
            }
            imageLoaded = true;  // Set the flag to indicate the image has been loaded
            std::cout << "Loaded image from: " << imagePath << std::endl;
        } 
        else if (transformations[i] == "-grayscale") {
            if (!imageLoaded) {
                std::cerr << "Error: Image must be loaded before applying transformations" << std::endl;
                return image;
            }
            std::cout << "Applying grayscale transformation" << std::endl;
            image = decoder.convertToGrayscale(decoder.splitChannels(image));
        } 
        else if (transformations[i] == "-gaussian") {
            if (!imageLoaded) {
                std::cerr << "Error: Image must be loaded before applying transformations" << std::endl;
                return image;
            }
            int kernelSize = std::stoi(transformations[++i]);
            double sigma = std::stod(transformations[++i]);
            std::cout << "Applying gaussian blur with kernel size: " << kernelSize << " and sigma: " << sigma << std::endl;
            image = decoder.applyGaussianBlur(image, kernelSize, sigma);
        } 
        else if (transformations[i] == "-quantization") {
            if (!imageLoaded) {
                std::cerr << "Error: Image must be loaded before applying transformations" << std::endl;
                return image;
            }
            int quantizationLevel = std::stoi(transformations[++i]);
            std::cout << "Applying quantization with level: " << quantizationLevel << std::endl;
            image = decoder.imageQuantization(image, quantizationLevel);
        } 
        else if (transformations[i] == "-channels") {
            if (!imageLoaded) {
                std::cerr << "Error: Image must be loaded before applying transformations" << std::endl;
                return image;
            }
            std::cout << "Displaying image channels" << std::endl;
            decoder.displayChannels(decoder.splitChannels(image), decoder.convertToGrayscale(decoder.splitChannels(image)));
        } 
        else if (transformations[i] == "-highPass") {
            if (!imageLoaded) {
                std::cerr << "Error: Image must be loaded before applying transformations" << std::endl;
                return image;
            }
            std::cout << "Applying high pass filter" << std::endl;
            image = decoder.applyHighPassFilter(image);
        } 
        else if (transformations[i] == "-rotate") {
            if (!imageLoaded) {
                std::cerr << "Error: Image must be loaded before applying transformations" << std::endl;
                return image;
            }
            double angle = std::stod(transformations[++i]);
            std::cout << "Rotating the image by " << angle << " degrees" << std::endl;
            image = decoder.rotateImage(image, angle);
        }
        else if (transformations[i] == "-invert") {
            if (!imageLoaded) {
                std::cerr << "Error: Image must be loaded before applying transformations" << std::endl;
                return image;
            }
            std::cout << "Inverting the colors of the image" << std::endl;
            image = decoder.invertColors(image);
        } 
        else if (transformations[i] == "-display") {
            std::cout << "Displaying the image" << std::endl;
            decoder.displayImage(image);  // Display the final image
        }
    }

    return image;  // Return the final transformed image
}

// Main function to test parsing
int main(int argc, char* argv[]) {
    ImageDecoder decoder;  // Assume you have an instance of ImageDecoder class to handle operations
    // Simulating the command line args as a vector of strings
    std::vector<std::string> args(argv + 1, argv + argc);

    std::vector<std::vector<std::string>> result;

    if (args[0] == "-load"){ // --------------------------------------------------------- [Load]
        std::cout << "-load" << std::endl;
        result = parseARG(args);
        cv::Mat image;
        image = applyTransformations(image, result[0]);
    } else if (args[0] == "-mse"){  // -------------------------------------------------- [MSE]
        std::cout << "-mse" << std::endl;
        result = parseCOMMAND(args);
        cv::Mat image1;
        image1 = applyTransformations(image1, result[1]);
        cv::Mat image2;
        image2 = applyTransformations(image2, result[2]);
        double mse = decoder.calculateMSE(image1, image2);
        std::cout << "\nMSE: " << mse << "\n"<< std::endl;
    } else if (args[0] == "-psnr"){ // -------------------------------------------------- [PSNR]
        std::cout << "-psnr" << std::endl;
        result = parseCOMMAND(args);
        cv::Mat image1;
        image1 = applyTransformations(image1, result[1]);
        cv::Mat image2;
        image2 = applyTransformations(image2, result[2]);
        double psnr = decoder.calculatePSNR(image1, image2);
        std::cout << "\nPSNR: " << psnr << "\n"<< std::endl;
    } else if (args[0] == "-diff"){ // -------------------------------------------------- [Absolute Difference]
        std::cout << "-diff" << std::endl;
        result = parseCOMMAND(args);
        cv::Mat image1;
        image1 = applyTransformations(image1, result[1]);
        cv::Mat image2;
        image2 = applyTransformations(image2, result[2]);
        if (result[3][1] == "true") {
            decoder.displayImage(decoder.calculateAbsoluteDifference(image1, image2));
        }else {
            decoder.calculateAbsoluteDifference(image1, image2);
        }
    } else if (args[0] == "-hist"){ // -------------------------------------------------- [histogram]
        std::cout << "-hist" << std::endl;
        if (args[1] == "-diff"){ // -------------------------------------------------- [histogram] -> [diff]
            std::vector<std::string> subArgs(args.begin() + 1, args.end());
            result = parseCOMMAND(subArgs);
            cv::Mat image1;
            image1 = applyTransformations(image1, result[1]);
            cv::Mat image2;
            image2 = applyTransformations(image2, result[2]);
            cv::Mat diffImage = decoder.calculateAbsoluteDifference(image1, image2);
            decoder.createHistogram(diffImage);
        }else { // ------------------------------------------------------------------- [histogram] -> [load]
            result = parseCOMMAND(args);
            cv::Mat image;
            image = applyTransformations(image, result[1]);
            decoder.createHistogram(image);
        } 
    }
    
    // Displaying the parsed result
    /*for (const auto& arr : result) {
        for (const auto& elem : arr) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }*/

    return 0;
}



/*int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: ./ImageDecoder <command> [options]\n";
        return -1;
    }

    ImageDecoder decoder;
    std::string imagePath1, imagePath2;
    bool grayscale1 = false, grayscale2 = false;
    bool display = false;
    bool highPass1 = false, highPass2 = false;
    bool channels1 = false, channels2 = false;
    int gaussianKernel1 = 0, gaussianKernel2 = 0;
    double gaussianSigma1 = 0, gaussianSigma2 = 0;
    int quantizationLevels1 = 0, quantizationLevels2 = 0;

    std::string command = argv[1];

    // Handle -hist command
    if (command == "-hist") {
        int argIndex = 2;
        bool isDiff = false;

        // Check if the next argument is '-diff'
        if (std::string(argv[argIndex]) == "-diff") {
            isDiff = true;
            argIndex++;

            // Process first image arguments
            if (std::string(argv[argIndex]) == "-load") {
                imagePath1 = argv[++argIndex];
            } else {
                std::cout << "Error: Missing '-load' for the first image in '-diff'.\n";
                return -1;
            }

            // Process options for the first image
            while (++argIndex < argc && std::string(argv[argIndex]).substr(0, 1) == "-") {
                std::string option = argv[argIndex];
                if (option == "-grayscale") {
                    grayscale1 = true;
                } else if (option == "-gaussian" && argIndex + 2 < argc) {
                    gaussianKernel1 = std::stoi(argv[++argIndex]);
                    gaussianSigma1 = std::stod(argv[++argIndex]);
                } else if (option == "-quantization" && argIndex + 1 < argc) {
                    quantizationLevels1 = std::stoi(argv[++argIndex]);
                } else if (option == "-channels") {
                    channels1 = true;
                } else if (option == "-highPass") {
                    highPass1 = true;
                } else if (option == "-load") {
                    // Start processing the second image
                    break;
                } else {
                    std::cout << "Error: Unknown option for first image: " << option << "\n";
                    return -1;
                }
            }

            // Process second image arguments
            if (std::string(argv[argIndex]) == "-load") {
                imagePath2 = argv[++argIndex];
            } else {
                std::cout << "Error: Missing '-load' for the second image in '-diff'.\n";
                return -1;
            }

            // Process options for the second image
            while (++argIndex < argc && std::string(argv[argIndex]).substr(0, 1) == "-") {
                std::string option = argv[argIndex];
                if (option == "-grayscale") {
                    grayscale2 = true;
                } else if (option == "-gaussian" && argIndex + 2 < argc) {
                    gaussianKernel2 = std::stoi(argv[++argIndex]);
                    gaussianSigma2 = std::stod(argv[++argIndex]);
                } else if (option == "-quantization" && argIndex + 1 < argc) {
                    quantizationLevels2 = std::stoi(argv[++argIndex]);
                } else if (option == "-channels") {
                    channels2 = true;
                } else if (option == "-highPass") {
                    highPass2 = true;
                } else {
                    std::cout << "Error: Unknown option for second image: " << option << "\n";
                    return -1;
                }
            }

            // Load both images
            cv::Mat image1 = cv::imread(imagePath1);
            cv::Mat image2 = cv::imread(imagePath2);

            if (image1.empty() || image2.empty()) {
                std::cout << "Error: Could not open or find one or both images!" << std::endl;
                return -1;
            }

            // Apply processing options for the first image
            if (grayscale1) {
                image1 = decoder.convertToGrayscale(decoder.splitChannels(image1));
            }
            if (gaussianKernel1 > 0) {
                image1 = decoder.applyGaussianBlur(image1, gaussianKernel1, gaussianSigma1);
            }
            if (quantizationLevels1 > 0) {
                image1 = decoder.imageQuantization(image1, quantizationLevels1);
            }
            if (highPass1) {
                image1 = decoder.applyHighPassFilter(image1);
            }

            // Apply processing options for the second image
            if (grayscale2) {
                image2 = decoder.convertToGrayscale(decoder.splitChannels(image2));
            }
            if (gaussianKernel2 > 0) {
                image2 = decoder.applyGaussianBlur(image2, gaussianKernel2, gaussianSigma2);
            }
            if (quantizationLevels2 > 0) {
                image2 = decoder.imageQuantization(image2, quantizationLevels2);
            }
            if (highPass2) {
                image2 = decoder.applyHighPassFilter(image2);
            }

            // Create a histogram based on the diff between two images
            cv::Mat diffImage = decoder.calculateAbsoluteDifference(image1, image2);
            decoder.createHistogram(diffImage);
        } else {
            // Single image argument
            if (std::string(argv[argIndex]) == "-load") {
                imagePath1 = argv[++argIndex];
            } else {
                std::cout << "Error: Missing '-load' for the histogram command.\n";
                return -1;
            }

            // Process options for the single image
            while (++argIndex < argc) {
                std::string option = argv[argIndex];
                if (option == "-grayscale") {
                    grayscale1 = true;
                } else if (option == "-gaussian" && argIndex + 2 < argc) {
                    gaussianKernel1 = std::stoi(argv[++argIndex]);
                    gaussianSigma1 = std::stod(argv[++argIndex]);
                } else if (option == "-quantization" && argIndex + 1 < argc) {
                    quantizationLevels1 = std::stoi(argv[++argIndex]);
                } else if (option == "-channels") {
                    channels1 = true;
                } else if (option == "-highPass") {
                    highPass1 = true;
                } else {
                    std::cout << "Error: Unknown option: " << option << "\n";
                    return -1;
                }
            }

            // Load and process the image
            cv::Mat image1 = cv::imread(imagePath1);
            if (image1.empty()) {
                std::cout << "Error: Could not open or find the image!" << std::endl;
                return -1;
            }

            // Apply grayscale conversion if requested
            if (grayscale1) {
                image1 = decoder.convertToGrayscale(decoder.splitChannels(image1));
            }

            // Apply Gaussian blur if requested
            if (gaussianKernel1 > 0) {
                image1 = decoder.applyGaussianBlur(image1, gaussianKernel1, gaussianSigma1);
            }

            // Apply quantization if requested
            if (quantizationLevels1 > 0) {
                image1 = decoder.imageQuantization(image1, quantizationLevels1);
            }

            // Apply high-pass filter if requested
            if (highPass1) {
                image1 = decoder.applyHighPassFilter(image1);
            }

            // Create a histogram for the processed image
            decoder.createHistogram(image1);
        }
    }

    // Handle diff, mse, psnr commands (require two images)
    if (command == "-diff" || command == "-mse" || command == "-psnr") {
        int argIndex = 2;

        // Process first image arguments
        if (std::string(argv[argIndex]) == "-load") {
            imagePath1 = argv[++argIndex];
        } else {
            std::cout << "Error: Missing '-load' for the first image.\n";
            return -1;
        }

        // Process options for the first image
        while (++argIndex < argc && std::string(argv[argIndex]).substr(0, 1) == "-") {
            std::string option = argv[argIndex];
            if (option == "-grayscale") {
                grayscale1 = true;
            } else if (option == "-gaussian" && argIndex + 2 < argc) {
                gaussianKernel1 = std::stoi(argv[++argIndex]);
                gaussianSigma1 = std::stod(argv[++argIndex]);
            } else if (option == "-quantization" && argIndex + 1 < argc) {
                quantizationLevels1 = std::stoi(argv[++argIndex]);
            } else if (option == "-highPass") {
                highPass1 = true;
            } else if (option == "-channels") {
                channels1 = true;
            } else if (option == "-load") {
                // Start processing the second image
                break;
            } else {
                std::cout << "Error: Unknown option for first image: " << option << "\n";
                return -1;
            }
        }

        // Process second image arguments
        if (std::string(argv[argIndex]) == "-load") {
            imagePath2 = argv[++argIndex];
        } else {
            std::cout << "Error: Missing '-load' for the second image.\n";
            return -1;
        }

        // Process options for the second image
        while (++argIndex < argc && std::string(argv[argIndex]).substr(0, 1) == "-") {
            std::string option = argv[argIndex];
            if (option == "-grayscale") {
                grayscale2 = true;
            } else if (option == "-gaussian" && argIndex + 2 < argc) {
                gaussianKernel2 = std::stoi(argv[++argIndex]);
                gaussianSigma2 = std::stod(argv[++argIndex]);
            } else if (option == "-quantization" && argIndex + 1 < argc) {
                quantizationLevels2 = std::stoi(argv[++argIndex]);
            } else if (option == "-highPass") {
                highPass1 = true;
            } else if (option == "-channels") {
                channels1 = true;
            } else if (option == "-display") {
                display = true;
            } else {
                std::cout << "Error: Unknown option for second image: " << option << "\n";
                return -1;
            }
        }

        // Load and process the images based on provided arguments
        cv::Mat image1 = cv::imread(imagePath1);
        cv::Mat image2 = cv::imread(imagePath2);

        if (image1.empty() || image2.empty()) {
            std::cout << "Error: Could not open or find one or both images!" << std::endl;
            return -1;
        }

        if (grayscale1) {
            image1 = decoder.convertToGrayscale(decoder.splitChannels(image1));
        }
        if (gaussianKernel1 > 0) {
            image1 = decoder.applyGaussianBlur(image1, gaussianKernel1, gaussianSigma1);
        }
        if (quantizationLevels1 > 0) {
            image1 = decoder.imageQuantization(image1, quantizationLevels1);
        }
        if (highPass1) {
            image1 = decoder.applyHighPassFilter(image1);
        }

        if (grayscale2) {
            image2 = decoder.convertToGrayscale(decoder.splitChannels(image2));
        }
        if (gaussianKernel2 > 0) {
            image2 = decoder.applyGaussianBlur(image2, gaussianKernel2, gaussianSigma2);
        }
        if (quantizationLevels2 > 0) {
            image2 = decoder.imageQuantization(image2, quantizationLevels2);
        }
        if (highPass2) {
            image2 = decoder.applyHighPassFilter(image2);
        }

        // Perform the requested operation
        if (command == "-diff") {
            cv::Mat differenceImage = decoder.calculateAbsoluteDifference(image1, image2);
            if (display) {
                decoder.displayImage(differenceImage);
            }
        } else if (command == "-mse") {
            double mse = decoder.calculateMSE(image1, image2);
            std::cout << "MSE: " << mse << std::endl;
        } else if (command == "-psnr") {
            double psnr = decoder.calculatePSNR(image1, image2);
            std::cout << "PSNR: " << psnr << " dB" << std::endl;
        }

    // Handle commands that work with a single image (-load)
    } else if (command == "-load") {
        imagePath1 = argv[2];
        int argIndex = 3;

        // Load the image
        cv::Mat image = cv::imread(imagePath1);
        if (image.empty()) {
            std::cout << "Error: Could not open or find the image!" << std::endl;
            return -1;
        }

        while (argIndex < argc) {
            std::string option = argv[argIndex];
            if (option == "-grayscale") {
                grayscale1 = true;
            } else if (option == "-gaussian" && argIndex + 2 < argc) {
                gaussianKernel1 = std::stoi(argv[++argIndex]);
                gaussianSigma1 = std::stod(argv[++argIndex]);
            } else if (option == "-quantization" && argIndex + 1 < argc) {
                quantizationLevels1 = std::stoi(argv[++argIndex]);
            } else if (option == "-highPass") {
                highPass1 = true;
            } else if (option == "-channels") {
                channels1 = true;
            } else if (option == "-display") {
                display = true;
            } else {
                std::cout << "Error: Unknown option: " << option << "\n";
                return -1;
            }
            argIndex++;
        }

        
        if (grayscale1) {
            image = decoder.convertToGrayscale(decoder.splitChannels(image));
        }
        if (gaussianKernel1 > 0) {
            image = decoder.applyGaussianBlur(image, gaussianKernel1, gaussianSigma1);
        }
        if (quantizationLevels1 > 0) {
            image = decoder.imageQuantization(image, quantizationLevels1);
        }
        if (highPass1) {
            image = decoder.applyHighPassFilter(image);
        }

        if (display) {
            decoder.displayImage(image);
        }
    } else {
        std::cout << "Unknown command: " << command << std::endl;
        return -1;
    }

    return 0;
}*/


/* MENU NAGIVATION

    int main() {
        ImageDecoder decoder;
        bool imageLoaded = false; // ------------------------------------------------------ Flag to check if an image is loaded
        int choice;
        std::string loadedImage = ""; // -------------------------------------------------- Name of the loaded image

        do {
            showMenu(loadedImage);
            std::cin >> choice;

            switch (choice) {
            case 1: // ------------------------| Load Image |
                {
                    std::string fullPath = getImagePath(loadedImage);
                    if (!fullPath.empty()) {
                        // --------------------------------------------------------------- Load and decode the image
                        std::pair<cv::Mat, std::vector<cv::Mat>> imageAndChannels = decoder.decodeImage(fullPath.c_str());

                        // --------------------------------------------------------------- Store the image and its channels in the dictionary   
                        imageStore[loadedImage] = { fullPath, imageAndChannels };
                        imageLoaded = true;

                        std::cout << "Image and channels stored successfully in the dictionary." << std::endl;
                    }
                }
                break;

            case 2: // ------------------------| Display the image |
                if (imageLoaded) {
                    cv::Mat image = imageStore[loadedImage].second.first; // ---------------------- Get the path to the loaded image
                    decoder.displayImage(image);  // ----------------------------------- Display the image
                } else {
                    std::cout << "No image loaded. Please load an image first.\n";
                }
                break;

            case 3: // ------------------------| Display the image in grey scale |
                if (imageLoaded) {
                    // Get the stored channels from the imageStore map
                    std::vector<cv::Mat> channels = imageStore[loadedImage].second.second;

                    // Convert the channels to grayscale and get the grayscale image
                    cv::Mat grayscaleImage = decoder.convertToGrayscale(channels);
                    std::cout << "Grayscale image generated successfully." << std::endl;

                    // Display the grayscale image using the updated displayImage function
                    if (!grayscaleImage.empty()) {
                        decoder.displayImage(grayscaleImage);  // Pass the cv::Mat grayscale image
                    } else {
                        std::cout << "Error: Could not generate the grayscale image." << std::endl;
                    }
                } else {
                    std::cout << "No image loaded. Please load an image first.\n";
                }
                break;
            case 4: // ------------------------| Display RBG and Grey Scale Channels |
                if (imageLoaded) {
                    // Get the stored color channels and grayscale image
                    std::vector<cv::Mat> channels = imageStore[loadedImage].second.second;
                    cv::Mat grayscaleImage = decoder.convertToGrayscale(channels);

                    // Call the function to display the channels in the terminal
                    decoder.displayChannels(channels, grayscaleImage);
                } else {
                    std::cout << "No image loaded. Please load an image first.\n";
                }
                break;
            case 5: // ------------------------| Histogram of grayscale |
                if (imageLoaded) {
                    // Get the grayscale image from the stored channels
                    cv::Mat grayscaleImage = decoder.convertToGrayscale(imageStore[loadedImage].second.second);

                    // Create a 2D vector to store the grayscale image data
                    std::vector<std::vector<uchar>> grayscaleData(grayscaleImage.rows, std::vector<uchar>(grayscaleImage.cols));

                    // Fill the 2D vector with pixel data from the grayscale image
                    for (int row = 0; row < grayscaleImage.rows; ++row) {
                        for (int col = 0; col < grayscaleImage.cols; ++col) {
                            grayscaleData[row][col] = grayscaleImage.at<uchar>(row, col);
                        }
                    }

                    // Call the createGrayscaleHistogram function with the grayscale data
                    decoder.createGrayscaleHistogram(grayscaleData);
                } else {
                    std::cout << "No image loaded. Please load an image first.\n";
                }
                break;
            case 6: // ------------------------| Apply Gaussian Blur |
                if (imageLoaded) {
                    int kernelSize;
                    int blurType;

                    // Ask the user for the kernel size
                    std::cout << "Enter the kernel size (odd number, e.g., 3, 5, 7): ";
                    std::cin >> kernelSize;

                    // Ensure the kernel size is an odd number
                    if (kernelSize % 2 == 0) {
                        std::cout << "Invalid kernel size. Must be an odd number.\n";
                        break;
                    }

                    // Ask the user for the blur type
                    std::cout << "Choose the type of blur:\n";
                    std::cout << "1. Soft blur (sigma = 0.5)\n";
                    std::cout << "2. Moderate blur (sigma = 1.0)\n";
                    std::cout << "3. Strong blur (sigma = 2.0)\n";
                    std::cout << "Enter your choice (1, 2, or 3): ";
                    std::cin >> blurType;


                    // Translate the choice into a sigma value
                    double sigma;
                    switch (blurType) {
                        case 1:
                            sigma = 0.5;  // Soft blur
                            break;
                        case 2:
                            sigma = 1.0;  // Moderate blur
                            break;
                        case 3:
                            sigma = 2.0;  // Strong blur
                            break;
                        default:
                            std::cout << "Invalid blur type selected.\n";
                            break;
                    }

                    // Apply the Gaussian blur to the loaded image
                    cv::Mat blurredImage = decoder.applyGaussianBlur(imageStore[loadedImage].second.first, kernelSize, sigma);

                    // Display the blurred image
                    decoder.displayImage(blurredImage);
                } else {
                    std::cout << "No image loaded. Please load an image first.\n";
                }
                break;
            case 7: // ------------------------| Calculate absolute difference between boat.ppm and girl.ppm |
                {
                    std::string boatImagePath = "../../../datasets/image/boat.ppm"; // Path to boat image
                    std::string girlImagePath = "../../../datasets/image/girl.ppm"; // Path to girl image

                    // Load the boat image
                    cv::Mat boatImage = cv::imread(boatImagePath);
                    if (boatImage.empty()) {
                        std::cout << "Error: Could not open or find the boat image!" << std::endl;
                        break;
                    }

                    // Load the girl image
                    cv::Mat girlImage = cv::imread(girlImagePath);
                    if (girlImage.empty()) {
                        std::cout << "Error: Could not open or find the girl image!" << std::endl;
                        break;
                    }

                    // Calculate the absolute difference between the two images
                    cv::Mat differenceImage = decoder.calculateAbsoluteDifference(boatImage, girlImage);

                    if (differenceImage.empty()) {
                        std::cout << "Error: Could not calculate the difference (image size or type mismatch)." << std::endl;
                        break;
                    }

                    // Display the resulting image (absolute difference)
                    decoder.displayImage(differenceImage);
                }
                break;
            case 8: // ------------------------| Calculate MSE and PSNR between boat.ppm and girl.ppm |
                {
                    std::string boatImagePath = "../../../datasets/image/boat.ppm"; // Path to boat image
                    std::string girlImagePath = "../../../datasets/image/girl.ppm"; // Path to girl image

                    // Load the boat image
                    cv::Mat boatImage = cv::imread(boatImagePath);
                    if (boatImage.empty()) {
                        std::cout << "Error: Could not open or find the boat image!" << std::endl;
                        break;
                    }

                    // Load the girl image
                    cv::Mat girlImage = cv::imread(girlImagePath);
                    if (girlImage.empty()) {
                        std::cout << "Error: Could not open or find the girl image!" << std::endl;
                        break;
                    }

                    // Calculate the Mean Squared Error (MSE)
                    double mse = decoder.calculateMSE(boatImage, girlImage);
                    if (mse >= 0.0) {
                        std::cout << "\n > Mean Squared Error (MSE): " << mse << std::endl;
                    }

                    // Calculate the Peak Signal-to-Noise Ratio (PSNR)
                    double psnr = decoder.calculatePSNR(boatImage, girlImage);
                    if (psnr >= 0.0) {
                        std::cout << "\n > Peak Signal-to-Noise Ratio (PSNR): " << psnr << " dB" << std::endl;
                    }
                }
                break;
            case 9: // ------------------------| Image Quantization and Comparison |
                {
                    // Ask user for quantization level
                    int quantizationLevels;
                    std::cout << "Enter the number of quantization levels (2 to 256): ";
                    std::cin >> quantizationLevels;

                    // Ensure valid input
                    if (quantizationLevels < 2 || quantizationLevels > 256) {
                        std::cout << "Invalid quantization level. Please enter a value between 2 and 256." << std::endl;
                        break;
                    }

                    // Load the original image (boat.ppm)
                    std::string boatImagePath = "../../../datasets/image/boat.ppm"; // Path to boat image
                    cv::Mat boatImage = cv::imread(boatImagePath);

                    if (boatImage.empty()) {
                        std::cout << "Error: Could not open the boat image!" << std::endl;
                        break;
                    }

                    // Apply image quantization
                    cv::Mat quantizedImage = decoder.imageQuantization(boatImage, quantizationLevels);

                    // Display the quantized image
                    decoder.displayImage(quantizedImage);

                    // Compare the original image with the quantized image
                    double mse = decoder.calculateMSE(boatImage, quantizedImage);
                    double psnr = decoder.calculatePSNR(boatImage, quantizedImage);

                    // Display MSE and PSNR
                    std::cout << "\nComparison of Original vs Quantized Image:" << std::endl;
                    std::cout << "> Mean Squared Error (MSE): " << mse << std::endl;
                    std::cout << "> Peak Signal-to-Noise Ratio (PSNR): " << psnr << " dB" << std::endl;
                }
                break;
            case 10: // ------------------------| Exit |
                std::cout << "Exiting the program...\n";
                break;

            default:
                std::cout << "Invalid option. Please try again.\n";
            }

        } while (choice != 10);

        return 0;
    }
*/