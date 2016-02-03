#include "PtronLayer.hpp"

//public
PtronLayer::PtronLayer(int numInputs, int numOutputs, double learnRate, Decider step)
        : step(step), lRate(learnRate), weights(numInputs+1, numOutputs) {
    weights.map([](double){ return /*randVal*/(); });
}

void PtronLayer::update(const Matrix& input, const Matrix& expected){
    auto fullInput = addBias(input);
    auto result = calculate(fullInput); //(numCases)x(numInputs+1)
    auto erro   = (expected - result); //(numCases)x(numInputs+1)
    auto delta  = lRate * (fullInput.T() * error); //(numCases+1)x(numOutputs)
    weights += delta;
}

Matrix PtronLayer::apply(const Matrix& input){
    return calculate(addBais(input));
}

//private
Matrix PtronLayer::addBias(const Matrix& m){
    auto bias = Matrix(m.rows, 1).fill(-1);
    return (m|bias);
}

Matrix PtronLayer::calculate(const Matrix& input){
    return (input * weights).map(step);
}

