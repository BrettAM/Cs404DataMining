#include <iostream>
#include <fstream>
#include "matrix.hpp"
#include "PtronLayer.hpp"
#include "util.hpp"
#include "Input.hpp"
using namespace std;

int main(int argc, char const *argv[]) {
    ProblemSet ps = readProblem(cin);
    PtronLayer p(ps.inputCount, ps.outputCount, 0.05, sigmoidFunc(20.0));

    //cout << (ps.trainingInput | ps.trainingOutput) << "\n" << ps.challenge << endl;

    auto norm = columnNormalizer(ps.trainingInput);

    Matrix trainingInput = norm(ps.trainingInput);
    Matrix challenge = norm(ps.challenge);

    for(int i=0; i<200000; i++){
        p.update(trainingInput, ps.trainingOutput);
    }

    cout << "W matrix:\n" << p.getWeights() << endl;
    cout << "BEGIN TESTING" << endl;
    cout << (challenge|p.apply(challenge));

    return 0;
}
