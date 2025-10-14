# Matrix-Klasse

Die `Matrix`-Klasse von ASC-bla beinhaltet einige, wichtige Matrixoperationen der linearen Algebra wie etwa Addition, Multiplikation oder Transposition.

## Beispiel

```cpp 
#include "matrix.hpp"
using namespace ASC_bla;

int main() {
    Matrix<double> A(3, 3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            A(i, j) = i + j;

    Matrix<double> B = 2.0 * A;
    Matrix<double> C = A * B;
    std::cout << "Matrix C = " << C << std::endl;
}
