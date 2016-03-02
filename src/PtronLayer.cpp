#include "PtronLayer.hpp"

#include <iostream>
using namespace std;

//public
PtronLayer::PtronLayer(int numInputs, int numOutputs, double learnRate, DecisionFunc* step)
        : step(step), lRate(learnRate), next(NULL),
          numInputs(numInputs), numOutputs(numOutputs), weights(numInputs+1, numOutputs) {
    weights.map([](double){ return randVal(); });
}
PtronLayer::PtronLayer(int numInputs, PtronLayer* next, double learnRate, DecisionFunc* step)
        : step(step), lRate(learnRate), next(next),
          numInputs(numInputs), numOutputs(next->numInputs), weights(numInputs+1, next->numInputs) {
    weights.map([](double){ return randVal(); });
}


Matrix PtronLayer::update(const Matrix& input, const Matrix& expected){
    //cerr << input << endl;

    auto fullInput = addBias(input); //(n)x(numInputs+1)
    //cerr << "fullInput" << endl;
    auto values = step->apply(fullInput * weights);
    //cerr << "values" << endl;
    auto result = values.first; //(n)x(numOutputs)
    //cerr << "result" << endl;
    auto derivative = values.second; //(n)x(numOutputs)
    //cerr << "derivative" << endl;

    //(n)x(numOutputs)
    Matrix error = (next == NULL)? (expected - result)
                                 : next->update(result, expected);

    //cerr << "error" << endl;
    Matrix errorGrad = error.elmult(derivative); //(n)x(numOutputs)
    //cerr << "errorGrad" << endl;

    auto delta = fullInput.T() * errorGrad; //(numInputs+1)x(numOutputs)
    //cerr << "delta" << endl;
    auto inputGrad = stripBias(errorGrad * weights.T()); //(n)x(numInputs)
    //cerr << "inputGrad" << endl;

    weights += (lRate * delta); //(numInputs+1)x(numOutputs)
    //cerr << "weights" << endl;

    return inputGrad;
}

Matrix PtronLayer::apply(const Matrix& input){
    auto subresult = (*step)(addBias(input) * weights);
    return (next == NULL)? subresult : next->apply(subresult);
}

