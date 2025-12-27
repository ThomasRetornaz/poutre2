//
// Created by thomas on 17/12/2025.
//
// NOLINTBEGIN
#include <nanobind/nanobind.h>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace nb = nanobind;

void init_se_types(nb::module_ &mod)
{
  nb::enum_<poutre::se::se_type>(mod, "se_type")
    .value("runtime", poutre::se::se_type::runtime)
    .value("neighbor_list", poutre::se::se_type::neighbor_list)
    .value("image", poutre::se::se_type::image)
    .value("chain", poutre::se::se_type::chain)
    .export_values();

  nb::enum_<poutre::se::Common_NL_SE>(mod, "static_nl_se")
    .value("segx1d", poutre::se::Common_NL_SE::SESegmentX1D)
    .value("square2d", poutre::se::Common_NL_SE::SESquare2D)
    .value("cross2d", poutre::se::Common_NL_SE::SECross2D)
    .value("segx2d", poutre::se::Common_NL_SE::SESegmentX2D)
    .value("segy2d", poutre::se::Common_NL_SE::SESegmentY2D)
    .value("segx3d", poutre::se::Common_NL_SE::SESegmentX3D)
    .value("segy3d", poutre::se::Common_NL_SE::SESegmentY3D)
    .value("segz3d", poutre::se::Common_NL_SE::SESegmentZ3D)
    .value("cross3d", poutre::se::Common_NL_SE::SECross3D)
    .value("square3d", poutre::se::Common_NL_SE::SESquare3D)
    .export_values();

  nb::enum_<poutre::se::Compound_NL_SE>(mod, "static_compound_se")
    .value("octagon", poutre::se::Compound_NL_SE::Octagon)
    .value("rhombicuboctahedron", poutre::se::Compound_NL_SE::Rhombicuboctahedron)
    .export_values();
}
// NOLINTEND