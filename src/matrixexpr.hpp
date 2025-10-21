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

    template <typename A, typename B>
    class MatrixAddExpr : public MatrixExpr<MatrixAddExpr<A,B>>
    {
        A a;
        B b;
        public:
            MatrixAddExpr(A _a, B _b) : a(_a), b(_b) {}

            auto operator() (size_t i, size_t j) const{
                return a(i,j) + b(i,j);
            }
            size_t height() const {return a.height(); }
            size_t width() const {return a.width(); }
    };

    template <typename A, typename B>
    class MatrixMulExpr : public<MatrixMulExpr<A,B>>
    {
        A a;
        B b;

        public:
            MatrixMulExpr(A _a, B _b) : a(_a), b(_b) {
                assert(a.width() == b.height());
            }
            size_t height() const {return a.height(); }
            size_t width() const {return b.width(); }

            //Zugriff auf den (i,j). Eintrag in das Produkt von a und b
            auto operator()(size_t i, size_t j) const {
                using elemA = decltype(a(i,0));
                using elemB = decltype(b(0,j));
                using TSUM = decltype(std::declval<elemA>() * std::declval<elemB>());

                TSUM sum = 0;
                for(size_t k = 0; k<a.width(); k++){
                    sum += a(i,k) * b(k,j);
                }

                return sum;
            }
    };
    
    template <typename Mat, typename Vec>
    class MatVecExpr : public VecExpr<MatVecExpr<Mat, Vec>> 
    {
        Mat a;
        Vec x;

        public:
            MatVecExpr(Mat _a, Vec _x) : a(_a), x(_x) {
                assert(a.width() == x.size());
            }

            size_t size() const {return a.height();}

            auto operator()(size_t i) const {
                using elemA = decltype(a(i,0));
                using elemB = decltype(x(0));
                using TSUM = decltype(std::declval<elemA>() * std::declval<elemB>());

                TSUM sum = 0;
                for(size_t k = 0; k < a.width(), k++){
                    sum += a(i,k) * x(k);
                }
                return sum;
            }
    }
}