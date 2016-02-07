#include <iostream>
#include <fstream>
#include "matrix.hpp"
#include "PtronLayer.hpp"
#include "util.hpp"
#include "Input.hpp"
using namespace std;

int main(int argc, char const *argv[]) {
    ProblemSet ps = readProblem(cin);
    PtronLayer p(ps.inputCount, ps.outputCount, 0.25, stepFunc(0.5));

    //cout << ps.trainingInput << "\n" << ps.trainingOutput << "\n" << ps.challenge;

    for(int i=0; i<100; i++){
        p.update(ps.trainingInput, ps.trainingOutput);
        //if(i % 4 == 0) cout << p.getWeights().T() << "\n";
    }

    cout << "BEGIN TESTING" << endl;
    cout << (ps.challenge|p.apply(ps.challenge));

    return 0;
}
