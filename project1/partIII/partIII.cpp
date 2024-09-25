#include "ImageDecoder.cpp"  // Include the ImageDecoder class
#include <filesystem>  // For file checking
#include <cstring>
#include <vector>

namespace fs = std::filesystem;  // Shorten the namespace for convenience

void showMenu(const std::string& loadedImage) {
    std::cout << "\n----- IMAGE PROCESSING MENU -----" << std::endl;
    if (!loadedImage.empty()) {
        std::cout << "\n----| LOADED Image : " << loadedImage << "|----\n" << std::endl;  // Display the loaded image name
    }
    std::cout << "1. Load Image" << std::endl;
    std::cout << "2. Display image" << std::endl;
    std::cout << "3. Save image" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

// Function to list all images in the dataset directory
std::vector<std::string> listImagesInDataset(const std::string& datasetPath) {
    std::vector<std::string> images;
    for (const auto& entry : fs::directory_iterator(datasetPath)) {
        if (entry.path().extension() == ".ppm") {
            images.push_back(entry.path().filename().stem());  // Only the filename without extension
        }
    }
    return images;
}

std::string getImagePath(std::string& imageName) {
    std::string datasetPath = "../dataset/";  // Adjusted relative path to dataset

    // List all images in the dataset directory
    std::vector<std::string> images = listImagesInDataset(datasetPath);

    if (images.empty()) {
        std::cout << "No images found in the dataset directory.\n";
        return "";
    }

    // Display a menu of available images
    std::cout << "\nAvailable images in the dataset:\n";
    for (size_t i = 0; i < images.size(); ++i) {
        std::cout << i + 1 << ". " << images[i] << std::endl;
    }

    int choice;
    std::cout << "Select an image by number: ";
    std::cin >> choice;

    // Ensure valid input
    if (choice < 1 || choice > images.size()) {
        std::cout << "Invalid selection.\n";
        return "";
    }

    // Set the selected image name (without extension)
    imageName = images[choice - 1];

    // Construct the full image path
    std::string imagePath = datasetPath + imageName + ".ppm";

    return imagePath;
}

int main() {
    ImageDecoder decoder;
    bool imageLoaded = false;
    int choice;
    std::string loadedImage = "";  // To store the name of the loaded image
    char outputFile[256];

    do {
        showMenu(loadedImage);
        std::cin >> choice;

        switch (choice) {
        case 1:
            // List and load image from the dataset directory
            {
                std::string fullPath = getImagePath(loadedImage);
                if (!fullPath.empty()) {  // Proceed only if a valid image is selected
                    decoder.decodeImage(fullPath.c_str());
                    imageLoaded = true;
                }
            }
            break;

        case 2:
            // Display the loaded image if available
            if (imageLoaded) {
                decoder.displayImage();
            } else {
                std::cout << "No image loaded. Please load an image first.\n";
            }
            break;

        case 3:
            // Save the image if available
            if (imageLoaded) {
                std::cout << "Enter the path to save the image: ";
                std::cin >> outputFile;
                decoder.saveImage(outputFile);
            } else {
                std::cout << "No image loaded. Please load an image first.\n";
            }
            break;

        case 4:
            std::cout << "Exiting the program...\n";
            break;

        default:
            std::cout << "Invalid option. Please try again.\n";
        }

    } while (choice != 4);

    return 0;
}
