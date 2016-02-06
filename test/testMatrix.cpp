#include "UnitTest++/UnitTest++.h"
#include "matrix.hpp"

TEST(ArrayConstructor) {
    Matrix m = {{1.0, 2.0}, {3.0, 4.0}};
    CHECK_EQUAL(m.rows, 2);
    CHECK_EQUAL(m.cols, 2);
    CHECK_EQUAL(m.get(0,0), 1);
    CHECK_EQUAL(m.get(0,1), 2);
    CHECK_EQUAL(m.get(1,0), 3);
    CHECK_EQUAL(m.get(1,1), 4);
}

TEST(SetThenGet) {
    Matrix m(1,1);
    m.set(0, 0, 12);
    CHECK_EQUAL(m.get(0,0), 12);
}

TEST(Transpose) {
    Matrix m(1, 2);
    m.set(0, 0, 13);
    m.set(0, 1, 37);
    Matrix t = m.T();
    CHECK_EQUAL(m.rows, t.cols);
    CHECK_EQUAL(m.cols, t.rows);
    CHECK_EQUAL(m.get(0,0), t.get(0,0));
    CHECK_EQUAL(m.get(0,1), t.get(1,0));
}

TEST(Fill) {
    Matrix m(2, 2);
    m.fill(12.34);
    for(size_t r=0; r<m.rows; r++){
        for(size_t c=0; c<m.cols; c++){
            CHECK_EQUAL(m.get(r,c), 12.34);
        }
    }
}

TEST(Map) {
    Matrix m(2, 2);
    m.fill(3);
    m.map([](double v) -> double { return v*v; });
    for(size_t r=0; r<m.rows; r++){
        for(size_t c=0; c<m.cols; c++){
            CHECK_EQUAL(m.get(r,c), 3*3);
        }
    }
}

TEST(AddAssign) {
    Matrix m(2, 2);
    Matrix d(2, 2);
    m.fill(0);
    d.fill(1);
    m += d;
    for(size_t r=0; r<m.rows; r++){
        for(size_t c=0; c<m.cols; c++){
            CHECK_EQUAL(m.get(r,c), 1);
        }
    }
}

TEST(Add) {
    Matrix m(2, 2);
    Matrix d(2, 2);
    m.fill(0);
    d.fill(1);
    Matrix s = m+d;
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 1);
        }
    }
}

TEST(Subtract) {
    Matrix m(2, 2);
    Matrix d(2, 2);
    m.fill(2);
    d.fill(1);
    Matrix s = m-d;
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 1);
        }
    }
}

TEST(ScalarMultiplyLeft) {
    Matrix m(2, 2);
    m.fill(2);
    Matrix s = 2*m;
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 4);
        }
    }
}

TEST(ScalarMultiplyRight) {
    Matrix m(2, 2);
    m.fill(2);
    Matrix s = m*2;
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 4);
        }
    }
}

TEST(InnerProduct) {
    Matrix a(1, 2);
    Matrix b(2, 1);
    a.fill(2);
    b.fill(3);
    Matrix s = a*b;
    CHECK_EQUAL(s.rows, 1);
    CHECK_EQUAL(s.cols, 1);
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 12);
        }
    }
}

TEST(OuterProduct) {
    Matrix a(2, 1);
    Matrix b(1, 2);
    a.fill(2);
    b.fill(3);
    Matrix s = a*b;
    CHECK_EQUAL(s.rows, 2);
    CHECK_EQUAL(s.cols, 2);
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 6);
        }
    }
}

TEST(MatrixMultiply) {
    Matrix a = {{1, 2}, {3, 4}};
    Matrix b = {{5, 5}, {6, 6}};

    Matrix s = a*b;

    CHECK_EQUAL(s.rows, 2);
    CHECK_EQUAL(s.cols, 2);
    CHECK_EQUAL(s.get(0,0), 17); CHECK_EQUAL(s.get(0,1), 17);
    CHECK_EQUAL(s.get(1,0), 39); CHECK_EQUAL(s.get(1,1), 39);
}

TEST(Concatenate) {
    Matrix a(2, 2);
    Matrix b(2, 1);
    a.fill(2);
    b.fill(2);
    Matrix s = a|b;
    CHECK_EQUAL(s.rows, 2);
    CHECK_EQUAL(s.cols, 3);
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 2);
        }
    }
}
