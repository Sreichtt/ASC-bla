#include <sstream>
#include <pybind11/pybind11.h>

#include "vector.hpp"

using namespace ASC_bla;
namespace py = pybind11;


PYBIND11_MODULE(bla, m) {
    m.doc() = "Basic linear algebra module"; // optional module docstring
    
    py::class_<Vector<double>> (m, "Vector")
      .def(py::init<size_t>(),
           py::arg("size"), "create vector of given size")
      .def("__len__", &Vector<double>::Size,
           "return size of vector")
      
      .def("__setitem__", [](Vector<double> & self, int i, double v) {
        if (i < 0) i += self.Size();
        if (i < 0 || i >= self.Size()) throw py::index_error("vector index out of range");
        self(i) = v;
      })
      .def("__getitem__", [](Vector<double> & self, int i) { return self(i); })
      
      .def("__setitem__", [](Vector<double> & self, py::slice inds, double val)
      {
        size_t start, stop, step, n;
        if (!inds.compute(self.Size(), &start, &stop, &step, &n))
          throw py::error_already_set();
        self.Range(start, stop).Slice(0,step) = val;
      })
      
      .def("__add__", [](Vector<double> & self, Vector<double> & other)
      { return Vector<double> (self+other); })

      .def("__rmul__", [](Vector<double> & self, double scal)
      { return Vector<double> (scal*self); })
      
      .def("__str__", [](const Vector<double> & self)
      {
        std::stringstream str;
        str << self;
        return str.str();
      })

     .def(py::pickle(
        [](Vector<double> & self) { // __getstate__
            /* return a tuple that fully encodes the state of the object */
          return py::make_tuple(self.Size(),
                                py::bytes((char*)(void*)&self(0), self.Size()*sizeof(double)));
        },
        [](py::tuple t) { // __setstate__
          if (t.size() != 2)
            throw std::runtime_error("should be a 2-tuple!");

          Vector<double> v(t[0].cast<size_t>());
          py::bytes mem = t[1].cast<py::bytes>();
          std::memcpy(&v(0), PYBIND11_BYTES_AS_STRING(mem.ptr()), v.Size()*sizeof(double));
          return v;
        }))
    ;

    py::class_<Matrix<double, RowMajor>>(m, "Matrix")
        .def(py::init<size_t, size_t>(),
            py::arg("height, width"), "create a matrix of given height and width")
        .def("__height__", &Matrix<double, RowMajor>::Height,
            "return height of matrix")
        .def("__width__", &Matrix<double, RowMajor>::Width,
            "return width of matrix")
        .def("__setitem__", [](Matrix<double, RowMajor> & self, std::tuple<int, int> ind, double v){
          i = std::get<0>(ind);
          j = std::get<1>(ind);
          if(i<0) i += self.Height();
          if(j<0) j += self.Width();
          if(i<0 || i >= self.Height()) throw py::error("Matrix row index out of range");
          if(j<0 || j >= self.Width()) throw py::error("Matrix col index out of range");
          self(i,j) = v;
        })
        .def("__getitem__",
           [](Matrix<double, RowMajor> self, std::tuple<int, int> ind) {
               return self(std::get<0>(ind), std::get<1>(ind));
           })
        .def_property_readonly("shape",
           [](const Matrix<double, RowMajor>& self) {
               return std::tuple(self.Height(), self.Width());
           })

        .def("__add__", 
            [](Matrix<double, RowMajor> & self, Matrix<double, RowMajor> & other){
              return Matrix<double, RowMajor>(self + other);
            })
        .def("__MatMatmul__",
            [](Matrix<double, RowMajor> & self, Matrix<double, RowMajor> & other){
              return Matrix<double, RowMajor>(self * other);
            })
        .def("matmul_with_time", 
          [](Matrix<double, RowMajor>& self, Matrix<double, RowMajor>& other) {
            auto t0 = std::chrono::high_resolution_clock::now();
            Matrix<double, RowMajor> C = self * other;
            auto t1 = std::chrono::high_resolution_clock::now();
            double cpp_time = std::chrono::duration<double>(t1 - t0).count();
            return py::make_tuple(C, cpp_time);
          },
        )

        .def("__MatVecmul__",
            [](Matrix<double, RowMajor> & mat, Vector<double> & vec){
              return Vector<double>(mat * vec);
            })

        .def("inverse",
            [](Matrix<double, RowMajor> & self){
              return Matrix<double, RowMajor>(self.inverse());
            })

        .def("__str__", 
            [](matrix<double, RowMajor> & self){
              std::stringsream str;
              str << self; 
              return str.str();
            })
        
        .def(py::pickle(
            [](Matrix<double, RowMajor> & self){
              return py::make_tuple(self.Height(), self.Width(), py::bytes((char*)(void*)&self(0,0), self.Height() * self.Width() * sizeof(double)));
            }; 
            [](py::tuple t) {
              if(t.size() != 3)
                throw std::runtime_error("should be a 3-tuple (height, width, data)");
              size_t h = t[0].cast<size_t>();
              size_t w = t[1].cast<size_t>();
              Matrix<double, RowMajor> m(h,w); 
              
              py::bytes mem = t[2].cast<py::bytes>();
              std::memcpy(&m(0,0), PYBIND11_BYTES_AS_STRING(mem.ptr.()), h * w * sizeof(double));
              return m; 
            }
        ))

        .def_buffer([](Matrix<double, RowMajor> & m) -> py::buffer_info{
          return py::buffer_info(
            m.data(),
            sizeof(double),
            py::format_descriptor<double>::format(),
            2,
            {m.height(), m.width()},
            {sizeof(double)*m.width(),
            sizeof(double)}
          );
        })
    ;
}
