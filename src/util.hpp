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
    /** apply the decision function to a value */
    virtual double calculate(double val)  const = 0;
    /** get the derivative of the decision function at a value */
    virtual double derivative(double val) const = 0;
    /** get the value and derivative matricies from val */
    virtual std::pair<Matrix, Matrix> apply(matrix val) const {
        Matrix value(val);
        Matrix derivative(val);
        value.map([&](double v){ return calculate(v); });
        derivative.map([&](double v){ return derivative(v); });
        return std::pair<Matrix,Matrix>(value, derivative);
    }
    /** operators for convenience */
    double operator()(double val) const {
        return calculate(val);
    }
    Matrix operator()(Matrix val) const{
        Matrix res(val);
        res.map( [&](double v) -> double { return calculate(v); } );
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
    double calculate(double val) const {
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
    double calculate(double val) const {
        return 1.0 / (1.0 + exp(-val*slope));
    }
    double derivative(double val) const {
        double valueAt = calculate(val);
        return (1.0 - valueAt) * valueAt;
    }
    std::pair<Matrix, Matrix> apply(matrix val) const {
        Matrix value(val);
        value.map([&](double v){ return calculate(v); });
        Matrix derivative(value);
        derivative.map([&](double v){ return (1.0-v)*v; });
        return std::pair<Matrix,Matrix>(value, derivative);
    }
};

inline double step(double v){
    return (v < 0.0)? 0.0 : 1.0;
}

inline double sigmoid(double v){
    return 1.0 / (1.0 + exp(-v));
}

std::function<Matrix(Matrix)> columnNormalizer(Matrix input);
