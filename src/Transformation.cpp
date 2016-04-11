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

    double pythag(double a, double b) {
        double absa, absb;
        absa=fabs(a);
        absb=fabs(b);
        if (absa > absb) return absa*sqrt(1.0+(absb/absa)*(absb/absa));
        else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+(absa/absb)*(absa/absb)));
    }
}

Matrix components(const Matrix& m, size_t n){
    auto cor = m.T()*m*(1.0/m.rows);
    auto tri = Tridiagonalize(cor);
    auto eig = eigen(tri);

    auto vectors = eig.vectors;

    if(n > vectors.cols) n = vectors.cols;
    Matrix res(vectors.rows, n);
    for(size_t i=0; i<n; i++){
        //select the largest eigen value column
        double maxVal = eig.values[0];
        int maxValIdx = 0;
        for(int j=0; j<eig.n; j++){
            if(eig.values[j] > maxVal){
                maxVal = eig.values[j];
                maxValIdx = j;
            }
        }
        //zero the value so it isn't selected again
        eig.values[maxValIdx] = 0.0;
        //copy it into result
        for(size_t r=0; r<res.rows; r++){
            res.set(r, i, vectors.get(r, maxValIdx));
        }
    }

    delete[] eig.values;

    return res;
}

/*hhTridiag Tridiagonalize(const Matrix& s){
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
}*/

hhTridiag Tridiagonalize(const Matrix& s){
    const int n = s.rows;
    double a[n][n];
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            a[i][j] = s.get(i,j);
        }
    }
    double d[n];
    double e[n];
    for(int i=0; i<n; i++){
        d[i] = 0.0;
        e[i] = 0.0;
    }
    //double **a, int n, double d[], double e[]){
    int l, k, j, i;
    double scale, hh, h, g, f;

    for (i=n-1; i>=1; i--) {
        l=i-1;
        h=scale=0.0;
        if (l > 0) {
            for (k=0; k<=l; k++) {
                scale += fabs(a[i][k]);
            }

            if (scale == 0.0) {             // Skip transformation.
                e[i]=a[i][l];
            }
            else {
                for (k=0; k<=l; k++) {
                    a[i][k] /= scale;       // Use scaled a's for transformation.
                    h += a[i][k]*a[i][k];   // Form sigma in h.
                }
                f=a[i][l];
                g=(f >= 0.0 ? -sqrt(h) : sqrt(h));
                e[i]=scale*g;
                h -= f*g;                   // Now h is equation (11.2.4).
                a[i][l]=f-g;                // Store u in the ith row of a.
                f=0.0;
                for (j=0; j<=l; j++) {
                    // Next statement can be omitted if eigenvectors not wanted
                    a[j][i]=a[i][j]/h;      // Store u/H in ith column of a.

                    g=0.0;                  // Form an element of A.u in g.
                    for (k=0; k<=j; k++) {
                        g += a[j][k]*a[i][k];
                    }
                    for (k=j+1; k<=l; k++) {
                        g += a[k][j]*a[i][k];
                    }

                    e[j]=g/h;               // Form element of p in temporarily unused element of e.
                    f += e[j]*a[i][j];
                }
                hh=f/(h+h);                 // Form K, equation (11.2.11).

                // Form q and store in e overwriting p.
                for (j=0; j<=l; j++) {
                    f=a[i][j];
                    e[j]=g=e[j]-hh*f;

                    // Reduce a, equation (11.2.13).
                    for (k=0; k<=j; k++) {
                        a[j][k] -= (f*e[k] + g*a[i][k]);
                    }
                }
            }
        }
        else {
            e[i]=a[i][l];
        }
        d[i]=h;
    }

    // Next statement can be omitted if eigenvectors not wanted
    d[0]=0.0;
    e[0]=0.0;

    // Contents of this loop can be omitted if eigenvectors not
    //   wanted except for statement d[i]=a[i][i];

    // Begin accumulation of transformation matrices.
    for (i=0; i<n; i++) {
        l=i-1;
        if (d[i]) {                        // This block skipped when i=0.
            for (j=0; j<=l; j++) {
                // Use u and u/H stored in a to form P.Q.
                g=0.0;
                for (k=0; k<=l; k++) {
                    g += a[i][k]*a[k][j];
                }
                for (k=0; k<=l; k++) {
                    a[k][j] -= g*a[k][i];
                }
            }
        }
        d[i]=a[i][i];                       // This statement remains.
        a[i][i]=1.0;                        // Reset row and column of a to identity matrix for next iteration.
        for (j=0; j<=l; j++) {
            a[j][i]=a[i][j]=0.0;
        }
    }

    Matrix transform(s);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            transform.set(i,j,a[j][i]);
        }
    }
    Matrix tri(n,n,0);
    for(int i=0; i<n; i++){
        tri.set(i,i,d[i]);
    }
    for(int i=0; i<n-1; i++){
        tri.set(i, i+1, e[i+1]);
        tri.set(i+1, i, e[i+1]);
    }
    return {tri, transform};
}
















EigenSystem eigen(const hhTridiag& tri) {
    const int n = tri.tridiag.cols;
    Matrix vectors = tri.transformation.T();
    double d[n]; // diagonal
    for(int i=0; i<n; i++){
        d[i] = tri.tridiag.get(i,i);
    }
    double e[n]; // offdiagonal
    for(int i=0; i<n-1; i++){
        e[i] = tri.tridiag.get(i,i+1);
    }
    e[n-1] = 0.0;

    int m, l, iter, i, k;
    double s, r, p, g, f, dd, c, b;

    // foreign algorithm

    for (l=0; l<n; l++) {
        iter=0;
        do {
            // Look for a single small subdiagonal element to split the matrix.
            for (m=l; m<n-1; m++) {
                dd=fabs(d[m])+fabs(d[m+1]);
                if ((double)(fabs(e[m])+dd) == dd) break;
            }

            if (m != l) {
                if (iter++ == 30) cout << "Too many iterations in tqli";
                g=(d[l+1]-d[l])/(2.0*e[l]); // Form shift.
                r=pythag(g, 1.0);
                g=d[m]-d[l]+e[l]/(g+copysign(r, g)); // This is dm - ks.
                s=c=1.0;
                p=0.0;
                for (i=m-1; i>=l; i--) { // A plane rotation as in the original QL, followed by Givens
                    f=s*e[i];            // rotations to restore tridiagonal form.
                    b=c*e[i];
                    e[i+1]=(r=pythag(f, g));
                    if (r == 0.0) {      // Recover from underflow.
                        d[i+1] -= p;
                        e[m]=0.0;
                        break;
                    }
                    s=f/r;
                    c=g/r;
                    g=d[i+1]-p;
                    r=(d[i]-g)*s+2.0*c*b;
                    d[i+1]=g+(p=s*r);
                    g=c*r-b;
                    // Next loop can be omitted if eigenvectors not wanted
                    // Form eigenvectors.
                    for (k=0; k<n; k++) {
                        f=vectors.get(k,i+1);
                        vectors.set(k,i+1, s*vectors.get(k,i)+c*f );
                        vectors.set(k,i  , c*vectors.get(k,i)-s*f );
                    }
                }
                if (r == 0.0 && i >= l) continue;
                d[l] -= p;
                e[l]=g;
                e[m]=0.0;
            }
        } while (m != l);
    }

    // end foreign algorithm

    double* values = new double[n];
    for(int i=0; i<n; i++) values[i] = d[i];
    return {n, values, vectors};
}
