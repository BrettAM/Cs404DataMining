#pragma once
#include "matrix.hpp"
#include <cmath>

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
