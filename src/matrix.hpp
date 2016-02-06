#pragma once
#include <functional>
#include <assert.h>
#include <string>
#include <sstream>

class Matrix{
private:
    double* data; //rows by columns
public:
    const int rows;
    const int cols;
    Matrix(int rows, int columns);
    ~Matrix();
    Matrix(const Matrix& other);
    /** return the value at `row` and `column` indexed at 0
      * defined here for inlining
      */
    double get(int row, int column) const {
        assert(row < rows);
        assert(column < cols);
        return data[row*cols+column];
    }
    /** set the value at `row` and `column` indexed at 0
      * defined here for inlining
      */
    void set(int row, int column, double value){
        assert(row < rows);
        assert(column < cols);
        data[row*cols+column] = value;
    }
    /** return a Matrix that is the transpose of this one */
    Matrix T();
    /** fill every element with `value` */
    void fill(double value);
    /** set every element to the result of transform(original_element) */
    void map(std::function<double(double)> transform);
    /** Make a string for displaying the state of this matrix */
    std::string toString(int precision);
    /** add correspending elements in `rhs` to this matrix */
    Matrix& operator+= (const Matrix& rhs);
    /** construct a matrix that is the elementwise difference of this and rhs */
    Matrix  operator- (const Matrix& rhs) const;
    /** construct a matrix that is the elementwise sum of this and rhs */
    Matrix  operator+ (const Matrix& rhs) const;
    /** construct a matrix that is the prodect this and rhs */
    Matrix  operator* (const Matrix& rhs) const;
    /** construct a matrix of this and `rhs` concatenated left to right */
    Matrix  operator| (const Matrix& rhs) const;
    /** construct a matrix that is the elementwise product of this and scalar */
    friend Matrix operator* (double scalar, const Matrix& rhs);
    friend Matrix operator* (const Matrix& rhs, double scalar);
};


