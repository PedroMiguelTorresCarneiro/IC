#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "GolombCoding.h"
#include "BitStream.h"

void printUsage() {
    std::cout << "Usage:\n"
              << "-Golomb -load <TXT_PATH> -SignMag|-PosNeg [-m <NUM>]\n"
              << "Example:\n"
              << "-Golomb -load \"../.../inputINT.txt\" -SignMag\n"
              << "-Golomb -load \"../.../inputINT.txt\" -PosNeg -m 6\n";
}

// Function to read integers from a file and calculate optimal m
int calculateOptimalM(const std::vector<int>& values) {
    if (values.empty()) {
        std::cerr << "No values provided to calculate optimal m." << std::endl;
        return 1; // Default m value (cannot be 0)
    }

    double sum = 0;
    for (int value : values) {
        sum += std::abs(value);
    }

    double mean = sum / values.size();

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

int main(int argc, char* argv[]) {
    try {
        if (argc < 5 || std::string(argv[1]) != "-Golomb" || std::string(argv[2]) != "-load") {
            printUsage();
            return 1;
        }

        std::string filePath = argv[3];
        std::string codingType;
        int mValue = 0;

        if (argc >= 5) {
            codingType = argv[4];
            if (codingType != "-SignMag" && codingType != "-PosNeg") {
                std::cerr << "Error: Invalid coding type. Use -SignMag or -PosNeg." << std::endl;
                printUsage();
                return 1;
            }
        } else {
            std::cerr << "Error: Coding type is required." << std::endl;
            printUsage();
            return 1;
        }

        if (argc == 7 && std::string(argv[5]) == "-m") {
            try {
                mValue = std::stoi(argv[6]);
                if (mValue <= 0) {
                    throw std::invalid_argument("mValue must be positive.");
                }
            } catch (const std::exception&) {
                std::cerr << "Error: Invalid value for m." << std::endl;
                printUsage();
                return 1;
            }
        } else if (argc > 5 && argc != 7) {
            std::cerr << "Error: Unexpected arguments." << std::endl;
            printUsage();
            return 1;
        }

        std::vector<int> valuesToEncode = readIntegersFromFile(filePath);
        if (valuesToEncode.empty()) {
            std::cerr << "No values to encode found in " << filePath << std::endl;
            return 1;
        }

        if (mValue == 0) {
            mValue = calculateOptimalM(valuesToEncode);
        }

        std::string encodedFile = "encoded_golomb.dat";
        BitStream writeStream(encodedFile, true);
        GolombCoding encoder(mValue, writeStream, codingType == "-PosNeg" ? GolombCoding::POS_NEG : GolombCoding::SIGN_MAG);

        std::cout << "Encoding values from " << filePath << ":" << std::endl;
        for (int value : valuesToEncode) {
            std::cout << "Encoding value: " << value << std::endl;
            encoder.encode(value);
        }
        writeStream.close();

        displayEncodedData(encodedFile);

        BitStream readStream(encodedFile, false);
        GolombCoding decoder(mValue, readStream, codingType == "-PosNeg" ? GolombCoding::POS_NEG : GolombCoding::SIGN_MAG);

        std::cout << "Decoded values:" << std::endl;
        for (size_t i = 0; i < valuesToEncode.size(); ++i) {
            std::cout << decoder.decode() << std::endl;
        }
        readStream.close();

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
