//
// Created by thomas on 17/12/2025.
//

// NOLINTBEGIN
#include <nanobind/nanobind.h>
#include <poutre/pixel_processing/arith.hpp>

namespace nb = nanobind;

void init_pp_arith(nb::module_ &mod)
{
  mod.def("arith_invert_image", &poutre::ArithInvertImage);
  mod.def("arith_inf_image", &poutre::ArithInfImage);
  mod.def("arith_sup_image", &poutre::ArithSupImage);
  mod.def("arith_saturated_add_image", &poutre::ArithSaturatedAddImage);
  mod.def("arith_saturated_sub_image", &poutre::ArithSaturatedSubImage);
  mod.def("arith_saturated_add_constant", &poutre::ArithSaturatedAddConstant);
  mod.def("arith_saturated_sub_constant", &poutre::ArithSaturatedSubConstant);
}
// NOLINTEND