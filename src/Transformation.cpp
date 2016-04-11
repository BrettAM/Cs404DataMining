#include "Transformation.hpp"

#include <iostream>
using namespace std;

namespace{
    Matrix getColumn(const Matrix& s, size_t c){
        Matrix res(s.rows, 1);
        for(size_t i=0; i<s.cols; i++){
            res.set(i,0,s.get(i,c));
        }
        return res;
    }
}

hhTridiag Tridiagonalize(const Matrix& s){
    Matrix tri(s);
    Matrix trans = Matrix::Ident(s.cols);
    for(size_t c=0; c<s.cols-1; c++){
        // get the column to zero
        Matrix col = getColumn(tri, c);
        double sub = col.get(c+1,0);
        // calculate the axis of refletion to zero col
        double a = 0;
        for(size_t j=c+1; j<s.cols; j++){
            auto q = col.get(j,0);
            a += q*q;
        }
        a = copysign(sqrt(a), -sub);
        double r = sqrt( 0.5*a*(a-sub) );
        Matrix v(s.cols, 1, 0);
        v.set(c+1,0, (sub-a)/(2*r));
        for(size_t i=c+2; i<s.cols; i++){
            v.set(i,0, col.get(i,0)/(2*r) );
        }
        // apply a housholder reflection matrix across v to tri
        Matrix hh = Matrix::Householder(v);
        tri = hh * tri * hh;
        trans = hh * trans;
    }
    return {tri, trans};
}

