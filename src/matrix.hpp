#ifndef FILE_MATRIX
#define FILE_MATRIX

#include <iostream>
#include <cassert>
#include "vector.hpp"
#include "matrixexpr.hpp"

namespace ASC_bla
{
    enum ORDERING { RowMajor, ColMajor };
    template <typename T, ORDERING ORD = RowMajor, typename TDIST = std::integral_constant<size_t,1>>
    class MatrixView: public MatrixExpr<MatrixView<T,TDIST>> 
    {
        protected:
        size_t m_width;
        size_t m_height;
        T* m_data;
        TDIST m_dist;

        public:
        MatrixView() = default;
        MatrixView(const MatrixView&) = default;

        template <typename TDIST2>
        MatrixView (const MatrixView<T,TDIST2> & m2)
            : m_data(m2.data()), m_width(m2.width()), m_height(m2.height()), m_dist(m2.dist()) { }

        MatrixView (size_t height, size_t width, T* data)
            : m_data(data), m_height(height), m_width(width) { }
        
        MatrixView (size_t height, size_t width, TDIST dist, T* data)
            : m_data(data), m_height(height), m_width(width), m_dist(dist) { }

        template <typename TB>
        MatrixView & operator= (const MatrixExpr<TB> & m2) {
            assert(m_height == m2.height() && m_width == m2.width());
            if(ORD == ColMajor) {
                for(size_t i = 0; i<m_height; i++){
                    for(size_t j = 0; j<m_width; j++){
                        m_data[i + j*m_dist] = m2(i,j);
                    }
                }
            }
            else{
                for(size_t i = 0; i<m_height; i++){
                    for(size_t j = 0; j<m_width; j++){
                        m_data[i*m_dist + j] = m2(i,j);
                    }
                }
            }
            return *this;      
        }

        MatrixView & operator= (T scal)
        {
            if(ORD == ColMajor) {
                for(size_t i = 0; i<m_height; i++){
                    for(size_t j = 0; j<m_width; j++){
                        m_data[i + j*m_dist] = scal;
                    }
                }
            }
            else{
                for(size_t i = 0; i<m_height; i++){
                    for(size_t j = 0; j<m_width; j++){
                        m_data[i*m_dist + j] = scal;
                    }
                }
            }
            return *this;
        }

        T * data() const {return m_data; }
        size_t Height() const {return m_height; }
        size_t Width() const {return m_width; }
        auto dist() const {return m_dist; }

        T & operator() (size_t i, size_t j){
            if(ORD == ColMajor) 
                return m_data[i + j*m_dist];
            else 
                return m_data[i*m_dist + j];
        }

        constexpr auto row(size_t i) {
            if constexpr(ORD == RowMajor){
                return VectorView<T>(m_width, m_data + i * m_width);
            }
            else {
                return VectorView<T>(m_width, )
            }
        }
    };
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
                return i*width + j;
            } else
            {
                return j*height + i;
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

            Matrix & operator=(const Matrix & m2)
            {
                for (size_t i = 0; i < width * height; i++)
                data[i] = m2.data[i];
                return *this;
            }

            Matrix & operator= (Matrix && m2)
            {
                assert(width == m2.width);
                assert(height == m2.height);
                std::swap(width, m2.width);
                std::swap(height, m2.height);
                std::swap(data, m2.data);
                return *this;
            }
            
            size_t Width() const {return width;}
            size_t Height() const {return height;}            
            T& operator()(size_t i, size_t j) {return data[index(i,j)];}
            const T& operator()(size_t i, size_t j) const {return data[index(i,j)];}

            void setT(size_t i, size_t j, T d){
                if (ORD == RowMajor){
                    data[i* width + j] = d;
                }
                else{
                    data[j*height + i] = d; 
                }
            }

    };

    template <typename T, ORDERING ORD>
    Matrix<T, ORD> operator+(const Matrix<T, ORD>& a, const Matrix<T, ORD>& b)
    {
        assert(a.Width() == b.Width() && a.Height() == b.Height());

        Matrix<T, ORD> sum(a.Height(), a.Width());
        for (size_t i = 0; i < a.Height(); ++i)
            for (size_t j = 0; j < a.Width(); ++j)
                sum(i, j) = a(i, j) + b(i, j);

        return sum;
    }   

    template <typename T, ORDERING ORD>
    std::ostream& operator<<(std::ostream& ost, const Matrix<T, ORD>& m)
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

    template <typename T, ORDERING ORD, typename VecT>
    auto operator*(const Matrix<T,ORD>& A, const Vector<VecT>& x) {
        assert(x.Size() == A.Width());
        Vector<decltype(T{} * VecT{})> y(A.Height());

        for (size_t i = 0; i < A.Height(); ++i)
            for (size_t j = 0; j < A.Width(); ++j)
                y(i) += A(i, j) * x(j);
        
        return y;
    }

}
#endif





