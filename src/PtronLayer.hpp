#pragma once
#include "util.hpp"
#include "DecisionFunc.hpp"
#include "matrix.hpp"

class PtronLayer {
private:
    DecisionFunc* step;
    double lRate;
    PtronLayer* next;
    Matrix weights;
public:
    const int numInputs;
    const int numOutputs;
    PtronLayer(int numInputs, int numOutputs, double learnRate, DecisionFunc* step);
    PtronLayer(int numInputs, PtronLayer* next, double learnRate, DecisionFunc* step);
    Matrix getWeights() { return Matrix(weights); }
    /**
     * train the perceptron on the input and corresponding expected data
     * input is a (numCases)x(numInputs) matrix
     * expected is a (numCases)x(numOutputs) matrix
     * returns the error gradient matrix
     */
    Matrix update(const Matrix& input, const Matrix& expected);
    /**
     * apply the perceptron to a set of input row vectors
     * input is a (n)x(numInputs) matrix
     * output is a (n)x(numOutputs) matrix
     */
    Matrix apply(const Matrix& input);
};
