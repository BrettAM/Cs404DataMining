#pragma once
#include "util.hpp"
#include "matrix.hpp"

class PtronLayer {
private:
    Decider step;
    double lRate;
    Matrix weights;
    /**
     * Append a -1 to each row of the input matrix `m`
     */
    Matrix addBias(const Matrix& m);
    /**
     * apply the perceptron to a biased input
     * input is a (n)x(numInputs+1) matrix
     * output is a (n)x(numOutputs) matrix
     */
    Matrix calculate(const Matrix& input);
public:
    PtronLayer(int numInputs, int numOutputs, double learnRate, Decider step);
    /**
     * train the perceptron on the input and corresponding expected data
     * input is a (numCases)x(numInputs) matrix
     * expected is a (numCases)x(numOutputs) matrix
     */
    void update(const Matrix& input, const Matrix& expected);
    /**
     * apply the perceptron to a set of input row vectors
     * input is a (n)x(numInputs) matrix
     * output is a (n)x(numOutputs) matrix
     */
    Matrix apply(const Matrix& input);
};
