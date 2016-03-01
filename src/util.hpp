#pragma once
#include <cmath>
#include <cmath>
#include <float.h>
#include <functional>
#include <limits>
#include <vector>
#include "matrix.hpp"

//typedef std::function<double(double)> Decider;

class Decider{
public:
    double derivative(double val) const;
    double operator()(double val) const;
    Matrix operator()(Matrix val) const{
        Matrix res(val);
        res.map( [&](double v) -> double { return (*this)(v); } );
        return res;
    }
    Matrix derivative(Matrix val) const{
        Matrix res(val);
        res.map( [&](double v) -> double { return derivative(v); }  );
        return res;
    }
};

class StepFunc : public Decider{
    const double cutoff;
public:
    StepFunc(double cutoff): cutoff(cutoff) {}
    double operator()(double val) const {
        return (val < cutoff)? 0.0 : 1.0;
    }
    double derivative(double val) const {
        return (val == cutoff)? std::numeric_limits<double>::infinity() : 0.0;
    }
};

class SigmoidFunc : public Decider{
    const double slope;
public:
    SigmoidFunc(double slope): slope(slope) {}
    double operator()(double val) const {
        return 1.0 / (1.0 + exp(-val*slope));
    }
    double derivative(double val) const {
        return (1.0 - (*this)(val)) * (*this)(val);
    }
};

inline double step(double v){
    return (v < 0.0)? 0.0 : 1.0;
}

inline double sigmoid(double v){
    return 1.0 / (1.0 + exp(-v));
}

std::function<Matrix(Matrix)> columnNormalizer(Matrix input);
