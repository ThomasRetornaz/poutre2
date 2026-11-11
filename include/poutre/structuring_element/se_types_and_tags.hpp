/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_STRUCTURING_ELEMENT_SE_TYPES_AND_TAGS
#define INCLUDE_POUTRE_STRUCTURING_ELEMENT_SE_TYPES_AND_TAGS

namespace poutre::se {
/**
 * @addtogroup poutre_se_interface_group
 *@{
 */

struct adaptative_neighborhood_tag
{
};

struct runtime_neighborhood_tag : adaptative_neighborhood_tag
{
};

struct static_neighborhood_tag : runtime_neighborhood_tag
{
};

enum class se_type : std::uint8_t {
  runtime,//! SE computed at runtime (think amoeba)
  neighbor_list,//!< Neighbor list structuring element
  image,//!< Image structuring element
  chain,//!< Chain of structuring element
};

enum class Common_NL_SE : std::uint8_t {
  Undef,//!< Undefined predefined nl
  SESegmentX1D,//!< 1D segment SE
  SESquare2D,//!< 2D Square SE
  SECross2D,//!< 2D Cross SE
  SESegmentX2D,//!< 2D Horizontal SE
  SESegmentY2D,//!< 2D Vertical SE
  SESegmentX3D,//!< 3D SE along X
  SESegmentY3D,//!< 3D SE along Y
  SESegmentZ3D,//!< 3D SE along Z
  SECross3D,//!< 3D Cross SE
  SESquare3D,//!< 3D Square SE
};

enum class Compound_NL_SE : std::uint8_t {
  Undef,
  Octagon,//!< alternate 2D Cross and Square SE
  Rhombicuboctahedron,//!< alternate 3D Cross and Square SE
};

//! @} doxygroup: poutre_se_interface_group
}// namespace poutre::se


#endif /* INCLUDE_POUTRE_STRUCTURING_ELEMENT_SE_TYPES_AND_TAGS */
