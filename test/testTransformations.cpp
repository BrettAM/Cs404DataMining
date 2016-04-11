#include "UnitTest++/UnitTest++.h"
#include "matrix.hpp"
#include "Transformation.hpp"

TEST(Tridiag){
    Matrix a = {{ 4, 1,-2, 2},
                { 1, 2, 0, 1},
                {-2, 0, 3,-2},
                { 2, 1,-2,-1}};
    hhTridiag ht = Tridiagonalize(a);
    Matrix recon = ht.transformation * a * ht.transformation.T();
    for(size_t r=0; r<a.rows; r++){
        for(size_t c=0; c<a.cols; c++){
            CHECK_CLOSE(ht.tridiag.get(r,c), recon.get(r,c), 0.00000001);
        }
    }
}
