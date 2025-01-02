#include "HuffmanCoding.h"

// Constructor
HuffmanCoding::HuffmanCoding() : root(nullptr) {}

// Destructor
HuffmanCoding::~HuffmanCoding() {
    clearTree(root);
}

// Clear the tree to prevent memory leaks
void HuffmanCoding::clearTree(Node* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// Generate Huffman codes recursively
void HuffmanCoding::generateCodes(Node* node, const std::string& code) {
    if (!node) return;

    if (!node->left && !node->right) { // Leaf node
        huffmanCodes[node->value] = code;
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

// Build the Huffman Tree
void HuffmanCoding::buildTree(const std::map<int, int>& frequencies) {
    auto comp = [](Node* left, Node* right) { return left->frequency > right->frequency; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> minHeap(comp);

    for (const auto& pair : frequencies) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        Node* parent = new Node(-1, left->frequency + right->frequency); // Internal node with no value (-1)
        parent->left = left;
        parent->right = right;
        minHeap.push(parent);
    }

    root = minHeap.top();
    generateCodes(root, "");
}

// Encode a vector of integers
std::string HuffmanCoding::encode(const std::vector<int>& input) {
    std::string encoded;
    for (int value : input) {
        encoded += huffmanCodes[value];
    }
    return encoded;
}

// Decode an encoded string back into integers
std::vector<int> HuffmanCoding::decode(const std::string& encoded) {
    std::vector<int> decoded;
    Node* currentNode = root;
    for (char bit : encoded) {
        currentNode = (bit == '0') ? currentNode->left : currentNode->right;

        if (!currentNode->left && !currentNode->right) { // Leaf node
            decoded.push_back(currentNode->value);
            currentNode = root;
        }
    }
    return decoded;
}

// Print the Huffman codes (for debugging)
void HuffmanCoding::printCodes() {
    for (const auto& pair : huffmanCodes) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}
