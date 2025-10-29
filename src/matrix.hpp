#ifndef FILE_MATRIX
#define FILE_MATRIX

#include <iostream>
#include <cassert>
#include "vector.hpp"
#include "matrixexpr.hpp"

namespace ASC_bla
{
    //enum ORDERING { RowMajor, ColMajor };
    template <typename T, ORDERING ORD = RowMajor, typename TDIST = std::integral_constant<size_t,1>>
    class MatrixView: public MatrixExpr<MatrixView<T, ORD, TDIST>> 
    {
        protected:
        size_t m_width;
        size_t m_height;
        T* m_data;
        TDIST m_dist;

        public:
        MatrixView() = default;
        MatrixView(const MatrixView&) = default;

        template <ORDERING ORD2, typename TDIST2>
        MatrixView (const MatrixView<T, ORD2, TDIST2> & m2)
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
            if constexpr(ORD == ColMajor) 
                return m_data[i + j*m_dist];
            else 
                return m_data[i*m_dist + j];
        }

        const T & operator() (size_t i, size_t j) const {
            if constexpr(ORD == ColMajor)
                return m_data[i + j*TDIST::value];
            else
                return m_data[i*TDIST::value + j];
        }

        constexpr auto row(size_t i) {
            if constexpr(ORD == RowMajor){
                return VectorView<T>(m_width, m_data + i * m_width); //default-stride 1, Start bei i-ter Zeile
            }
            else {
                return VectorView<T>(m_width, m_dist, m_data + i); //m_dist = m_height für ColMajor, Start an Stelle i
            }
        }

        constexpr auto col(size_t j) {
            if constexpr(ORD == ColMajor){
                return VectorView<T>(m_height, m_data + j * m_height);
            }
            else {
                return VectorView<T>(m_height, m_dist, m_data + j); // m_dist = m_width
            }
        }

        constexpr auto rows(size_t first, size_t next) {
            if constexpr(ORD == RowMajor){
                return MatrixView<T, ORD, TDIST>(next - first, m_width, m_dist, m_data + first * m_width);
            } 
            else {
                return MatrixView<T, ORD, TDIST>(next - first, m_width, m_dist, m_data + first);
            }
        }

        constexpr auto cols(size_t first, size_t next) {
            if constexpr (ORD == ColMajor) {
                return MatrixView<T, ORD, TDIST>(m_height, next - first, m_dist, m_data + first * m_height); //m_dist von Obermatrix übernommen
            }
            else {
                return MatrixView<T, ORD, TDIST>(m_height, next - first, m_dist, m_data + first);
                //return VectorView<T>(m_width, m_data + i * m_height);
            }
        }

        auto transpose() const {
            if constexpr (ORD == RowMajor){
                return MatrixView<T, ColMajor, TDIST>(m_width, m_height, m_dist, m_data); //m_width und m_height im Konstruktor vertauscht
            }
            else{
                return MatrixView<T, RowMajor, TDIST>(m_width, m_height, m_dist, m_data);
            }
        }

    };


    template <typename A, typename B>
    auto operator+(const MatrixExpr<A>& a, const MatrixExpr<B>& b) {
        return MatrixAddExpr(a.derived(), b.derived());
    }

    template <typename A, typename B>
    auto operator*(const MatrixExpr<A>& a, const MatrixExpr<B>& b) {
        return MatrixMulExpr(a.derived(), b.derived());
    }

    template<typename Mat, typename Vec>
    auto operator*(const MatrixExpr<Mat>& a, VecExpr<Vec>& x) {
        return MatVecExpr(a.derived(), x.derived());
    }

    template <typename T, ORDERING ORD = RowMajor>
    class Matrix : public MatrixView<T,ORD>
    {
        typedef MatrixView<T, ORD> BASE;
        using BASE::m_width;
        using BASE::m_height;
        using BASE::m_data;

        size_t index(size_t i, size_t j) const
        {
            if(ORD == RowMajor)
            {
                return i*m_width + j;
            } else
            {
                return j*m_height + i;
            }
        }

        public: 
            //Matrix (size_t _height = 0, size_t _width = 0)
                //: m_height(_height), m_width(_width), m_data(new T[_height*_width]) { ; }

            Matrix(size_t _height = 0, size_t _width = 0)
                : BASE(_height, _width, new T[_height*_width]) { }

            Matrix(const Matrix& m)
                : Matrix(m.m_height, m.m_width)
                {
                    for (size_t i = 0; i < m_height * m_width; ++i)
                        m_data[i] = m.m_data[i];
                }

            //Matrix(Matrix && m)
                //: m_width(0), m_height(0), m_data(nullptr)
            Matrix(Matrix && m)
                : BASE(0, 0, nullptr) 
            {
                std::swap(m_width, m.m_width);
                std::swap(m_height, m.m_height);
                std::swap(m_data, m.m_data);
            }

            ~Matrix () {delete [] m_data; }

            Matrix & operator=(const Matrix & m2)
            {
                for (size_t i = 0; i < m_width * m_height; i++)
                m_data[i] = m2.m_data[i];
                return *this;
            }

            Matrix & operator= (Matrix && m2)
            {
                assert(m_width == m2.m_width);
                assert(m_height == m2.m_height);
                std::swap(m_width, m2.m_width);
                std::swap(m_height, m2.m_height);
                std::swap(m_data, m2.m_data);
                return *this;
            }
            
            size_t Width() const {return m_width;}
            size_t Height() const {return m_height;}            
            T& operator()(size_t i, size_t j) {return m_data[index(i,j)];}
            const T& operator()(size_t i, size_t j) const {return m_data[index(i,j)];}

            void setT(size_t i, size_t j, T d){
                if (ORD == RowMajor){
                    m_data[i * m_width + j] = d;
                }
                else{
                    m_data[j * m_height + i] = d; 
                }
            }

            Matrix<T, ORD> inverse() const;

    };


    template <typename T, ORDERING ORD>
    Matrix<T, ORD> Matrix<T, ORD>::inverse() const {
        assert(m_height == m_width);

        size_t n = m_height;

        Matrix<T, ORD> A(*this);                        
        Matrix<T, ORD> I(n, n);                         
        for (size_t i = 0; i < n; ++i){
            I(i,i) = 1;
        }

        for (size_t j = 0; j < n; ++j) {
            size_t pivot_row = j;
            auto col_j = A.col(j);
            while (pivot_row < n && col_j(pivot_row) == 0)      
                ++pivot_row;

            assert(pivot_row != n);

            if (pivot_row != j) {                                
                for (size_t k = 0; k < n; ++k) {
                    std::swap(A(j,k), A(pivot_row,k));
                    std::swap(I(j,k), I(pivot_row,k));
                }
            }

            T pivot_val = A(j,j);
            for (size_t k = 0; k < n; ++k) {
                A(j,k) /= pivot_val;
                I(j,k) /= pivot_val;
            }

            for (size_t i = 0; i < n; ++i) {                     
                if (i == j) continue;
                T factor = A(i,j);
                for (size_t k = 0; k < n; ++k) {
                    A(i,k) -= factor * A(j,k);                  
                    I(i,k) -= factor * I(j,k);
                }
            }
        }

        return I; 
    }



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
        assert(x.size() == A.Width());
        Vector<decltype(T{} * VecT{})> y(A.Height());

        for (size_t i = 0; i < A.Height(); ++i)
            for (size_t j = 0; j < A.Width(); ++j)
                y(i) += A(i, j) * x(j);
        
        return y;
    }

}
#endif





