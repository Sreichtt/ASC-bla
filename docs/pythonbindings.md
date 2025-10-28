# Python Bindings

Wir wollen nun unsere in C++ erstellten Klassen und Methoden elegant in Python einbinden.
Durch Ausdrücke wie 

```cpp
py::class_<Matrix<double, RowMajor>>(m, "Matrix")
        .def(py::init<size_t, size_t>(),
            py::arg("height, width"), "create a matrix of given height and width")
        .def("__height__", &Matrix<double, RowMajor>::height,
            "return height of matrix")
        .def("__width__", &Matrix<double, RowMajor>::width,
            "return width of matrix")

```
wrappen wir unsere Klasse Matrix<double,RowMajor> in python.

Nun wollen wir set- und get-Funktionen einbinden:
```cpp
.def("__setitem__", [](Matrix<double, RowMajor> & self, std::tuple<int, int> ind, double v){
          i = std::get<0>(ind);
          j = std::get<1>(ind);
          if(i<0) i += self.height();
          if(j<0) j += self.width();
          if(i<0 || i >= self.height()) throw py::error("Matrix row index out of range");
          if(j<0 || j >= self.width()) throw py::error("Matrix col index out of range");
          self(i,j) = v;
        })
        .def("__getitem__",
           [](Matrix<double, RowMajor> self, std::tuple<int, int> ind) {
               return self(std::get<0>(ind), std::get<1>(ind));
           })
```

Die Funktion shape soll die Höhe und Breite der Matrix zurückgeben.

```cpp
    .def_property_readonly("shape",
            [](const Matrix<double, RowMajor>& self) {
                return std::tuple(self.height(), self.width());
            })
```
unpraktischerweise verwenden wir seit Anfang der VU die Funktionen height() und width() sowie dieselben Namen für die Parameter innerhalb der Matrix-Klasse und nicht rows und cols, aber man versteht den Punkt ja trotzdem.

die Funktionen `__add__`, `__MatMatmul__` und `__Matvecmul__`binden nun die jeweiligen Funktionen ein. 





