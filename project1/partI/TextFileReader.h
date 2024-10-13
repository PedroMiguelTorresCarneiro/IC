#ifndef TEXTFILEREADER_H
#define TEXTFILEREADER_H

#include <string>
#include <list>
#include <map>

using namespace std;

class TextFileReader {
private:
    /**
     * Map to store the content of loaded files.
     * The key is the file name, and the value is a list of strings representing the file content line by line.
     */
    map<string, list<string>> fileContents;  // Store loaded file content

public:

    /**
     * Loads a file and stores its content line by line.
     * @param filePath: The path to the file to be loaded.
     * @param loadedFileName: A reference to the variable where the file's name will be stored.
     * @return: True if the file was successfully loaded, otherwise false.
     */
    bool loadFile(const string& filePath,string& loadedFileName);

    /**
     * Prints the content of the file loaded under the given file name.
     * @param fileName: The name of the file whose content is to be printed.
     */
    void printFileContent(const string& fileName);

    /**
     * Checks if a file has already been loaded.
     * @param fileName: The name of the file to check.
     * @return: True if the file is loaded, otherwise false.
     */
    bool isFileLoaded(const string& fileName);

    /**
     * Converts all text in the file to lowercase.
     * @param fileName: The name of the file to be processed.
     */
    void convertToLowercase(const string& fileName);

    /**
     * Removes all punctuation marks from the text in the file.
     * @param fileName: The name of the file to be processed.
     */
    void removePunctuation(const string& fileName);

     /**
     * Calculates the frequency of each character in the file.
     * @param fileName: The name of the file to analyze.
     */
    void calculateCharsFrequency(const string& fileName);

     /**
     * Calculates the frequency of each word in the file.
     * @param fileName: The name of the file to analyze.
     */
    void calculateWordsFrequency(const string& fileName);

    /**
     * Calculates the frequency of N-grams in the file's text.
     * N-grams are sequences of 'n' consecutive words.
     * @param fileName: The name of the file to analyze.
     * @param n: The size of the N-gram (e.g., bigrams for n=2, trigrams for n=3).
     */
    void calculateNgramsFrequency(const string& fileName, int n);

      /**
     * Tokenizes a line of text into individual words.
     * @param line: The line to tokenize.
     * @return: A vector of strings, where each string is a token (word).
     */
    vector<string> tokenize(const string& line);

    string guessEncoding(const string& filePath);
};

#endif  // TEXTFILEREADER_H
