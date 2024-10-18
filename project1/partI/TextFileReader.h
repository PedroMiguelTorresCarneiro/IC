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
    map<string, list<vector<string>>> fileContents;  // Store loaded file content

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
     * Removes duplicated spaces from the text in the file.
     * @param line: The line of text to process.
     * @return: A vector of strings, where each string is a token (word).
     */
    vector<string> removeDuplicatedSpaces(const vector<string>& line);

    /**
     * Removes all numbers from the text in the file.
     * @param fileName: The name of the file to be processed.
     */
    void removeNumbers(const string& fileName);

     /**
     * Calculates the frequency of each character in the file.
     * @param fileName: The name of the file to analyze.
     */
    void calculateCharsFrequency(const string& fileName, bool histogram);

     /**
     * Calculates the frequency of each word in the file.
     * @param fileName: The name of the file to analyze.
     */
    void calculateWordsFrequency(const string& fileName, bool histogram);

    /**
     * Calculates the frequency of N-grams in the file's text.
     * N-grams are sequences of 'n' consecutive words.
     * @param fileName: The name of the file to analyze.
     * @param n: The size of the N-gram (e.g., bigrams for n=2, trigrams for n=3).
     */
    void calculateNgramsFrequency(const string& fileName, int n, bool histogram);

    /**
     * Tokenizes a line of text into individual words.
     * @param line: The line to tokenize.
     * @return: A vector of strings, where each string is a token (word).
     */
    vector<string> tokenize(const vector<string>& line);

    /**
     * Sorts a map by its values in descending order.
     * @param wordMap: A map containing words and their frequencies.
     * @return: A vector of pairs, where each pair contains a word and its frequency.
     */
    vector<pair<string, int>> sortMapByValue(const map<string, int>& wordMap);

    /**
     * Prints a map of words/chars/ngrams and their frequencies.
     * @param sortedMap: A vector of pairs, where each pair contains a word and its frequency.
     */
    void printAlignedMap(const vector<pair<string, int>>& sortedMap);

    /**
     * Calls a Python script to generate a histogram of the given data.
     * @param scriptName: The name of the Python script to execute.
     * @param mode: The type of data to be processed (e.g., chars, words, ngrams).
     * @param data: The data to be processed by the Python script.
     * @return: A string containing the output of the Python script.
     */
    void executePythonScript(const string& scriptName, const string& mode, const string& data);    
    
    /**
     * Converts a map of words/chars/ngrams and their frequencies to a string.
     * @param frequencyMap: A map containing words/chars/ngrams and their frequencies.
     * @return: A string representation of the map.
     */
    string mapToString(const map<string, int>& frequencyMap);
    
    //string guessEncoding(const string& filePath);

    int utf8CharBytes(unsigned char byte);
};

#endif  // TEXTFILEREADER_H
