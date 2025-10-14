#ifndef FILE_MATRIX
#define FILE_MATRIX

#include <iostream>
#include <cassert>
#include "vector.hpp"

namespace ASC_bla
{

    enum ORDERING { RowMajor, ColMajor };
    template <typename T, ORDERING ORD = RowMajor>
    class Matrix
    {
        size_t width;
        size_t height;
        T * data;

        size_t index(size_t i, size_t j) const
        {
            if(ORD == RowMajor)
            {
                return data[i*width + j]
            } else
            {
                return data[j*height + i]
            }
        }

        public: 
            Matrix (size_t _height = 0, size_t _width = 0)
                : height(_height), width(_width), data(new T[height*width]) { ; }

            Matrix(const Matrix& m)
                : Matrix(m.height, m.width)
                {
                    for (size_t i = 0; i < height * width; ++i)
                        data[i] = m.data[i];
                }

            Matrix(Matrix && m)
                : width(0), height(0), data(nullptr)
            {
                std::swap(width, m.width);
                std::swap(height, m.height);
                std::swap(data, m.data);
            }

            ~Matrix () {delete [] data; }
            
            size_t Width() const {return width;}
            size_t Height() const {return height;}
            
            & operator()(size_t i, size_t j) {return data[index(i,j)];}
            const T& operator()(size_t i, size_t j) const {return data[index(i,j)];}

    };

    template <typename T, ORDERING ORD, typename VecT>

    auto operator*(const Matrix<T,ORD>& A, const Vector<VecT>& x) const {
        assert(x.size() == A.Width());
        Vector<decltype(T{} * VecT{})> y(A.Height(), decltype(T{} * VecT{}){});

        for (size_t i = 0; i < A.Height; ++i)
            for (size_t j = 0; j < A.Width; ++j)
                y[i] += A(i, j) * x[j];
        
        return y;
    }

}
#endif
