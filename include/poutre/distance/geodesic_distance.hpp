
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   geodesic_distance.hpp
 * @author Thomas Retornaz
 * @brief  Morphological geodesic distance transform
 *
 *
 */

#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/distance/distance.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace poutre::dist {
/**
 * @addtogroup poutre_distance_group
 *@{
 */

/*!@brief Computes geodesic distance from the marker (i_marker), and inside the mask (i_mask). Pixel's equal to 0 are
 * considered as background.
 * \warning o_img must be 64 bits (PType_GrayINT64) to avoid overflow
 */
DIST_API void geodesic_distance(const IInterface &i_marker,
  const IInterface &i_mask,
  se::Common_NL_SE nl_static,
  IInterface &o_img);

//! @} doxygroup: poutre_distance_group

}// namespace poutre::dist