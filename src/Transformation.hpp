#pragma once
#include "matrix.hpp"
#include <cmath>
#include "util.hpp"

/** Returns a tridiagonal matrix with the same eigenvalues as the input
  * input: A symmetric matrix
  * output: A tridiagonal matrix with the same eigenvalues as the input
  *         An orthogonal transformation matrix that maps input to tridiag
  */
typedef struct hhTridiag{
    Matrix tridiag;
    Matrix transformation;
} hhTridiag;

hhTridiag Tridiagonalize(const Matrix& s);

typedef struct EigenSystem{
    int n;
    double* values;
    Matrix vectors;
} EigenSystem;

EigenSystem eigen(const hhTridiag& tri);

/** Returns the n dominant components of m
  */
Matrix components(const Matrix& m, size_t n);
