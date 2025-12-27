//
// Created by thomas on 09/12/2025.
//
// NOLINTBEGIN
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/unique_ptr.h>
#include <nanobind/stl/vector.h>
#include <poutre/base/image_interface.hpp>

namespace nb = nanobind;

void init_base_image(nb::module_ &mod)
{
  nb::class_<poutre::IInterface>(mod, "Image", "Common image interface")
    .def("ptype", &poutre::IInterface::GetPType)
    .def("ctype", &poutre::IInterface::GetCType)
    .def("shape", &poutre::IInterface::GetShape)
    .def("rank", &poutre::IInterface::GetRank)
    .def("__repr__", &poutre::IInterface::str);
  ;

  mod.def("as_sizes_compatible", &poutre::AsSizesCompatible, "test if two images have compatible sizes");
  mod.def("as_types_compatible", &poutre::AsTypesCompatible, "test if two images have compatible types");
  mod.def("are_images_different", &poutre::AreImagesDifferent, "test if two images belong to different memory");

  mod.def("factory_image", &poutre::Create, "Factory to create image from given shape, types");

  mod.def("from_string", &poutre::ImageFromString, "Factory to create image from given string");
  mod.def("to_string", &poutre::ImageToString, "Serialize the whole image to string");
}
// NOLINTEND