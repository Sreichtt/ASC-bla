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

## Matrix-Vektor-Multiplikation

Um Matrizen mit Vektoren einer passenden Länge multiplizieren zu können, überladen wir den Operator `*` mit den passenden Inputs wiefolgt:

```cpp
template <typename T, ORDERING ORD, typename VecT>

    auto operator*(const Matrix<T,ORD>& A, const Vector<VecT>& x) const {
        assert(x.size() == A.Width());
        Vector<decltype(T{} * VecT{})> y(A.Height(), decltype(T{} * VecT{}){});

        for (size_t i = 0; i < A.Height; ++i)
            for (size_t j = 0; j < A.Width; ++j)
                y[i] += A(i, j) * x[j];
        
        return y;
    }