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

TEST(FillConstructor) {
    Matrix m(2, 2, 12.34);
    for(size_t r=0; r<m.rows; r++){
        for(size_t c=0; c<m.cols; c++){
            CHECK_EQUAL(m.get(r,c), 12.34);
        }
    }
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

TEST(Bisect) {
    Matrix m = {{1.0, 2.0, 3.0},
                {4.0, 5.0, 6.0} };
    auto lr = m.bisect(2);
    Matrix left = lr.first;
    Matrix right = lr.second;
    CHECK_EQUAL(m.rows, left.rows);
    CHECK_EQUAL(m.rows, right.rows);
    CHECK_EQUAL(left.cols, 2);
    CHECK_EQUAL(right.cols, 1);

    CHECK_EQUAL( left.get(0,0), 1.0);
    CHECK_EQUAL( left.get(0,1), 2.0);
    CHECK_EQUAL( left.get(1,0), 4.0);
    CHECK_EQUAL( left.get(1,1), 5.0);
    CHECK_EQUAL(right.get(0,0), 3.0);
    CHECK_EQUAL(right.get(1,0), 6.0);
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
    Matrix m(2, 2, 3.0);
    m.map([](double v) -> double { return v*v; });
    for(size_t r=0; r<m.rows; r++){
        for(size_t c=0; c<m.cols; c++){
            CHECK_EQUAL(m.get(r,c), 3*3);
        }
    }
}

TEST(ElementWiseMultiply) {
    Matrix a = {{1, 2}, {3, 4}};
    Matrix b = {{-1,1}, {0, 4}};
    Matrix res = a.elmult(b);
    Matrix exp = {{-1, 2}, {0, 16}};
    for(size_t r=0; r<res.rows; r++){
        for(size_t c=0; c<res.cols; c++){
            CHECK_EQUAL(res.get(r,c), exp.get(r,c));
        }
    }
}

TEST(AddAssign) {
    Matrix m(2, 2, 0.0);
    Matrix d(2, 2, 1.0);
    m += d;
    for(size_t r=0; r<m.rows; r++){
        for(size_t c=0; c<m.cols; c++){
            CHECK_EQUAL(m.get(r,c), 1);
        }
    }
}

TEST(Add) {
    Matrix m(2, 2, 0.0);
    Matrix d(2, 2, 1.0);
    Matrix s = m+d;
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 1);
        }
    }
}

TEST(Subtract) {
    Matrix m(2, 2, 2.0);
    Matrix d(2, 2, 1.0);
    Matrix s = m-d;
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 1);
        }
    }
}

TEST(ScalarMultiplyLeft) {
    Matrix m(2, 2, 2.0);
    Matrix s = 2*m;
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 4);
        }
    }
}

TEST(ScalarMultiplyRight) {
    Matrix m(2, 2, 2.0);
    Matrix s = m*2;
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 4);
        }
    }
}

TEST(InnerProduct) {
    Matrix a(1, 2, 2.0);
    Matrix b(2, 1, 3.0);
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
    Matrix a(2, 1, 2.0);
    Matrix b(1, 2, 3.0);
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
    Matrix a(2, 2, 2.0);
    Matrix b(2, 1, 2.0);
    Matrix s = a|b;
    CHECK_EQUAL(s.rows, 2);
    CHECK_EQUAL(s.cols, 3);
    for(size_t r=0; r<s.rows; r++){
        for(size_t c=0; c<s.cols; c++){
            CHECK_EQUAL(s.get(r,c), 2);
        }
    }
}

TEST(IdentFactory) {
    Matrix a = {{1, 2}, {3, 4}};
    Matrix p = a*Matrix::Ident(2);
    for(size_t r=0; r<a.rows; r++){
        for(size_t c=0; c<a.cols; c++){
            CHECK_EQUAL(p.get(r,c), a.get(r,c));
        }
    }
}

TEST(HouseholderFactory) {
    Matrix a = {{1.0},{1.0},{0.0}};
    Matrix p = Matrix::Householder(a)*a;
    for(size_t r=0; r<a.rows; r++){
        for(size_t c=0; c<a.cols; c++){
            CHECK_CLOSE(p.get(r,c),-a.get(r,c), 0.00000001);
        }
    }
}

TEST(FrobeniusNorm) {
    Matrix a = {{0.0, 3.0}, {4.0, 0.0}};
    CHECK_EQUAL(a.frobeniusNorm(), 5.0);
}

TEST(FrobeniusNormalized) {
    Matrix a = {{0.0, 3.0}, {4.0, 0.0}};
    Matrix r = a.frobeniusNormalized();
    CHECK_EQUAL(r.frobeniusNorm(), 1.0);
}

TEST(Assignment) {
    Matrix a = {{1.0}};
    Matrix b = {{0.0}};
    b = a;
    CHECK_EQUAL(b.get(0,0), 1.0);
}
