
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;  // Shorten the namespace for convenience

// TextFileReader class to read and store content of a text file
class TextFileReader {

private:
    std::list<std::string> fileContent;  // Stores lines from the text file

public:
    // Method to open and read the text file
    bool readFile(const std::string& filePath) {
        std::ifstream inputFile(filePath);

        // Check if the file is open
        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open file: " << filePath << std::endl;
            return false;
        }

        // Add each line to the list
        std::string currentLine;
        while (std::getline(inputFile, currentLine)) {
            fileContent.push_back(currentLine);
        }

        // Close the file
        inputFile.close();
        return true;
    }

    // Method to print the content of the file
    void printFileContent() {
        std::cout << "\n----- FILE CONTENT -----\n" << std::endl;
        for (const auto& line : fileContent) {
            std::cout << line << std::endl;
        }
    }
};

// Function to list all text files in the dataset directory
std::vector<std::string> listTextFilesInDataset(const std::string& datasetPath) {
    std::vector<std::string> textFiles;
    for (const auto& entry : fs::directory_iterator(datasetPath)) {
        if (entry.path().extension() == ".txt") {
            textFiles.push_back(entry.path().filename().string());  // Store filename with extension
        }
    }
    return textFiles;
}

// Function to show the menu for text file manipulation
void showMenu(const std::string& loadedFile) {
    std::cout << "\n----- TEXT FILE PROCESSING MENU -----" << std::endl;
    if (!loadedFile.empty()) {
        std::cout << "\n----| LOADED File : " << loadedFile << "|----\n" << std::endl;
    }
    std::cout << "1. Load Text File" << std::endl;
    std::cout << "2. Display Text File Content" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

// Function to display available files and allow user to choose
std::string getFilePath(std::string& fileName) {
    std::string datasetPath = "../partI/dataset/pt/";  // The directory where files are located

    // List all .txt files in the dataset directory
    std::vector<std::string> textFiles = listTextFilesInDataset(datasetPath);

    if (textFiles.empty()) {
        std::cout << "No text files found in the dataset directory.\n";
        return "";
    }

    // Display a menu of available files
    std::cout << "\nAvailable text files in the dataset:\n";
    for (size_t i = 0; i < textFiles.size(); ++i) {
        std::cout << i + 1 << ". " << textFiles[i] << std::endl;
    }

    int choice;
    std::cout << "Select a file by number: ";
    std::cin >> choice;

    // Ensure valid input
    if (choice < 1 || choice > textFiles.size()) {
        std::cout << "Invalid selection.\n";
        return "";
    }

    // Set the selected file name
    fileName = textFiles[choice - 1];

    // Construct the full file path
    std::string filePath = datasetPath + fileName;

    return filePath;
}

int main() {
    TextFileReader reader;
    bool fileLoaded = false;
    int choice;
    std::string loadedFile = "";  // To store the name of the loaded file
    std::string filePath;

    do {
        showMenu(loadedFile);
        std::cin >> choice;

        switch (choice) {
            case 1:
                // List and load a file from the dataset directory
                filePath = getFilePath(loadedFile);
                if (!filePath.empty()) {  // Proceed only if a valid file is selected
                    if (reader.readFile(filePath)) {
                        fileLoaded = true;
                        std::cout << "File read successfully.\n";
                    } else {
                        fileLoaded = false;
                    }
                }
                break;

            case 2:
                // Display the text file content if loaded
                if (fileLoaded) {
                    reader.printFileContent();
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
