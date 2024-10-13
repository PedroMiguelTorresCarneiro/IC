#include <iostream>
#include <string>
#include <vector>
#include "TextFileReader.h"

using namespace std;

/*
void showMenu() {
    cout << "\n----- TEXT FILE PROCESSING MENU -----" << endl;
    cout << "1. Load a Text File <pre-chosen for test purposes>" << endl;
    cout << "2. Display Loaded Text File Content <pre-chosen>" << endl;
    cout << "3. Converting to lowercase <pre-chosen>" << endl;
    cout << "4. Removing Punctuation <pre-chosen>" << endl;
    cout << "5. Calculate the chars frequencies  <pre-chosen> " << endl;
    cout << "6. Calculate the words frequencies <pre-chosen> " << endl;
    cout << "7. Calculate the frequency of n-grams (bigrams == 2, trigrams == 3)" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    TextFileReader reader;
    string filePath = "../../../datasets/text/pt/ep-11-06-22-011.txt";
    string loadedFileName;
    int choice;
    bool fileLoaded = false;

    do {
        showMenu(); // Show the menu
        cin >> choice;

        switch (choice) {
            case 1:
                // Load the pre-defined file
                if (reader.loadFile(filePath, loadedFileName)) {
                    fileLoaded = true;
                    cout << "\nFile loaded successfully!\n";
                } else {
                    cerr << "\nFailed to load file.\n";
                }
                break;

            case 2:
                // Display the loaded file content
                if (fileLoaded) {
                    reader.printFileContent(loadedFileName); // Use the correct file name
                } else {
                    cout << "\nNo file loaded. Please load a file first.\n";
                }
                break;
            case 3:
                // Convert the loaded file content to lowercase
                if (fileLoaded) {
                    reader.convertToLowercase(loadedFileName); // Use the correct file name
                    cout << "\nFile content converted to lowercase successfully!\n";
                    reader.printFileContent(loadedFileName); // Use the correct file name
                } else {
                    cout << "\nNo file loaded. Please load a file first.\n";
                }
                break;
            case 4:
                // Remove punctuation from the loaded file content
                if (fileLoaded) {
                    reader.removePunctuation(loadedFileName); // Use the correct file name
                    cout << "\nPunctuation removed successfully!\n";
                    reader.printFileContent(loadedFileName); // Use the correct file name
                } else {
                    cout << "\nNo file loaded. Please load a file first.\n";
                }
                break;
            case 5:
                // Calculate the frequency of characters in the loaded file content
                if (fileLoaded) {
                    reader.calculateCharsFrequency(loadedFileName); // Use the correct file name
                } else {
                    cout << "\nNo file loaded. Please load a file first.\n";
                }
                break;
            case 6:
                // Calculate the frequency of words in the loaded file content
                if (fileLoaded) {
                    reader.calculateWordsFrequency(loadedFileName); // Use the correct file name
                } else {
                    cout << "\nNo file loaded. Please load a file first.\n";
                }
                break;
            case 7:
                // Calculate the frequency of n-grams in the loaded file content
                if (fileLoaded) {
                    int n;
                    cout << "Enter the value of n for n-grams: ";
                    cin >> n;
                    reader.calculateNgramsFrequency(loadedFileName, n); // Use the correct file name
                } else {
                    cout << "\nNo file loaded. Please load a file first.\n";
                }
               
                break;
            case 8:
                cout << "Exiting the program...\n";
                break;

            default:
                cout << "Invalid option. Please try again.\n";
                break;
        }
    } while (choice != 8);

    return 0;
}

*/

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: ./TextDecoder <command> [options]\n";
        cout << "Commands: \n";
        cout << "  -load <file-path> [-lower] [-punct] [-display] [-charFreq | -wordFreq | -nGrams <N>]\n";
        cout << "  -charFreq -load <file-path> [-lower] [-punct] [-display]\n";
        cout << "  -wordFreq -load <file-path> [-lower] [-punct] [-display]\n";
        cout << "  -nGrams <N> -load <file-path> [-lower] [-punct] [-display]\n";
        return -1;
    }

    TextFileReader reader;
    string loadedFileName;
    bool lowercase = false, removePunctuation = false, display = false;
    int nGramsValue = 0;
    string filePath;
    string command = argv[1];
        // Check if the first argument is "-encod"
    if (command == "-encod") {
        if (argc < 4 || string(argv[2]) != "-load") {
            cout << "Error: Missing '-load' argument for encoding detection.\n";
            return -1;
        }

        // Get the file path after '-load'
        filePath = argv[3];

        // Detect encoding
        string detectedEncoding = reader.guessEncoding(filePath);
        if (!detectedEncoding.empty()) {
            cout << "\nDetected Encoding: " << detectedEncoding << "\n\n";
        } else {
            cout << "Could not detect encoding.\n";
        }
        return 0;  // Exit after detecting encoding
    }

    // Check if the first argument is "-load"
    if (command == "-load") {
        if (argc < 3) {
            cout << "\nError: Missing file path after '-load'.\n\n";
            return -1;
        }

        filePath = argv[2];  // The file path is the next argument

        // Load the file
        if (!reader.loadFile(filePath, loadedFileName)) {
            cout << "Error: Could not load file: " << filePath << "\n";
            return -1;
        }

        // Process remaining options starting from index 3
        for (int i = 3; i < argc; ++i) {
            string option = argv[i];

            if (option == "-lower") {
                lowercase = true;
            } else if (option == "-punct") {
                removePunctuation = true;
            } else if (option == "-display") {
                display = true;
            } else if (option == "-charFreq") {
                reader.calculateCharsFrequency(loadedFileName);
                return 0;
            } else if (option == "-wordFreq") {
                reader.calculateWordsFrequency(loadedFileName);
                return 0;
            } else if (option == "-nGrams") {
                if (i + 1 < argc) {
                    nGramsValue = stoi(argv[++i]);
                    reader.calculateNgramsFrequency(loadedFileName, nGramsValue);
                    return 0;
                } else {
                    cout << "Error: Missing number after -nGrams.\n";
                    return -1;
                }
            } else {
                cout << "Error: Unknown option: " << option << "\n";
                return -1;
            }
        }

        // Apply transformations after loading the file
        if (lowercase) {
            reader.convertToLowercase(loadedFileName);
            cout << "File content converted to lowercase.\n";
        }
        if (removePunctuation) {
            reader.removePunctuation(loadedFileName);
            cout << "Punctuation removed from file content.\n";
        }
        if (display) {
            reader.printFileContent(loadedFileName);
        }
    } else if (command == "-nGrams" || command == "-charFreq" || command == "-wordFreq") {
        // Handle operations like nGrams, charFreq, or wordFreq before -load
        if (command == "-nGrams") {
            if (argc < 5) {
                cout << "Error: Missing number after -nGrams and file path.\n";
                return -1;
            }

            nGramsValue = stoi(argv[2]);

            if (string(argv[3]) != "-load") {
                cout << "Error: Missing '-load' argument for nGrams.\n";
                return -1;
            }

            filePath = argv[4];
            if (!reader.loadFile(filePath, loadedFileName)) {
                cout << "Error: Could not load file: " << filePath << "\n";
                return -1;
            }

            // Apply nGrams after loading the file
            reader.calculateNgramsFrequency(loadedFileName, nGramsValue);

        } else if (command == "-charFreq" || command == "-wordFreq") {
            // Check for "-load"
            if (argc < 4 || string(argv[2]) != "-load") {
                cout << "Error: Missing '-load' argument for " << command << ".\n";
                return -1;
            }

            filePath = argv[3];
            if (!reader.loadFile(filePath, loadedFileName)) {
                cout << "Error: Could not load file: " << filePath << "\n";
                return -1;
            }

            // Perform the operation based on the command
            if (command == "-charFreq") {
                reader.calculateCharsFrequency(loadedFileName);
            } else if (command == "-wordFreq") {
                reader.calculateWordsFrequency(loadedFileName);
            }
        }
    } else {
        cout << "Unknown command: " << command << "\n";
        return -1;
    }

    return 0;
}