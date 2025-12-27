
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   leveling.hpp
 * @author Thomas Retornaz
 * @brief  Upper, Lower Leveling
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

/*!@brief Low leveling
*
A function g is a lower leveling of a function f if and only if
for any couple of neighbouring pixels (p, q) g(p)<g(q)->g(p)>=f
This algorithm modify marker to become a low_leveling of ref
*/
GEO_API void low_leveling(const IInterface &i_ref, const IInterface &i_marker,  se::Common_NL_SE nl_static, IInterface &o_img);

/*!@brief High leveling
*
A function g is a upper leveling of a function f if and only if
for any couple of neighbouring pixels (p, q) g(p)>g(q)->g(p)<=f
This algorithm modify marker to become a high_leveling of ref
*/
GEO_API void high_leveling(const IInterface &i_ref, const IInterface &i_marker,  se::Common_NL_SE nl_static, IInterface &o_img);

/*!@brief leveling
*
A function g is a leveling of a function f if and only if it is both an upper and a lower leveling of the function f
This algorithm modify g (a copy of the marker) to become a leveling of f (the ref)
*/
GEO_API void leveling(const IInterface &i_ref, const IInterface &i_marker,  se::Common_NL_SE nl_static, IInterface &o_img);

//! @} doxygroup: poutre_geodesy_group
}