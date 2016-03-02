#include "PtronLayer.hpp"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-0.5, 0.5);
double randVal(){
    return dis(gen);
}

//public
PtronLayer::PtronLayer(int numInputs, int numOutputs, double learnRate, Decider* step)
        : step(step), lRate(learnRate), next(NULL),
          numInputs(numInputs), numOutputs(numOutputs), weights(numInputs+1, numOutputs) {
    weights.map([](double){ return randVal(); });
}
PtronLayer::PtronLayer(int numInputs, PtronLayer* next, double learnRate, Decider* step)
        : step(step), lRate(learnRate), next(next),
          numInputs(numInputs), numOutputs(next->numInputs), weights(numInputs+1, numOutputs) {
    weights.map([](double){ return randVal(); });
}

Matrix PtronLayer::update(const Matrix& input, const Matrix& expected){
    auto fullInput = addBias(input); //(numCases)x(numInputs+1)
    auto values = step->apply(fullInput * weights);
    auto result = values.left;
    auto derivative = values.right;

    Matrix errorGrad;
    if(next != NULL){
        errorGrad = next->update(result, expected);
    } else {
        errorGrad = (expected - finalResult).elmult(derivative);
    }

    auto delta = fullInput.T() * errorGrad;
    auto inputGrad = delta * weights.T();

    weights += (lRate * delta);

    return inputGrad;
}

Matrix PtronLayer::apply(const Matrix& input){
    auto subresult = (*step)(addBias(input) * weights);
    return (next == NULL)? subresult : next->apply(subreslt);
}

//private
Matrix PtronLayer::addBias(const Matrix& m){
    auto bias = Matrix(m.rows, 1, -1.0);
    return (m|bias);
}
