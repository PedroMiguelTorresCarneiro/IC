#ifndef PREDICT_MODEL_H
#define PREDICT_MODEL_H
#include <vector>
#include <iostream>

std::vector<int16_t> calculateResidualsStereo(const std::vector<int16_t> &samples);
std::vector<int16_t> calculateResidualsMono(const std::vector<int16_t> &samples);
std::vector<int16_t> calculateResidualsStereo2(const std::vector<int16_t> &samples);
std::vector<int16_t> calculateResidualsMono2(const std::vector<int16_t> &samples);
std::vector<int16_t> reconstructSamples(const std::vector<int16_t> &residuals);
std::vector<int16_t> reconstructSamples2(const std::vector<int16_t> &residuals);

#endif