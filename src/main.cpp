#include <iostream>
#include <fstream>
#include "matrix.hpp"
#include "PtronLayer.hpp"
#include "util.hpp"
#include "Input.hpp"
using namespace std;

/**
 * input: matrix after application of sigmoid function to A
 * output: matrix of same dim representing the sigmoid derivative at A
 */
Matrix sigmoidDeriv(const Matrix& input){
    Matrix res = input;
    res.map([](double v) -> double {return 1.0 - v;});
    return res.hadamard(input);
}
/**
 * input: (n)x(m) matrix
 * output: (n)x(m+1) matrix, the input concatenated with a column of "-1"
 */
Matrix addBias(const Matrix& m){
    auto bias = Matrix(m.rows, 1, -1.0);
    return (m|bias);
}

Matrix stripBias(const Matrix& m){
    return m.bisect(m.cols-1).first;
}

int main(int argc, char const *argv[]) {
    ProblemSet ps = readProblem(cin);

    SigmoidFunc DecisionFunc(20.0);
    PtronLayer hidden(ps.inputCount,  ps.hiddenCount, 0.05, &DecisionFunc);
    PtronLayer output(ps.hiddenCount, ps.outputCount, 0.05, &DecisionFunc);

    //cout << (ps.trainingInput | ps.trainingOutput) << "\n" << ps.challenge << endl;

    auto norm = columnNormalizer(ps.trainingInput);

    Matrix trainingInput = addBias(norm(ps.trainingInput));
    Matrix challenge = addBias(norm(ps.challenge));

/*void PtronLayer::update(const Matrix& input, const Matrix& expected){
    auto fullInput = addBias(input);
    auto result = calculate(fullInput); //(numCases)x(numInputs+1)
    auto error  = (expected - result); //(numCases)x(numInputs+1)
    auto delta  = lRate * (fullInput.T() * error); //(numInputs+1)x(numOutputs)
    update(delta);
}
*/
    Matrix hiddenW(ps.inputCount+1, ps.hiddenCount);
    Matrix outputW(ps.hiddenCount+1, ps.outputCount);
    hiddenW.map([](double){ return randVal(); });
    outputW.map([](double){ return randVal(); });

    for(int i=0; i<20000; i++){
        //trainingInput = (n)x(inputCount+1)
        Matrix hiddenResult =

        // results at each layer
        Matrix subresult = hidden.apply(trainingInput); // (n)x(hiddenCount)
        Matrix result    = output.apply(subresult); // (n)x(outputCount)

        //cout << "subresult: " << subresult << endl;
        //cout << "result: " << result << endl;

        // derivatives at each layer - elementwise calculation of (1.0 - elem) * elem
        Matrix srDeriv = sigmoidDeriv(addBias(subresult)); // (n)x(hiddenCount+1)
        Matrix  rDeriv = sigmoidDeriv(result); // (n)x(outputCount)

        //cout << "srDeriv: " << srDeriv << endl;
        //cout << "rDeriv: " << rDeriv << endl;

        Matrix errorMat  = (ps.trainingOutput - result); // (n)x(outputCount)

        //cout << "errorMat: " << errorMat << endl;
        //backprop

        Matrix deltao = errorMat.hadamard(rDeriv); // (n)x(outputCount)
        Matrix deltah = (deltao*output.getWeights().T()).hadamard(srDeriv); // (n)x(hiddenCount+1)

        //cout << "deltao: " << deltao << endl;
        //cout << "deltah: " << deltah << endl;

        Matrix outputDelta = addBias(subresult).T() * deltao;
            // (hiddenCount+1)x(outputCount)

        //cout << "outputDelta: " << outputDelta << endl;

        Matrix hiddenDelta = addBias(trainingInput).T() * stripBias(deltah);
            // (inputCount+1)x(hiddenCount)

        //cout << "hiddenDelta: " << hiddenDelta << endl;

        output.update(outputDelta);
        hidden.update(hiddenDelta);
    }

    cout << "hidden W matrix:\n" << hidden.getWeights() << endl;
    cout << "output W matrix:\n" << output.getWeights() << endl;
    cout << "BEGIN TESTING" << endl;
    cout << (challenge | output.apply(hidden.apply(challenge)));

    //cout << (challenge|p.apply(challenge));

    return 0;
}
