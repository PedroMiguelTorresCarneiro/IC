#include <iostream>
#include <string>
#include <vector>
#include "TextFileReader.h"
#include <boost/locale.hpp>
#include <locale>

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
using namespace std;


vector<vector<string>> parseArgs(const vector<string>& args) {
    vector<vector<string>> result;
    vector<string> loadArgs;

    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-load") {
            loadArgs.push_back(args[i]);  // Capture -load
            loadArgs.push_back(args[++i]);  // Capture file path
        } else if (args[i] == "-lower" || args[i] == "-punct" || args[i] == "-num" || args[i] == "-display") {
            loadArgs.push_back(args[i]);  // Capture flags like -lower, -punct, -display
        }
    }

    result.push_back(loadArgs);
    return result;
}

vector<vector<string>> parseCOMMAND(vector<string> args) {
    vector<vector<string>> result;
    vector<string> command;
    vector<string> fileArgs;
    vector<string> displayArgs = {"-display", "false"};  // Default display flag to false
    vector<string> histArgs = {"-hist", "false"};  // Default histogram flag to false
    int nGramsValue = 0;

    // Handle the first command argument
    command.push_back(args[0]);

    // Iterate over the rest of the arguments
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[0] == "-nGrams") {
            // Ensure the next argument is the N value for N-grams
            if (i + 1 < args.size() && args[i].find_first_not_of("0123456789") == string::npos) {
                nGramsValue = stoi(args[i]);  // Parse N value
                command.push_back(to_string(nGramsValue));
                ++i;  // Skip the next argument as it's already processed
            } else {
                cerr << "Error: Missing or invalid N value for -nGrams.\n";
                break;
            }
        }

        if (args[i] == "-load") {
            fileArgs.push_back(args[i]);   // Add -load argument
            fileArgs.push_back(args[++i]); // Add file path
        } else if (args[i] == "-lower" || args[i] == "-punct" || args[i] == "-num") {
            fileArgs.push_back(args[i]);   // Add options like -lower or -punct
        } else if (args[i] == "-display") {
            displayArgs[1] = "true";  // Set display flag to true
        } else if (args[i] == "-hist") {
            histArgs[1] = "true";  // Set histogram flag to true
        }
    }

    result.push_back(command);
    result.push_back(fileArgs);
    result.push_back(displayArgs);
    result.push_back(histArgs);

    return result;
}


void applyTransformations(TextFileReader& reader, const vector<string>& transformations, const string& loadedFileName) {
    for (const auto& transform : transformations) {
        if (transform == "-lower") {
            reader.convertToLowercase(loadedFileName);  // Convert to lowercase
        } else if (transform == "-punct") {
            reader.removePunctuation(loadedFileName);  // Remove punctuation
        } else if (transform == "-display") {
            reader.printFileContent(loadedFileName);  // Display content
        } else if (transform == "-num") {
            reader.removeNumbers(loadedFileName);  // Remove numbers
        }
    }
}

int main(int argc, char* argv[]) {
    // Set the global locale to UTF-8 using Boost.Locale
    try {
        locale::global(boost::locale::generator().generate("en_US.UTF-8"));
    } catch (const runtime_error& e) {
        cerr << "Locale setup failed: " << e.what() << endl;
        return -1;
    }

    TextFileReader reader;  // Assuming you have a TextFileReader class to handle text operations
    vector<string> args(argv + 1, argv + argc);  // Gather arguments into a vector
    vector<vector<string>> result;

    string loadedFileName;  // Non-const variable to store the loaded file name

    // First, check if the command is related to loading and transformations
    if (args[0] == "-load") {
        cout << "-load" << endl;
        result = parseArgs(args);
        const string& filePath = result[0][1];  // Capture the file path

        // Load the file and apply transformations
        if (reader.loadFile(filePath, loadedFileName)) {
            applyTransformations(reader, result[0], loadedFileName);  // Apply transformations on the loaded file
        } else {
            cerr << "Error: Could not load file: " << filePath << endl;
            return -1;
        }
    }
    // Handle character frequency command
    else if (args[0] == "-charFreq") {
        result = parseCOMMAND(args);
        const string& filePath = result[1][1];

        if (reader.loadFile(filePath, loadedFileName)) {
            applyTransformations(reader, result[1], loadedFileName);  // Apply transformations
            reader.calculateCharsFrequency(loadedFileName, result[3][1] == "true");  // Calculate char frequency
        } else {
            cerr << "Error: Could not load file: " << filePath << endl;
            return -1;
        }
    }
    // Handle word frequency command
    else if (args[0] == "-wordFreq") {
        result = parseCOMMAND(args);
        const string& filePath = result[1][1];

        if (reader.loadFile(filePath, loadedFileName)) {
            applyTransformations(reader, result[1], loadedFileName);  // Apply transformations
            reader.calculateWordsFrequency(loadedFileName, result[3][1] == "true");  // Calculate word frequency
        } else {
            cerr << "Error: Could not load file: " << filePath << endl;
            return -1;
        }
    }
    // Handle N-grams frequency command
    else if (args[0] == "-nGrams") {
        cout << "-nGrams" << endl;

        // Pass the entire argument list to parseCOMMAND (including nGrams value)
        result = parseCOMMAND(args);

        // Get the N-grams value directly from the parsed result
        int nGramsValue = stoi(result[0][1]);  // result[0][1] contains the N value
        cout << "nGramsValue: " << nGramsValue << endl;

        const string& filePath = result[1][1];  // Get the file path

        // Load file and apply transformations
        if (reader.loadFile(filePath, loadedFileName)) {
            applyTransformations(reader, result[1], loadedFileName);  // Apply transformations (like -lower, -punct)
            reader.calculateNgramsFrequency(loadedFileName, nGramsValue, result[3][1] == "true");  // Calculate N-grams
        } else {
            cerr << "Error: Could not load file: " << filePath << endl;
            return -1;
        }
    }
    // Handle histogram flag
    else if (args[0] == "-hist") {
        if (args[1] == "-charFreq" || args[1] == "-wordFreq" || args[1] == "-nGrams") {
            vector<string> subArgs(args.begin() + 1, args.end());
            result = parseCOMMAND(subArgs);
            const string& filePath = result[1][1];

            if (reader.loadFile(filePath, loadedFileName)) {
                applyTransformations(reader, result[1], loadedFileName);  // Apply transformations

                if (subArgs[0] == "-charFreq") {
                    reader.calculateCharsFrequency(loadedFileName, true);  // Enable histogram for char frequency
                } else if (subArgs[0] == "-wordFreq") {
                    reader.calculateWordsFrequency(loadedFileName, true);  // Enable histogram for word frequency
                } else if (subArgs[0] == "-nGrams") {
                    int nGramsValue = stoi(result[0][1]);
                    reader.calculateNgramsFrequency(loadedFileName, nGramsValue, true);  // Enable histogram for N-grams
                }
            } else {
                cerr << "Error: Could not load file: " << filePath << endl;
                return -1;
            }
        } else {
            cerr << "Error: Invalid histogram option.\n";
            return -1;
        }
    }
    else {
        cerr << "Error: Unknown command.\n";
        return -1;
    }

    return 0;
}


