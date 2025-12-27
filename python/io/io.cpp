//
// Created by thomas on 17/12/2025.
//
// NOLINTBEGIN
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/unique_ptr.h>
#include <poutre/io/loader.hpp>
#include <poutre/io/writer.hpp>

namespace nb = nanobind;

void init_io(nb::module_ &mod)
{
  nb::class_<poutre::io::ImageLoader>(mod, "ImageLoader", "Base image loader")
    .def(nb::init<>())
    .def("set_name", &poutre::io::ImageLoader::SetName, nb::rv_policy::reference_internal)
    .def("set_path", &poutre::io::ImageLoader::SetPath, nb::rv_policy::reference_internal)
    .def("load", &poutre::io::ImageLoader::Load);

  nb::class_<poutre::io::ImageWriter>(mod, "ImageWriter", "Base image writer")
    .def(nb::init<>())
    .def("set_name", &poutre::io::ImageWriter::SetName, nb::rv_policy::reference_internal)
    .def("set_path", &poutre::io::ImageWriter::SetPath, nb::rv_policy::reference_internal)
    .def("write", &poutre::io::ImageWriter::Write);
}

// NOLINTEND