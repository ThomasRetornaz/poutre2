//
// Created by thomas on 17/12/2025.
//

// NOLINTBEGIN

#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h>
#include <poutre/pixel_processing/compare.hpp>

namespace nb = nanobind;

void init_pp_compare(nb::module_ &mod)
{
  mod.def("compare",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &,
      poutre::CompOpType,
      const poutre::ScalarTypeVariant &,
      const poutre::ScalarTypeVariant &,
      const poutre::ScalarTypeVariant &,
      poutre::IInterface &)>(&poutre::CompareImage));

  mod.def("compare",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &,
      poutre::CompOpType,
      const poutre::IInterface &,
      const poutre::IInterface &,
      const poutre::IInterface &,
      poutre::IInterface &)>(&poutre::CompareImage));

  mod.def("compare",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &,
      poutre::CompOpType,
      const poutre::ScalarTypeVariant &,
      const poutre::IInterface &,
      const poutre::IInterface &,
      poutre::IInterface &)>(&poutre::CompareImage));


  mod.def("compare",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &,
      poutre::CompOpType,
      const poutre::ScalarTypeVariant &,
      const poutre::IInterface &,
      const poutre::ScalarTypeVariant &,
      poutre::IInterface &)>(&poutre::CompareImage));

  mod.def("compare",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &,
      poutre::CompOpType,
      const poutre::IInterface &,
      const poutre::ScalarTypeVariant &,
      const poutre::IInterface &,
      poutre::IInterface &)>(&poutre::CompareImage));

  mod.def("compare",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &,
      poutre::CompOpType,
      const poutre::IInterface &,
      const poutre::ScalarTypeVariant &,
      const poutre::ScalarTypeVariant &,
      poutre::IInterface &)>(&poutre::CompareImage));

  mod.def("compare",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &,
      poutre::CompOpType,
      const poutre::ScalarTypeVariant &,
      const poutre::ScalarTypeVariant &,
      const poutre::IInterface &,
      poutre::IInterface &)>(&poutre::CompareImage));

  mod.def("compare",// cppcheck-suppress cstyleCast
    static_cast<void (*)(const poutre::IInterface &,
      poutre::CompOpType,
      const poutre::IInterface &,
      const poutre::IInterface &,
      const poutre::ScalarTypeVariant &,
      poutre::IInterface &)>(&poutre::CompareImage));
}

// NOLINTEND