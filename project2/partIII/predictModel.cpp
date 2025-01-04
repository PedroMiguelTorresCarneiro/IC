#include <vector>
#include <iostream>

//Calculate residuals using first order prediction
std::vector<int16_t> calculateResidualsMono(const std::vector<int16_t> &samples) {
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


std::vector<int16_t> calculateResidualsStereo(const std::vector<int16_t> &samples) {
    std::vector<int16_t> leftChannel, rightChannel;

    //split samples into 2 channels
    for (size_t i = 0; i < samples.size(); i += 2) {
        leftChannel.push_back(samples[i]);      
        rightChannel.push_back(samples[i + 1]); 
    }

    //calculate residuals for each channel
    std::vector<int16_t> leftResiduals = calculateResidualsMono(leftChannel);
    std::vector<int16_t> rightResiduals = calculateResidualsMono(rightChannel);

    // combine the channels
    std::vector<int16_t> residuals;
    for (size_t i = 0; i < leftResiduals.size(); ++i) {
        residuals.push_back(leftResiduals[i]);    
        residuals.push_back(rightResiduals[i]); 
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