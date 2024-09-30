#ifndef TEXTFILEREADER_H
#define TEXTFILEREADER_H

#include <string>
#include <list>
#include <map>

/**
 * TextFileReader class for handling the loading and displaying of text file content.
 * 
 * This class provides methods to load the contents of a text file into memory and to display
 * the contents. The loaded file contents are stored in a `std::map`, where the key is the file name
 * and the value is a `std::list<std::string>` representing each line of the file.
 */
class TextFileReader {
private:
    /**
     * Map to store the content of loaded files.
     * The key is the file name, and the value is a list of strings representing the file content line by line.
     */
    std::map<std::string, std::list<std::string>> fileContents;  // Store loaded file content

public:
    /**
     * Method to load the content of a specific file.
     * Reads the file line by line and stores the content in memory.
     * 
     * @param filePath The full path to the file to be loaded.
     * @param fileName The name of the file to be used as the key in the map.
     * @return Returns `true` if the file was successfully loaded, `false` otherwise.
     */
    bool loadFile(const std::string& filePath, const std::string& fileName);

    /**
     * Method to print the content of a loaded file.
     * If the file has been loaded into memory, it prints the content line by line.
     * 
     * @param fileName The name of the file whose content is to be printed.
     */
    void printFileContent(const std::string& fileName);

    /**
     * Method to check if a file has been loaded.
     * 
     * @param fileName The name of the file to check.
     * @return Returns `true` if the file has been loaded, `false` otherwise.
     */
    bool isFileLoaded(const std::string& fileName);
};

#endif  // TEXTFILEREADER_H
