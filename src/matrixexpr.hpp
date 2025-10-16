#ifndef MATRIX_EXPRESSION
#define MATRIX_EXPRESSION

#include <cassert>

namespace ASC_bla
{
    enum ORDERING {RowMajor, ColMajor};

    template <typename T, ORDERING ORD = RowMajor>
    class MatrixExpr {
        public:
        auto derived() const {return static_cast<const T&> (*this);}
        size_t height() const {return derived().Height();}
        size_t width() const {return derived().Width();}
        auto operator() (size_t i, size_t j) const {return derived()(i,j);}
    };


    
}