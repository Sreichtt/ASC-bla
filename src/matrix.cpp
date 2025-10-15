#include "matrix.hpp"
#include <cmath>
#include "vector.hpp"

int main{
    Matrix<double, RowMajor> A(2,2);
    for (int i = 0; i<2;i++){
        for (int k = 0;k<2, k++){
            A(i,k) = i*k;
        }
    }

    Vector<double> v(2);
    for (int i = 0; i<v.Size();i++){
        v(i) = i
    }

    cout << " A = " << A << endl;
    cout << " x = " << x << endl;
    cout << "A*x = " << A*x << endl;
}