
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   mreconstruct.hpp
 * @author Thomas Retornaz
 * @brief  Geodesic reconstructions
 *
 *
 */

#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/geodesy/geodesy.hpp>

namespace poutre::geo {
/**
 * @addtogroup poutre_geodesy_group
 *@{
 */
enum class reconstruction_type {
  erode,                        //! geodesic reconstruction by erosion
  dilate,                       //!  geodesic reconstruction by dilatation
  // dual                          //! dual reconstruction -> leveling instead
};

//! Reconstruction of i_marker under/over i_mask regarding the nl_static SE, put the result in o_img
GEO_API void Reconstruction(reconstruction_type rect_type,const IInterface &i_marker, const IInterface &i_mask, se::Common_NL_SE nl_static, IInterface &o_img);
//! @} doxygroup: poutre_geodesy_group

}