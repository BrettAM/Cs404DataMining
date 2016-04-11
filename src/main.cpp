#include <iostream>
#include <fstream>
#include "matrix.hpp"
#include "DecisionFunc.hpp"
#include "PtronLayer.hpp"
#include "util.hpp"
#include "Input.hpp"
#include "Transformation.hpp"
using namespace std;

Sigmoid sig(40.0);

int main(int argc, char const *argv[]) {
    Matrix ps = readMatrix(cin);

    auto norm = columnCenterer(ps);
    Matrix centered = norm(ps);

    Matrix comps = components(centered, 10);

    Matrix compressed = centered * comps;

/*
    cout << "Input: \n" << ps << endl;
    cout << "Centered: \n" << centered << endl;
    cout << "Correlation: \n" << (centered.T()*centered)*(1.0/centered.rows) << endl;
    cout << "Components: \n" << comps << endl;
    cout << "Compressed: \n" << compressed << endl;
    cout << "Reconstruction: \n" << compressed * comps.T() << endl;
*/
    cout << compressed.rows << " " << compressed.cols << endl;
    cout << compressed.toString(6);

    return 0;
}
