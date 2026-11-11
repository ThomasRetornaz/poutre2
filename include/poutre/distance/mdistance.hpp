/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_DISTANCE_MDISTANCE
#define INCLUDE_POUTRE_DISTANCE_MDISTANCE

#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/distance/distance.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace poutre::dist {
/**
 * @addtogroup poutre_distance_group
 *@{
 */

/*!@brief Computes the morphological distance function on images. Pixel's equal to 0 are considered as background.
 * \warning o_img must be 64 bits (PType_GrayINT64) to avoid overflow
 */
DIST_API void mdistance(const IInterface &i_img, se::Common_NL_SE nl_static, IInterface &o_img);

//! @} doxygroup: poutre_distance_group

}// namespace poutre::dist


#endif /* INCLUDE_POUTRE_DISTANCE_MDISTANCE */
