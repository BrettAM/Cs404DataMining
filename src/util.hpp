#pragma once
#include <cmath>
#include <functional>

typedef std::function<double(double)> Decider;

inline double step(double v){
    return (v < 0.0)? 0.0 : 1.0;
}

inline double sigmoid(double v){
    return 1.0 / (1.0 + exp(-v));
}

/** Return a step function that switches from 0.0 to 1.0 at `cutoff` */
std::function<double(double)> stepFunc(double cutoff){
    return [cutoff](double v) -> double { return step(v-cutoff); };
}

/** Return a sigmoid function with slope `slope` at 0.0 */
std::function<double(double)> sigmoidFunc(double slope){
    return [slope](double v) -> double { return sigmoid(v*slope); };
}
