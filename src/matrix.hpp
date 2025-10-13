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
                : Matrix(m.height, m.width)
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

            Matrix & operator=(const Matrix & m2)
            {
                for (size_t i = 0; i < width * height; i++)
                data[i] = m2.data[i];
                return *this;
            }

            Matrix & operator= (Matrix && m2)
            {
                std::swap(width, m2.width);
                std::swap(height, m2.height);
                std::swap(data, m2.data);
                return *this;
            }
    
            size_t Width() const { return width; }
            size_t Height() const { return height; }
            T & operator()(size_t i) {return data[i]; }
            const T & operator()(size_t i) const { return data[i]; }
            T & operator()(size_t i, size_t j) { return data[i + j * width]; }
            const T & operator()(size_t i, size_t j) const { return data[i + j * width]; }
    };

    template <typename T>
    Matrix<T> operator+ (const Matrix<T> & a, const Matrix<T> & b)
    {
        Matrix<T> sum(a.Width(), a.Height());
        for (size_t i = 0; i < a.height*a.width(); i++)
            sum(i) = a(i)+b(i);
        return sum;
    }
  

    template <typename T>
    std::ostream & operator<<(std::ostream & ost, const Matrix<T> & m)
    {
        for (size_t i = 0; i < m.Height(); ++i)
        {
            for (size_t j = 0; j < m.Width(); ++j)
            {
                ost << m(i, j);
                if (j + 1 < m.Width()) ost << ", ";
            }
            ost << '\n';
        }
        return ost;
    }
}
#endif
