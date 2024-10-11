#ifndef TEXTFILEREADER_H
#define TEXTFILEREADER_H

#include <string>
#include <list>
#include <map>

class TextFileReader {
private:
    /**
     * Map to store the content of loaded files.
     * The key is the file name, and the value is a list of strings representing the file content line by line.
     */
    std::map<std::string, std::list<std::string>> fileContents;  // Store loaded file content

public:
    bool loadFile(const std::string& filePath,std::string& loadedFileName);
    void printFileContent(const std::string& fileName);
    bool isFileLoaded(const std::string& fileName);
    void convertToLowercase(const std::string& fileName);
    void removePunctuation(const std::string& fileName);
    void calculateCharsFrequency(const std::string& fileName);
    void calculateWordsFrequency(const std::string& fileName);
    void calculateNgramsFrequency(const std::string& fileName, int n);
    std::vector<std::string> tokenize(const std::string& line);
};

#endif  // TEXTFILEREADER_H
