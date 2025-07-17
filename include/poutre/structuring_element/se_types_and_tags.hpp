
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   se_types_and_tags.hpp
 * @author Thomas Retornaz
 * @brief  Definition of SE enum and tags
 *
 *
 */

namespace poutre::se {
/**
 * @addtogroup poutre_se_interface_group Str
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

enum class se_type {
  runtime,                            //! SE computed at runtime (think amoeba)
  neighbor_list,                      //!< Neighbor list structuring element
  image,                              //!< Image structuring element
  chain,                              //!< Chain of structuring element
};

enum class Common_NL_SE
{
  Undef,             //!< Undefined predefined nl
  SESegmentX1D,      //!< 1D segment SE
  SESquare2D,        //!< 2D Square SE
  SECross2D,         //!< 2D Cross SE
  SESegmentX2D,      //!< 2D Horizontal SE
  SESegmentY2D,      //!< 2D Vertical SE
  SESegmentX3D,      //!< 3D SE along X
  SESegmentY3D,      //!< 3D SE along Y
  SESegmentZ3D,      //!< 3D SE along Z
  SECross3D,         //!< 3D Cross SE
  SESquare3D,        //!< 3D Square SE
};

enum class Compound_NL_SE
{
  Undef,
  Octagon,             //!< alternate 2D Cross and Square SE
  Rhombicuboctahedron  //!< alternate 3D Cross and Square SE
};

//! @} doxygroup: poutre_se_interface_group
}// namespace poutre