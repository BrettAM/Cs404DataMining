#pragma once
#include <cmath>
#include <functional>

double step(double v){
    return (v < 0.0)? 0.0 : 1.0;
}

std::function<double(double)> stepFunc(double cutoff){
    return [cutoff](double v) -> double { return step(v+cutoff); };
}

double sigmoid(double v){
    return 1.0 / (1.0 + exp(-v));
}

std::function<double(double)> sigmoidFunc(double slope){
    return [slope](double v) -> double { return sigmoid(v*slope); };
}
