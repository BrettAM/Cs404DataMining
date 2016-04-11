#include "util.hpp"

std::function<Matrix(Matrix)> columnNormalizer(const Matrix& input){
    std::vector<double> shift(input.cols);
    std::vector<double> scale(input.cols);

    for(size_t c=0; c<input.cols; c++){
        double min = DBL_MAX;
        double max = DBL_MIN;
        for(size_t r=0; r<input.rows; r++){
            double v = input.get(r, c);
            min = fmin(min, v);
            max = fmax(max, v);
        }
        shift[c] = -min;
        scale[c] = 1.0/(max-min);
    }

    return [shift, scale](Matrix m) -> Matrix {
        //truncate to the cols we normalized earlier
        Matrix res(m.rows, shift.size());
        for(size_t c=0; c<shift.size(); c++){
            for(size_t r=0; r<m.rows; r++){
                res.set(r,c, ((m.get(r,c)+shift[c])*scale[c]) );
            }
        }
        return res;
    };
}

/** returns a (Matrix)=>Matrix that centers the the input matrix's columns */
std::function<Matrix(Matrix)> columnCenterer(const Matrix& input){
    std::vector<double> shift(input.cols);

    for(size_t c=0; c<input.cols; c++){
        double mean = 0.0;
        for(size_t r=0; r<input.rows; r++){
            mean += input.get(r, c);
        }
        shift[c] = -mean/((double)input.rows);
    }

    return [shift](Matrix m) -> Matrix {
        //truncate to the cols we normalized earlier
        Matrix res(m.rows, shift.size());
        for(size_t c=0; c<shift.size(); c++){
            for(size_t r=0; r<m.rows; r++){
                res.set(r,c, (m.get(r,c)+shift[c]) );
            }
        }
        return res;
    };
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-0.01, 0.01);
double randVal(){
    return dis(gen);
}

Matrix addBias(const Matrix& m){
    auto bias = Matrix(m.rows, 1, -1.0);
    return (m|bias);
}

Matrix stripBias(const Matrix& m){
    return m.bisect(m.cols-1).first;
}
