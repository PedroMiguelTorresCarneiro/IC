#include <vector>
#include <iostream>

//Calculate Residuals using first order prediction
std::vector<int16_t> calculateResiduals(const std::vector<int16_t> &samples) {
    std::vector<int16_t> residuals;

    // Can't calculate residuals for the first sample, so just add it as it is
    residuals.push_back(samples[0]);

    for (size_t i = 1; i < samples.size(); ++i) {
        int predicted = samples[i - 1];  
        int residual = samples[i] - predicted; 
        residuals.push_back(residual);
    }

    return residuals;
}

//----------------------------------------------------------------------------------

//Reconstruct the original samples from the residuals (1st order)
std::vector<int16_t> reconstructSamples(const std::vector<int16_t> &residuals) {
    std::vector<int16_t> samples;

    // Again, the first sample is directly added
    samples.push_back(residuals[0]);

    for (size_t i = 1; i < residuals.size(); ++i) {
        int reconstructed = samples[i - 1] + residuals[i]; // Reconstruct sample
        samples.push_back(reconstructed);
    }

    return samples;
}