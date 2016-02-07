#include "Input.hpp"

using namespace std;

ProblemSet readProblem(istream& input){
    int inputCount;
    input >> inputCount;
    Matrix train = readMatrix(input);
    Matrix challenge = readMatrix(input);
    auto lr = train.bisect(inputCount);
    int outputCount = train.cols - inputCount;
    return {inputCount, outputCount, lr.first, lr.second, challenge};
}

Matrix readMatrix(std::istream& input){
    int rows, cols;
    input >> rows;
    input >> cols;
    Matrix m(rows, cols);
    for(int r=0; r<rows; r++){
        for(int c=0; c<cols; c++){
            double val;
            input >> val;
            m.set(r,c, val);
        }
    }
    return m;
}
