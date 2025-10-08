#ifndef FILE_MATRIX
#define FILE_MATRIX

#include <iostream>

namespace ASC_bla
{

    template <typename T>
    class Matrix
    {
        size_t width;
        size_t height;
        T * data;

        public: 
            Matrix (size_t _height, size_t _width)
                : height(_height), width(_width), data(new T[height*width]) { ; }

            Matrix(const Matrix &m)
                : Matrix(m.Size())
            {
                *this = m;
            }

            Matrix(Matrix && m)
                : width(0), height(0), data(nullptr)
            {
                std::swap(width, m.width);
                std::swap(height, m.height);
                std::swap(data, m.data);
            }

            ~Matrix () {delete [] data; }
    }
}

#endif