#include "UnitTest++/UnitTest++.h"
#include "matrix.hpp"
#include "Transformation.hpp"

#include <iostream>
using namespace std;

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

TEST(Eigen){
    Matrix a = {{ 1, 3, 4, 0},
                { 2, 3, 1, 0},
                { 2, 3, 0, 9},
                { 0, 3, 1, 0} };
    Matrix aa = a.T()*a;

    hhTridiag ht = Tridiagonalize(aa);

    EigenSystem e = eigen(ht);

    double exp[] = {1.7344, 3.97999, 37.8562, 100.429};

    for(size_t i=0; i<4; i++){
        CHECK_CLOSE(exp[i], e.values[i], 0.01);
    }

    delete[] e.values;
    /*cout << e.n << endl;
    for(int i=0; i<e.n; i++){
        cout << e.values[i] << " ";
    }
    cout << endl;
    cout << e.vectors << endl; */
}

TEST(Component){
    Matrix a = {{ 1, 3, 4, 0},
                { 2, 3, 1, 0},
                { 2, 3, 0, 9},
                { 0, 3, 1, 0} };

    Matrix comps = components(a, 2);

    Matrix exp = {{0.25,  0.17 },
                  {0.45,  0.62 },
                  {0.12,  0.61 },
                  {0.85,  -0.46} };

    for(size_t r=0; r<exp.rows; r++){
        for(size_t c=0; c<exp.cols; c++){
            CHECK_CLOSE(comps.get(r,c), exp.get(r,c), 0.01);
        }
    }
}


