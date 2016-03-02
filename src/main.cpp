#include <iostream>
#include <fstream>
#include "matrix.hpp"
#include "PtronLayer.hpp"
#include "util.hpp"
#include "Input.hpp"
using namespace std;

namespace{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1, 1);
    double initialVal(){
        return dis(gen);
    }
}
/**
 * input: matrix after application of sigmoid function to A
 * output: matrix of same dim representing the sigmoid derivative at A
 */
Matrix sigmoidDeriv(const Matrix& input){
    Matrix res = input;
    res.map([](double v) -> double {return 1.0 - v;});
    return res.elmult(input);
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

SigmoidFunc DecisionFunc(10.0);

void train(Matrix& input, Matrix& expected, Matrix& hiddenW, Matrix& outputW){
        //trainingInput = (n)x(inputCount+1)

        Matrix hiddenResult = addBias(DecisionFunc(input * hiddenW));
        // (n)x(hiddenCount+1)
        Matrix finalResult  = DecisionFunc(hiddenResult * outputW);
        // (n)x(outputCount)

        //cout << "hiddenResult: " << hiddenResult << endl;
        //cout << "finalResult: " << finalResult << endl;

        // derivatives at each layer - elementwise calculation of (1.0 - elem) * elem
        Matrix hiddenDeriv = sigmoidDeriv(hiddenResult); // (n)x(hiddenCount+1)
        Matrix outputDeriv = sigmoidDeriv(finalResult); // (n)x(outputCount)

        //cout << "hiddenDeriv: " << hiddenDeriv << endl;
        //cout << "outputDeriv: " << outputDeriv << endl;

        Matrix errorMat = (expected - finalResult); // (n)x(outputCount)

        //cout << "errorMat: " << errorMat << endl;
        //backprop steps below

        Matrix deltao = errorMat.elmult(outputDeriv);
            // (n)x(outputCount)
        Matrix deltah = (deltao*(outputW.T())).elmult(hiddenDeriv);
            // (n)x(hiddenCount+1)

        //cout << "deltao: " << deltao << endl;
        //cout << "deltah: " << deltah << endl;

        Matrix outputDelta = (hiddenResult.T()) * deltao;
            // (hiddenCount+1)x(outputCount)

        //cout << "outputDelta: " << outputDelta << endl;

        Matrix hiddenDelta = (input.T()) * stripBias(deltah);
            // (inputCount+1)x(hiddenCount)

        //cout << "hiddenDelta: " << hiddenDelta << endl;

        hiddenW += 0.0001 * (hiddenDelta);
        outputW += 0.0001 * (outputDelta);
}

int main(int argc, char const *argv[]) {
    ProblemSet ps = readProblem(cin);

    //cout << (ps.trainingInput | ps.trainingOutput) << "\n" << ps.challenge << endl;

    auto norm = columnNormalizer(ps.trainingInput);
    Matrix trainingInput = addBias(norm(ps.trainingInput));
    Matrix challenge = addBias(norm(ps.challenge));

    Matrix hiddenW(ps.inputCount+1, ps.hiddenCount);
    Matrix outputW(ps.hiddenCount+1, ps.outputCount);
    hiddenW.map([](double){ return initialVal(); });
    outputW.map([](double){ return initialVal(); });

    cout << "TrainingInput: " << trainingInput << endl;

    for(size_t i=0; i<2000; i++){
        train(trainingInput, ps.trainingOutput, hiddenW, outputW);
    }

    cout << "hidden W matrix:\n" << hiddenW << endl;
    cout << "output W matrix:\n" << outputW << endl;
    cout << "BEGIN TESTING" << endl;

    Matrix hiddenResult = addBias(DecisionFunc(challenge * hiddenW));
    Matrix finalResult  = DecisionFunc(hiddenResult * outputW);

    cout << (stripBias(challenge) | finalResult);

    return 0;
}
