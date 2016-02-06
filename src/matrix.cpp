#include "matrix.hpp"

Matrix::Matrix(int rows, int columns)
    : rows(rows), cols(columns) {
    data = new double[rows*cols];
}
Matrix::~Matrix(){
    delete[] data;
}
Matrix::Matrix(const Matrix& other)
    : rows(other.rows), cols(other.cols) {
    data = new double[rows*cols];
    std::copy(other.data, other.data+rows*cols, data);
}
Matrix Matrix::T() const{ //transpose
    Matrix nm(cols, rows);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            nm.set(c, r, get(r,c));
        }
    }
    return nm;
}
void Matrix::fill(double value){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            set(r, c, value);
        }
    }
}
void Matrix::map(std::function<double(double)> transform){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            double value = transform(get(r,c));
            set(r, c, value);
        }
    }
}
std::string Matrix::toString(int precision) const{
    std::stringstream ss;
    ss.precision(precision);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            ss << get(r,c) << "  ";
        }
        ss << "\n";
    }
    return ss.str();
}
Matrix& Matrix::operator+= (const Matrix& rhs){
    assert (this->rows == rhs.rows);
    assert (this->cols == rhs.cols);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            double value = this->get(r,c) + rhs.get(r,c);
            set(r, c, value);
        }
    }
    return *this;
}
Matrix Matrix::operator- (const Matrix& rhs) const{
    assert (this->rows == rhs.rows);
    assert (this->cols == rhs.cols);
    Matrix nm(rows, cols);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            double value = this->get(r,c) - rhs.get(r,c);
            nm.set(r, c, value);
        }
    }
    return nm;
}
Matrix Matrix::operator+ (const Matrix& rhs) const{
    assert (this->rows == rhs.rows);
    assert (this->cols == rhs.cols);
    Matrix nm(rows, cols);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            double value = this->get(r,c) + rhs.get(r,c);
            nm.set(r, c, value);
        }
    }
    return nm;
}
Matrix Matrix::operator* (const Matrix& rhs) const{
    assert (this->cols == rhs.rows);
    Matrix nm(rows, rhs.cols);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < rhs.cols; c++){
            double value = 0.0;
            for(int i=0; i<cols; i++){
                value += get(r, i) * rhs.get(i, c);
            }
            nm.set(r, c, value);
        }
    }
    return nm;
}
Matrix Matrix::operator| (const Matrix& rhs) const{
    assert (this->rows == rhs.rows);
    Matrix nm(rows, cols+rhs.cols);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            nm.set(r, c, this->get(r,c));
        }
    }
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < rhs.cols; c++){
            nm.set(r, cols+c, rhs.get(r,c));
        }
    }
    return nm;
}
Matrix operator* (double scalar, const Matrix& rhs){
    Matrix nm = rhs;
    nm.map([=](double v) -> double { return scalar*v; });
    return nm;
}
Matrix operator* (const Matrix& rhs, double scalar){
    return scalar*rhs;
}
std::ostream& operator<<(std::ostream& os, const Matrix& m){
    os << m.toString(6);
    return os;
}
