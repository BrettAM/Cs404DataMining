#pragma once
#include <functional>

class Matrix{
public:
    const int rows;
    const int columns;
    Matrix(int rows, int columns);
    Matrix T(); //transpose
    void fill(double value);
    double get(int row, int column);
    void set(int row, int column, double value);
    void map(std::function<double(double)> transform);
    Matrix& operator+= (const Matrix& rhs);
    Matrix  operator- (const Matrix& rhs) const;
    Matrix  operator+ (const Matrix& rhs) const;
    Matrix  operator* (const Matrix& rhs) const;
    Matrix  operator* (double scalar) const;
    Matrix  operator| (const Matrix& rhs) const;
    friend Matrix operator* (double scalar, const Matrix& rhs);
};


