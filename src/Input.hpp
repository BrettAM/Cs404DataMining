#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "matrix.hpp"

struct ProblemSet{
    int inputCount;
    int outputCount;
    Matrix trainingInput;
    Matrix trainingOutput;
    Matrix challenge;
};

ProblemSet readProblem(std::string file);
