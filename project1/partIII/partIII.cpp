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
    std::cout << "5. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

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
                std::string path = imageStore[loadedImage].first; // ---------------------- Get the path to the loaded image
                decoder.displayImage(path.c_str());  // ----------------------------------- Display the image
            } else {
                std::cout << "No image loaded. Please load an image first.\n";
            }
            break;

        case 3: // ------------------------| Display the image in grey scale |
            if (imageLoaded) {
                std::vector<cv::Mat> channels = imageStore[loadedImage].second.second; // - Get the loaded image channels

                decoder.convertToGrayscale(channels); // ------------------------------------- Convert the image to grayscale and display
            } else {
                std::cout << "No image loaded. Please load an image first.\n";
            }
            break;
        case 4: // ------------------------| Display RBG and Grey Scale Channels |

            break;
        case 5: // ------------------------| Exit |
            std::cout << "Exiting the program...\n";
            break;

        default:
            std::cout << "Invalid option. Please try again.\n";
        }

    } while (choice != 4);

    return 0;
}
