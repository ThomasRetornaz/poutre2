//
// Created by thomas on 17/12/2025.
//

// NOLINTBEGIN
#include <nanobind/nanobind.h>
#include <poutre/low_level_morpho/ero_dil.hpp>
#include <poutre/low_level_morpho/ero_dil_line.hpp>

namespace nb = nanobind;

void init_llm_ero_dil(nb::module_ &mod)
{
  mod.def("erode",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &, poutre::se::Common_NL_SE, const int, poutre::IInterface &)>(&poutre::Erode));
  mod.def("erode",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &, poutre::se::Compound_NL_SE, const int, poutre::IInterface &)>(&poutre::Erode));
  mod.def("dilate",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &, poutre::se::Common_NL_SE, const int, poutre::IInterface &)>(&poutre::Dilate));
  mod.def("dilate",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &, poutre::se::Compound_NL_SE, const int, poutre::IInterface &)>(&poutre::Dilate));

  mod.def("dilateX", &poutre::DilateX);
  mod.def("dilateY", &poutre::DilateY);
  mod.def("erodeX", &poutre::ErodeX);
  mod.def("erodeY", &poutre::ErodeY);
}

// NOLINTEND