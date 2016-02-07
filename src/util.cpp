#include "util.hpp"

std::function<Matrix(Matrix)> columnNormalizer(Matrix input){
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
