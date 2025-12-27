//
// Created by thomas on 18/12/2025.
//
// NOLINTBEGIN
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/unique_ptr.h>
#include <nanobind/stl/vector.h>
#include <poutre/structuring_element/se_interface.hpp>

namespace nb = nanobind;

void init_se_interface(nb::module_ &mod)
{
  nb::class_<poutre::se::IStructuringElement>(mod, "SE", "Common image interface")
    .def("transpose", &poutre::se::IStructuringElement::Transpose)
    .def("type", &poutre::se::IStructuringElement::GetType)
    .def("remove_center", &poutre::se::IStructuringElement::RemoveCenter)
    .def("size", &poutre::se::IStructuringElement::GetSize)
    .def("clone", &poutre::se::IStructuringElement::Clone)
    .def("is_equal", &poutre::se::IStructuringElement::is_equal)
    .def("is_equal_unordered", &poutre::se::IStructuringElement::is_equal_unordered)
    ;
}
// NOLINTEND
