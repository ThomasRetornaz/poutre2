
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   label_flat_zones.hpp
 * @author Thomas Retornaz
 * @brief  Label flat zones
 *
 *
 */

#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/label/label.hpp>

namespace poutre::label {
/**
 * @addtogroup poutre_label_group
 *@{
 */

/*!@brief Label all flat zones according to nl_static
* \warning o_img must be 64 bits (PType_GrayINT64) to avoid overflow
* @return number of labels
*/
LAB_API size_t label_flat_zones(const IInterface &i_img,  se::Common_NL_SE nl_static, IInterface &o_img);

//! @} doxygroup: poutre_label_group
}