#include "matrix.hpp"
#include <cmath>
#include "vector.hpp"

int main(){
    ASC_bla::Matrix<double,ASC_bla::RowMajor> A(2,2);
    for (double i = 0; i<2;i++){
        for (double k = 0;k<2; k++){
            A.setT(i,k,(i+1)*(k+1));
        }
    }

    ASC_bla::Vector<double> v(2);
    for (double i = 0; i<v.Size();i++){
        v(i) = i;
    }

    std::cout << " A = " << A << std::endl;
    std::cout << " v = " << v << std::endl;
    std::cout << "A*x = " << A*v << std::endl;

    return 0;
}