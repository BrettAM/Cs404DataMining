#pragma once
#include "util.hpp"
#include "matrix.hpp"
#include <random>

class PtronLayer {
private:
    Decider* step;
    PtronLayer* next;
    double lRate;
    Matrix weights;
    /**
     * Append a -1 to each row of the input matrix `m`
     */
    Matrix addBias(const Matrix& m);
public:
    const int numInputs;
    const int numOutputs;
    PtronLayer(int numInputs, int numOutputs, double learnRate, Decider* step);
    PtronLayer(int numInputs, PtronLayer* next, double learnRate, Decider* step);
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
