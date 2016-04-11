#include "matrix.hpp"

Matrix::Matrix(size_t rows, size_t columns)
    : rows(rows), cols(columns) {
    data = new double[rows*cols];
}
Matrix::Matrix(size_t rows, size_t columns, double initialValue)
    : rows(rows), cols(columns) {
    data = new double[rows*cols];
    fill(initialValue);
}
Matrix::Matrix(std::initializer_list<std::initializer_list<double>> values)
        : rows(values.size()), cols(values.begin()->size()){
    //all columns must have the same number of elements
    for(auto row : values){
        assert(row.size() == cols);
    }

    data = new double[rows*cols];
    for(size_t r=0; r<rows; r++){
        for(size_t c=0; c<cols; c++){
            set(r,c, *((values.begin()+r)->begin()+c) );
        }
    }
}
Matrix::~Matrix(){
    delete[] data;
}
Matrix::Matrix(const Matrix& other)
    : rows(other.rows), cols(other.cols) {
    data = new double[rows*cols];
    std::copy(other.data, other.data+rows*cols, data);
}
Matrix& Matrix::operator=(const Matrix& other){
    assert(rows == other.rows);
    assert(cols == other.cols);
    for(size_t r=0; r<rows; r++){
        for(size_t c=0; c<cols; c++){
            set(r,c, other.get(r,c));
        }
    }
    return *this;
}
Matrix Matrix::Ident(size_t n){
    Matrix r(n, n, 0.0);
    for(size_t i=0; i<n; i++) r.set(i, i, 1.0);
    return r;
}
/** Construct a Householder transformation around v
  * v the unit column vector defining a hyperplane
  * returns a Householder matrix which reflects vectors across
  *     the hyperplace defined by v
  */
Matrix Matrix::Householder(const Matrix& v){
    assert(v.cols == 1);
    double n = v.frobeniusNorm();
    Matrix hh = Ident(v.rows) - v*v.T()*(2.0/(n*n));
    return hh;
}
Matrix Matrix::T() const{ //transpose
    Matrix nm(cols, rows);
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < cols; c++){
            nm.set(c, r, get(r,c));
        }
    }
    return nm;
}
std::pair<Matrix, Matrix> Matrix::bisect(size_t splitAt) const {
    assert(splitAt < cols);
    Matrix left(rows, splitAt);
    Matrix right(rows, cols-splitAt);
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < splitAt; c++){
            left.set(r,c,get(r,c));
        }
    }
    for(size_t r = 0; r < rows; r++){
        for(size_t c = splitAt; c < cols; c++){
            right.set(r,c-splitAt,get(r,c));
        }
    }
    return std::pair<Matrix,Matrix>(left,right);
}
void Matrix::fill(double value){
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < cols; c++){
            set(r, c, value);
        }
    }
}
void Matrix::map(std::function<double(double)> transform){
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < cols; c++){
            double value = transform(get(r,c));
            set(r, c, value);
        }
    }
}
Matrix Matrix::elmult(const Matrix& rhs){
    assert (this->rows == rhs.rows);
    assert (this->cols == rhs.cols);
    Matrix res(rows, cols);
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < cols; c++){
            res.set(r, c, this->get(r,c)*rhs.get(r,c));
        }
    }
    return res;
}
double Matrix::frobeniusNorm() const{
    double sum = 0.0;
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < cols; c++){
            double e = get(r,c);
            sum += e*e;
        }
    }
    return sqrt(sum);
}
Matrix Matrix::frobeniusNormalized() const{
    Matrix r(*this);
    double m = 1.0 / frobeniusNorm();
    r.map([=](double v){return v*m;});
    return r;
}
std::string Matrix::toString(int precision) const{
    std::stringstream ss;
    ss.setf(std::ios::fixed,std::ios::floatfield);
    ss.precision(precision);
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < cols; c++){
            ss << get(r,c) << "  ";
        }
        ss << "\n";
    }
    return ss.str();
}
Matrix& Matrix::operator+= (const Matrix& rhs){
    assert (this->rows == rhs.rows);
    assert (this->cols == rhs.cols);
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < cols; c++){
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
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < cols; c++){
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
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < cols; c++){
            double value = this->get(r,c) + rhs.get(r,c);
            nm.set(r, c, value);
        }
    }
    return nm;
}
Matrix Matrix::operator* (const Matrix& rhs) const{
    assert (this->cols == rhs.rows);
    Matrix nm(rows, rhs.cols);
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < rhs.cols; c++){
            double value = 0.0;
            for(size_t i=0; i<cols; i++){
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
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < cols; c++){
            nm.set(r, c, this->get(r,c));
        }
    }
    for(size_t r = 0; r < rows; r++){
        for(size_t c = 0; c < rhs.cols; c++){
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
    os << m.toString(2);
    return os;
}
