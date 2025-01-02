#ifndef PREDICT_MODEL_H
#define PREDICT_MODEL_H
#include <vector>
#include <iostream>

std::vector<int16_t> calculateResiduals(const std::vector<int16_t> &samples);
std::vector<int16_t> reconstructSamples(const std::vector<int16_t> &residuals);

#endif