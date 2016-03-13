#include <iostream>
#include <fstream>
#include "matrix.hpp"
#include "DecisionFunc.hpp"
#include "PtronLayer.hpp"
#include "util.hpp"
#include "Input.hpp"
using namespace std;

Sigmoid sig(40.0);

int main(int argc, char const *argv[]) {
    ProblemSet ps = readProblem(cin);

    auto norm = columnNormalizer(ps.trainingInput);
    Matrix trainingInput = norm(ps.trainingInput);
    Matrix challenge = norm(ps.challenge);

    cout << "input: " << ps.inputCount << " hidden: " << ps.hiddenCount <<
                " output: " << ps.outputCount << endl << endl;

    PtronLayer output(ps.hiddenCount, ps.outputCount, 0.01, &sig);
    /*PtronLayer middle(ps.hiddenCount, &output, 0.01, &sig);
    PtronLayer hidden(ps.inputCount, &middle, 0.01, &sig);*/
    PtronLayer hidden(ps.inputCount, &output, 0.01, &sig);

    for(size_t i=0; i<50000; i++){
        hidden.update(trainingInput, ps.trainingOutput);
    }

    cout << "hidden W matrix:\n" << hidden.getWeights() << endl;
    cout << "output W matrix:\n" << output.getWeights() << endl;
    cout << "BEGIN TESTING" << endl;
    cout << (ps.challenge | hidden.apply(challenge));

    return 0;
}
