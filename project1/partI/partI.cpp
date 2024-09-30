#include <iostream>
#include <filesystem>
#include <vector>
#include "TextFileReader.h"

namespace fs = std::filesystem;

/**
 * Function to list all available text files in the specified directory and
 * return them as a vector of strings sorted by name.
 *
 * @param datasetPath The path to the dataset directory where the text files are stored.
 * @return A vector of strings containing the filenames of the text files sorted alphabetically.
 */
std::vector<std::string> listTextFilesInDataset(const std::string& datasetPath) {
    std::vector<std::string> textFiles;
    
    // Iterate through the directory and add all .txt files to the vector
    for (const auto& entry : fs::directory_iterator(datasetPath)) {
        if (entry.path().extension() == ".txt") {
            textFiles.push_back(entry.path().filename().string());  // Store the filename with extension
        }
    }

    // Sort the file names in alphabetical order
    std::sort(textFiles.begin(), textFiles.end());

    return textFiles;
}

/**
 * Function to display the available text files and allow the user to select one.
 *
 * @param textFiles A vector containing the sorted filenames of the text files.
 * @return The name of the file selected by the user, or an empty string if no valid file is selected.
 */
void showMenu(const std::string& loadedFile) {
    std::cout << "\n----- TEXT FILE PROCESSING MENU -----" << std::endl;
    if (!loadedFile.empty()) {
        std::cout << "\n----| LOADED File : " << loadedFile << "|----\n" << std::endl;
    }
    std::cout << "1. Load a Text File" << std::endl;
    std::cout << "2. Display Loaded Text File Content" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

/**
 * Function to display the available text files and allow the user to select one.
 *
 * @param textFiles A vector containing the sorted filenames of the text files.
 * @return The name of the file selected by the user, or an empty string if no valid file is selected.
 */
std::string chooseFileToLoad(const std::vector<std::string>& textFiles) {
    if (textFiles.empty()) {
        std::cout << "No text files found in the dataset directory.\n";
        return "";
    }

    // Display available text files
    std::cout << "\nAvailable text files:\n";
    for (size_t i = 0; i < textFiles.size(); ++i) {
        std::cout << i + 1 << ". " << textFiles[i] << std::endl;
    }

    int choice;
    std::cout << "Select a file by number to load: ";
    std::cin >> choice;

    if (choice < 1 || choice > textFiles.size()) {
        std::cout << "Invalid selection.\n";
        return "";
    }

    return textFiles[choice - 1];  // Return the filename
}

int main() {
    TextFileReader reader;
    std::vector<std::string> textFiles;
    bool fileLoaded = false;
    std::string loadedFile = "";
    int choice;
    std::string datasetPath = "../dataset/pt/";

    do {
        showMenu(loadedFile);  // Show the menu with the loaded file name (if any)
        std::cin >> choice;

        switch (choice) {
            case 1:
                // List available files and allow the user to select one to load
                textFiles = listTextFilesInDataset(datasetPath);  // List and sort the files
                if (textFiles.empty()) {
                    std::cout << "No text files available in the directory.\n";
                } else {
                    std::string fileToLoad = chooseFileToLoad(textFiles);  // Let the user choose from the sorted list
                    if (!fileToLoad.empty()) {
                        std::string fullPath = datasetPath + fileToLoad;  // Full path to the file
                        if (reader.loadFile(fullPath, fileToLoad)) {
                            loadedFile = fileToLoad;  // Store the loaded file name
                            fileLoaded = true;
                            std::cout << "File loaded successfully: " << loadedFile << "\n";
                        } else {
                            std::cout << "Error loading file.\n";
                        }
                    }
                }
                break;

            case 2:
                // Display the loaded file content
                if (fileLoaded) {
                    reader.printFileContent(loadedFile);  // Display the content of the loaded file
                } else {
                    std::cout << "No file loaded. Please load a file first.\n";
                }
                break;

            case 3:
                std::cout << "Exiting the program...\n";
                break;

            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }

    } while (choice != 3);

    return 0;
}
