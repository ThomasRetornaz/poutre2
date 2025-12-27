//
// Created by thomas on 17/12/2025.
//

// NOLINTBEGIN
#include <nanobind/nanobind.h>
#include <nanobind/stl/unique_ptr.h>
#include <poutre/pixel_processing/copy_convert.hpp>

namespace nb = nanobind;

void init_pp_copy_convert(nb::module_ &mod)
{
  mod.def("clone", &poutre::Clone);
  mod.def("clone_geometry", &poutre::CloneGeometry);
  mod.def("convert", &poutre::Convert);
  mod.def("convert_geometry",// cppcheck-suppress cstyleCast
    static_cast<std::unique_ptr<poutre::IInterface> (*)(
      const poutre::IInterface &, poutre::CompoundType, poutre::PType)>(&poutre::ConvertGeometry));
  mod.def("copy_into", &poutre::CopyInto);
  mod.def("convert_into", &poutre::ConvertInto);
}

// NOLINTEND