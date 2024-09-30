#include "TextFileReader.h"
#include <iostream>
#include <fstream>

/**
 * Method to read and store the content of a specific file.
 * 
 * This method opens the file at the specified `filePath` and reads its contents line by line.
 * Each line is stored in a `std::list<std::string>`. The file content is stored in a map (`fileContents`)
 * where the key is the `fileName` and the value is the list of strings representing the file content.
 * 
 * @param filePath The full path to the file to be loaded.
 * @param fileName The name of the file, used as the key in the map to store the content.
 * @return Returns `true` if the file was opened and read successfully, `false` if the file could not be opened.
 */
bool TextFileReader::loadFile(const std::string& filePath, const std::string& fileName) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file: " << filePath << std::endl;
        return false;
    }

    std::list<std::string> content;
    std::string line;

    while (std::getline(inputFile, line)) {
        content.push_back(line);
    }

    inputFile.close();
    fileContents[fileName] = content;
    return true;
}

/**
 * Method to print the content of a specific file.
 * 
 * This method prints the content of the file that was previously loaded using the `loadFile` method.
 * The file content is printed line by line. If the file is not found in the map, an error message is displayed.
 * 
 * @param fileName The name of the file whose content is to be printed.
 */
void TextFileReader::printFileContent(const std::string& fileName) {
    if (fileContents.find(fileName) != fileContents.end()) {
        std::cout << "\n----- CONTENT OF FILE: " << fileName << " -----\n" << std::endl;
        for (const auto& line : fileContents[fileName]) {
            std::cout << line << std::endl;
        }
    } else {
        std::cout << "File not found or not loaded: " << fileName << std::endl;
    }
}
