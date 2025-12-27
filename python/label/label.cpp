//
// Created by thomas on 17/12/2025.
//
// NOLINTBEGIN
#include <nanobind/nanobind.h>
#include <poutre/label/label_binary.hpp>
#include <poutre/label/label_extrema.hpp>
#include <poutre/label/label_flat_zones.hpp>

namespace nb = nanobind;

void init_label(nb::module_ &mod)
{
  mod.def("label_binary", &poutre::label::label_binary);
  mod.def("label_flat_zones", &poutre::label::label_flat_zones);
  mod.def("label_maxima", &poutre::label::label_maxima);
  mod.def("label_minima", &poutre::label::label_minima);
}