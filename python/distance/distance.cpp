/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
// NOLINTBEGIN


#include <nanobind/nanobind.h>
#include <poutre/distance/geodesic_distance.hpp>
#include <poutre/distance/mdistance.hpp>

namespace nb = nanobind;

void init_distance(nb::module_ &mod)
{
  mod.def("mdistance", &poutre::dist::mdistance);
  mod.def("geodesic_distance", &poutre::dist::geodesic_distance);
}

// NOLINTEND
