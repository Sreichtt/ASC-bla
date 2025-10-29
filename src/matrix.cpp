#include "matrix.hpp"
#include <cmath>
#include "vector.hpp"

int main(){
    ASC_bla::Matrix<double,ASC_bla::RowMajor> A(2,2);
    for (size_t i = 0; i<2; i++){
        for (size_t k = 0; k<2; k++){
            A.setT(i,k,(i+1)*(k+1));
        }
    }

    ASC_bla::Vector<double> v(2);
    for (size_t i = 0; i<v.size(); i++){
        v(i) = i;
    }

    std::cout << "A = \n" << A << std::endl;
    std::cout << "v = " << v << std::endl;
    std::cout << "A*v = " << A*v << std::endl;

    ASC_bla::Matrix<double,ASC_bla::RowMajor> B(2,2);
    for (size_t i = 0; i<2; i++){
        for (size_t k = 0; k<2; k++){
            B.setT(i,k,(i+2)*(k+1));
        }
    }
    std::cout << "B = \n" << B << std::endl;
    std::cout << "A + B = \n" << A+B << std::endl; 

    return 0;
}