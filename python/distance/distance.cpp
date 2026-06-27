//
// Created by thomas on 17/12/2025.
//
// NOLINTBEGIN
#include "../../include/poutre/distance/mdistance.hpp"


#include <nanobind/nanobind.h>
#include <poutre/distance/mdistance.hpp>

namespace nb = nanobind;

void init_distance(nb::module_ &mod) { mod.def("mdistance", &poutre::dist::mdistance); }

// NOLINTEND
