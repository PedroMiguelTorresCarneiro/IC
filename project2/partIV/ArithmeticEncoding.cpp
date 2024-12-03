#include "ArithmeticEncoding.h"

// Compute frequency table
std::map<int, int> ArithmeticEncoding::computeFrequencyTable(const std::vector<int>& data) {
    std::map<int, int> frequencyTable;
    for (int symbol : data) {
        frequencyTable[symbol]++;
    }
    return frequencyTable;
}

// Build probability ranges
std::map<int, ArithmeticEncoding::ProbabilityRange>
ArithmeticEncoding::buildProbabilityRanges(const std::map<int, int>& frequencyTable, size_t totalSymbols) {
    std::map<int, ProbabilityRange> probabilities;

    double cumulativeProbability = 0.0;
    for (const auto& [symbol, frequency] : frequencyTable) {
        double probability = static_cast<double>(frequency) / totalSymbols;
        probabilities[symbol] = {cumulativeProbability, cumulativeProbability + probability};
        cumulativeProbability += probability;
    }
    return probabilities;
}

long double ArithmeticEncoding::encode(const std::vector<int>& data,
                                  const std::map<int, ProbabilityRange>& probabilities) {
    long double low = 0.0;
    long double high = 1.0;

    for (int symbol : data) {
        auto it = probabilities.find(symbol);
        if (it == probabilities.end()) {
            std::cerr << "Symbol not found in probabilities: " << symbol << std::endl;
            continue;
        }
        const ProbabilityRange& range = it->second;
        long double rangeWidth = high - low;
        high = low + rangeWidth * range.high;
        low = low + rangeWidth * range.low;
    }

    return (low + high) / 2; // Return the midpoint
}

std::vector<int> ArithmeticEncoding::decode(long double encodedValue,
                                            const std::map<int, ProbabilityRange>& probabilities,
                                            size_t dataSize) {
    std::vector<int> decodedData;
    long double value = encodedValue;

    for (size_t i = 0; i < dataSize; ++i) {
        for (const auto& [symbol, range] : probabilities) {
            if (value >= range.low && value < range.high) {
                decodedData.push_back(symbol);
                value = (value - range.low) / (range.high - range.low);
                break;
            }
        }
    }

    return decodedData;
}

// Utility to print probability ranges
void ArithmeticEncoding::printProbabilityRanges(const std::map<int, ProbabilityRange>& probabilities) {
    std::cout << "Probability Ranges:\n";
    for (const auto& [symbol, range] : probabilities) {
        std::cout << "Symbol: " << symbol
                  << ", Range: [" << std::fixed << std::setprecision(6) << range.low
                  << ", " << range.high << ")\n";
    }
}
