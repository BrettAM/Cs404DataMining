#pragma once
#include <limits>
#include <cmath>

class DecisionFunc{
public:
    /** apply the decision function to a value */
    virtual double calculate(double val)  const = 0;
    /** get the derivative of the decision function at a value */
    virtual double derivative(double val) const = 0;
    /** get the value and derivative matrices from val */
    virtual std::pair<Matrix, Matrix> apply(Matrix val) const {
        Matrix value(val);
        Matrix deriv(val);
        value.map( [&](double v){ return calculate(v); } );
        deriv.map( [&](double v){ return derivative(v); } );
        return std::pair<Matrix,Matrix>(value, deriv);
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

class StepFunc : public DecisionFunc{
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

class Sigmoid : public DecisionFunc{
    const double slope;
public:
    Sigmoid(double slope): slope(slope) {}
    double calculate(double val) const {
        return 1.0 / (1.0 + exp(-val*slope));
    }
    double derivative(double val) const {
        double valueAt = calculate(val);
        return (1.0 - valueAt) * valueAt;
    }
    std::pair<Matrix, Matrix> apply(Matrix val) const {
        Matrix value(val);
        value.map([&](double v){ return calculate(v); });
        Matrix derivative(value);
        derivative.map([&](double v){ return (1.0-v)*v; });
        return std::pair<Matrix,Matrix>(value, derivative);
    }
};
