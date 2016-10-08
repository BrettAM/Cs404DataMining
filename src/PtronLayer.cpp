#include "PtronLayer.hpp"

#include <iostream>
using namespace std;

//public
PtronLayer::PtronLayer(int numInputs, int numOutputs, double learnRate, DecisionFunc* step)
        : step(step), lRate(learnRate), next(NULL),
          weights(numInputs+1, numOutputs),
          numInputs(numInputs), numOutputs(numOutputs) {
    weights.map([](double){ return randVal(); });
}
PtronLayer::PtronLayer(int numInputs, PtronLayer* next, double learnRate, DecisionFunc* step)
        : step(step), lRate(learnRate), next(next),
          weights(numInputs+1, next->numInputs),
          numInputs(numInputs), numOutputs(next->numInputs) {
    weights.map([](double){ return randVal(); });
}


Matrix PtronLayer::update(const Matrix& input, const Matrix& expected){
    auto fullInput = addBias(input); //(n)x(numInputs+1)
    auto values = step->apply(fullInput * weights);
    auto result = values.first; //(n)x(numOutputs)
    auto derivative = values.second; //(n)x(numOutputs)

    //(n)x(numOutputs)
    Matrix error = (next == NULL)? (expected - result)
                                 : next->update(result, expected);

    Matrix errorGrad = error.elmult(derivative); //(n)x(numOutputs)

    auto delta = fullInput.T() * errorGrad; //(numInputs+1)x(numOutputs)
    auto inputGrad = stripBias(errorGrad * weights.T()); //(n)x(numInputs)

    weights += (lRate * delta); //(numInputs+1)x(numOutputs)

    return inputGrad;
}

Matrix PtronLayer::apply(const Matrix& input){
    auto subresult = (*step)(addBias(input) * weights);
    return (next == NULL)? subresult : next->apply(subresult);
}

