#include <iostream>
#include <fstream>
#include <vector>
#include "GolombCoding.h"
#include "BitStream.h"

// Function to read integers from a file and calculate optimal m
int calculateOptimalM(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return -1; // Return an error indicator
    }

    std::vector<int> values;
    int value;
    double sum = 0;
    size_t count = 0;

    // Read values and calculate sum and count
    while (inputFile >> value) {
        values.push_back(value);
        sum += std::abs(value); // Using absolute values since Golomb coding handles positive integers
        ++count;
    }
    inputFile.close();

    if (count == 0) {
        std::cerr << "No values found in the file." << std::endl;
        return 1; // Default m value (cannot be 0)
    }

    // Calculate mean
    double mean = sum / count;

    // Calculate nearest power of 2 greater than or equal to the mean
    int optimalM = 1;
    while (optimalM < mean) {
        optimalM *= 2;
    }
    return optimalM;
}


// Function to read integers from a file
std::vector<int> readIntegersFromFile(const std::string& filePath) {
    std::vector<int> values;
    std::ifstream inputFile(filePath);
    if (inputFile.is_open()) {
        int value;
        while (inputFile >> value) {
            values.push_back(value);
        }
        inputFile.close();
    } else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
    return values;
}

// Function to display encoded data
void displayEncodedData(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (file.is_open()) {
        std::cout << "Encoded data in " << filePath << " (as binary):" << std::endl;
        char byte;
        while (file.get(byte)) {
            for (int i = 7; i >= 0; --i) {
                std::cout << ((byte >> i) & 1); // Print each bit
            }
            std::cout << ' '; // Space between bytes for readability
        }
        std::cout << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to open file for displaying encoded data: " << filePath << std::endl;
    }
}

int main() {
    try {
        // Read integers from inputINT.txt
        std::vector<int> valuesToEncode = readIntegersFromFile("inputINT.txt");
        if (valuesToEncode.empty()) {
            std::cerr << "No values to encode found in inputINT.txt." << std::endl;
            return 1;
        }

        // Initialize BitStream for writing to a file
        BitStream writeStream("test_golomb.dat", true);

        // Create a GolombCoding object for encoding with m = 5 and SIGN_MAG mode
        GolombCoding encoder(5, writeStream, GolombCoding::SIGN_MAG);

        // Encode the values read from inputINT.txt
        std::cout << "Encoding values from inputINT.txt:" << std::endl;
        for (int value : valuesToEncode) {
            std::cout << "Encoding value: " << value << std::endl;
            encoder.encode(value);
        }

        // Close the writing stream
        writeStream.close();

        // Display the encoded data
        displayEncodedData("test_golomb.dat");

        // Initialize BitStream for reading from the same file
        BitStream readStream("test_golomb.dat", false);

        // Create a GolombCoding object for decoding
        GolombCoding decoder(5, readStream, GolombCoding::SIGN_MAG);

        // Decode and print the values
        std::cout << "Decoded values:" << std::endl;
        for (size_t i = 0; i < valuesToEncode.size(); ++i) {
            std::cout << decoder.decode() << std::endl;
        }

        // Close the reading stream
        readStream.close();

        // Repeat similar test for POS_NEG mode
        BitStream writeStream2("test_golomb_pos_neg.dat", true);
        GolombCoding encoder2(33, writeStream2, GolombCoding::POS_NEG);

        std::cout << "\nEncoding values using POS_NEG mode from inputINT.txt:" << std::endl;
        for (int value : valuesToEncode) {
            std::cout << "Encoding value: " << value << std::endl;
            encoder2.encode(value);
        }

        writeStream2.close();

        // Display the encoded data
        displayEncodedData("test_golomb_pos_neg.dat");

        BitStream readStream2("test_golomb_pos_neg.dat", false);
        GolombCoding decoder2(33, readStream2, GolombCoding::POS_NEG);

        std::cout << "Decoded values using POS_NEG mode:" << std::endl;
        for (size_t i = 0; i < valuesToEncode.size(); ++i) {
            std::cout << decoder2.decode() << std::endl;
        }

        readStream2.close();

        std::string filePath = "inputINT.txt";
        int optimalM = calculateOptimalM(filePath);

        if (optimalM > 0) {
            std::cout << "Optimal m value based on data in " << filePath << " is: " << optimalM << std::endl;
        } else {
            std::cerr << "Error calculating optimal m." << std::endl;
        }

        // Repeat similar test for POS_NEG mode
        BitStream writeStream3("test_golomb_pos_negOptimun.dat", true);
        GolombCoding encoder3(optimalM, writeStream3, GolombCoding::POS_NEG);

        std::cout << "\nEncoding values using POS_NEG mode from inputINT.txt:" << std::endl;
        for (int value : valuesToEncode) {
            std::cout << "Encoding value: " << value << std::endl;
            encoder3.encode(value);
        }

        writeStream3.close();

        // Display the encoded data
        displayEncodedData("test_golomb_pos_negOptimun.dat");

        BitStream readStream3("test_golomb_pos_negOptimun.dat", false);
        GolombCoding decoder3(optimalM, readStream2, GolombCoding::POS_NEG);

        std::cout << "Decoded values using POS_NEG mode:" << std::endl;
        for (size_t i = 0; i < valuesToEncode.size(); ++i) {
            std::cout << decoder3.decode() << std::endl;
        }

        readStream3.close();

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    

    return 0;
}
