#ifndef HUFFMANCODING_H
#define HUFFMANCODING_H

#include <map>
#include <vector>
#include <string>
#include <queue>
#include <iostream>

class HuffmanCoding {
private:
    struct Node {
        int value;
        int frequency;
        Node* left;
        Node* right;

        Node(int val, int freq) : value(val), frequency(freq), left(nullptr), right(nullptr) {}
    };

    Node* root;
    std::map<int, std::string> huffmanCodes;

    void generateCodes(Node* node, const std::string& code);
    void clearTree(Node* node);

public:
    HuffmanCoding();
    ~HuffmanCoding();

    void buildTree(const std::map<int, int>& frequencies); // Build Huffman Tree
    std::string encode(const std::vector<int>& input); // Encode input integers
    std::vector<int> decode(const std::string& encoded); // Decode encoded string
    void printCodes(); // For debugging
};

#endif
