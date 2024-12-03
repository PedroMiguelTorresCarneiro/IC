#ifndef ARITHMETIC_ENCODING_H
#define ARITHMETIC_ENCODING_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

class ArithmeticEncoding {
public:
    struct ProbabilityRange {
        double low;
        double high;
    };

    // Constructor
    ArithmeticEncoding() = default;

    // Method to compute frequency table
    std::map<int, int> computeFrequencyTable(const std::vector<int>& data);

    // Method to build probability ranges
    std::map<int, ProbabilityRange> buildProbabilityRanges(const std::map<int, int>& frequencyTable, size_t totalSymbols);

    // Encode data
    long double encode(const std::vector<int>& data, const std::map<int, ProbabilityRange>& probabilities);

    // Decode data
    std::vector<int> decode(long double encodedValue, const std::map<int, ProbabilityRange>& probabilities, size_t dataSize);

    // Utility to display probability ranges
    void printProbabilityRanges(const std::map<int, ProbabilityRange>& probabilities);

private:
    static constexpr double PRECISION = 1e-10; // Precision for arithmetic operations
};

#endif // ARITHMETIC_ENCODING_H
