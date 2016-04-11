#pragma once
#include <float.h>
#include <functional>
#include <random>
#include "matrix.hpp"

/** returns a (Matrix)=>Matrix that column normalizes the input matrix */
std::function<Matrix(Matrix)> columnNormalizer(const Matrix& input);
/** returns a (Matrix)=>Matrix that centers the the input matrix's columns */
std::function<Matrix(Matrix)> columnCenterer(const Matrix& input);
/** return a random value suited for initialing a NN weight matrix */
double randVal();
/** Add a column of -1's to the right of the matrix m*/
Matrix addBias(const Matrix& m);
/** remive the rightmost column of a matrix m */
Matrix stripBias(const Matrix& m);
