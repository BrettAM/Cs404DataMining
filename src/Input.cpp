#include "Input.hpp"

using namespace std;

ProblemSet readProblem(string file){
    ifstream input(file, ifstream::in);
    if(!input.is_open()){
        throw runtime_error("Can't read problem file");
    }
    int inputCount, irows, icols;
    input >> inputCount;
    input >> irows;
    input >> icols;
    Matrix train(irows, icols);
    for(int r=0; r<irows; r++){
        for(int c=0; c<icols; c++){
            double val;
            input >> val;
            train.set(r,c, val);
        }
    }
    int crows, ccols;
    input >> crows;
    input >> ccols;
    Matrix challenge(crows, ccols);
    for(int r=0; r<crows; r++){
        for(int c=0; c<ccols; c++){
            double val;
            input >> val;
            challenge.set(r,c, val);
        }
    }
    auto lr = train.bisect(inputCount);
    return {inputCount, icols-inputCount, lr.first, lr.second, challenge};
}
