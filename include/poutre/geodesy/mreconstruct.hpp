/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_GEODESY_MRECONSTRUCT
#define INCLUDE_POUTRE_GEODESY_MRECONSTRUCT

#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/geodesy/geodesy.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace poutre::geo {
/**
 * @addtogroup poutre_geodesy_group
 *@{
 */
enum class reconstruction_type : std::uint8_t {
  erode,//! geodesic reconstruction by erosion
  dilate,//!  geodesic reconstruction by dilatation
  // dual                          //! dual reconstruction -> leveling instead
};

//! Reconstruction of i_marker under/over i_mask regarding the nl_static SE, put the result in o_img
GEO_API void Reconstruction(reconstruction_type rect_type,
                            const IInterface &i_marker,
                            const IInterface &i_mask,
                            se::Common_NL_SE nl_static,
                            IInterface &o_img);
//! @} doxygroup: poutre_geodesy_group

}// namespace poutre::geo


#endif /* INCLUDE_POUTRE_GEODESY_MRECONSTRUCT */
