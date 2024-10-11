#include <iostream>
#include "TextFileReader.h"


void showMenu() {
    std::cout << "\n----- TEXT FILE PROCESSING MENU -----" << std::endl;
    std::cout << "1. Load a Text File <pre-chosen for test purposes>" << std::endl;
    std::cout << "2. Display Loaded Text File Content <pre-chosen>" << std::endl;
    std::cout << "3. Converting to lowercase <pre-chosen>" << std::endl;
    std::cout << "4. Removing Punctuation <pre-chosen>" << std::endl;
    std::cout << "5. Calculate the chars frequencies  <pre-chosen> " << std::endl;
    std::cout << "6. Calculate the words frequencies <pre-chosen> " << std::endl;
    std::cout << "7. Calculate the frequency of n-grams (bigrams == 2, trigrams == 3)" << std::endl;
    std::cout << "8. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    TextFileReader reader;
    std::string filePath = "../../../datasets/text/pt/ep-11-06-22-011.txt";
    std::string loadedFileName;
    int choice;
    bool fileLoaded = false;

    do {
        showMenu(); // Show the menu
        std::cin >> choice;

        switch (choice) {
            case 1:
                // Load the pre-defined file
                if (reader.loadFile(filePath, loadedFileName)) {
                    fileLoaded = true;
                    std::cout << "\nFile loaded successfully!\n";
                } else {
                    std::cerr << "\nFailed to load file.\n";
                }
                break;

            case 2:
                // Display the loaded file content
                if (fileLoaded) {
                    reader.printFileContent(loadedFileName); // Use the correct file name
                } else {
                    std::cout << "\nNo file loaded. Please load a file first.\n";
                }
                break;
            case 3:
                // Convert the loaded file content to lowercase
                if (fileLoaded) {
                    reader.convertToLowercase(loadedFileName); // Use the correct file name
                    std::cout << "\nFile content converted to lowercase successfully!\n";
                    reader.printFileContent(loadedFileName); // Use the correct file name
                } else {
                    std::cout << "\nNo file loaded. Please load a file first.\n";
                }
                break;
            case 4:
                // Remove punctuation from the loaded file content
                if (fileLoaded) {
                    reader.removePunctuation(loadedFileName); // Use the correct file name
                    std::cout << "\nPunctuation removed successfully!\n";
                    reader.printFileContent(loadedFileName); // Use the correct file name
                } else {
                    std::cout << "\nNo file loaded. Please load a file first.\n";
                }
                break;
            case 5:
                // Calculate the frequency of characters in the loaded file content
                if (fileLoaded) {
                    reader.calculateCharsFrequency(loadedFileName); // Use the correct file name
                } else {
                    std::cout << "\nNo file loaded. Please load a file first.\n";
                }
                break;
            case 6:
                // Calculate the frequency of words in the loaded file content
                if (fileLoaded) {
                    reader.calculateWordsFrequency(loadedFileName); // Use the correct file name
                } else {
                    std::cout << "\nNo file loaded. Please load a file first.\n";
                }
                break;
            case 7:
                // Calculate the frequency of n-grams in the loaded file content
                if (fileLoaded) {
                    int n;
                    std::cout << "Enter the value of n for n-grams: ";
                    std::cin >> n;
                    reader.calculateNgramsFrequency(loadedFileName, n); // Use the correct file name
                } else {
                    std::cout << "\nNo file loaded. Please load a file first.\n";
                }
               
                break;
            case 8:
                std::cout << "Exiting the program...\n";
                break;

            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    } while (choice != 8);

    return 0;
}
