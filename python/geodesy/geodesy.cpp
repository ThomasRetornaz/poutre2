//
// Created by thomas on 17/12/2025.
//
// NOLINTBEGIN
#include <nanobind/nanobind.h>
#include <poutre/geodesy/extrema.hpp>
#include <poutre/geodesy/leveling.hpp>
#include <poutre/geodesy/mreconstruct.hpp>

namespace nb = nanobind;

void init_geodesy(nb::module_ &mod)
{
  nb::enum_<poutre::geo::reconstruction_type>(mod, "RecType")
    .value("dilate", poutre::geo::reconstruction_type::dilate)
    .value("erode", poutre::geo::reconstruction_type::erode)
    .export_values();

  mod.def("h_minima", &poutre::geo::h_maxima);
  mod.def("h_minima", &poutre::geo::h_minima);
  mod.def("h_concave", &poutre::geo::h_concave);
  mod.def("h_convex", &poutre::geo::h_convex);
  mod.def("dynamic_pseudo_closing", &poutre::geo::dynamic_pseudo_closing);
  mod.def("dynamic_pseudo_opening", &poutre::geo::dynamic_pseudo_opening);

  mod.def("high_leveling", &poutre::geo::high_leveling);
  mod.def("low_leveling", &poutre::geo::low_leveling);
  mod.def("leveling", &poutre::geo::leveling);
}

// NOLINTEND