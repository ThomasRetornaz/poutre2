
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   copy_convert.hpp
 * @author Thomas Retornaz
 * @brief  Copy/Convert image helpers
 *
 *
 */

#include <poutre/base/image_interface.hpp>
#include <poutre/pixel_processing/pixel_processing.hpp>

namespace poutre {
/**
 * @addtogroup image_processing_copy_group
 * @ingroup image_processing_group
 *@{
 */

//! Deep clone of provided image
PP_API std::unique_ptr<IInterface> Clone(const IInterface &i_img1);

//! Clone of provided image but no data copied
PP_API std::unique_ptr<IInterface> CloneGeometry(const IInterface &i_img1);

//! Convert the provided image: using same geometries  but with different @c  CompoundType and @c
//! PType, data is copied via hard casting
PP_API std::unique_ptr<IInterface> Convert(const IInterface &i_img1, CompoundType ctype, PType ptype);

//! Convert the provided image: using same geometries  but with different @c  CompoundType and @c
//! PType, no data is copied
PP_API std::unique_ptr<IInterface> ConvertGeometry(const IInterface &i_img1, CompoundType ctype, PType ptype);

//! Create a clone of provided image using same geometries but with different  @c PType, no data
//! is copied
PP_API std::unique_ptr<IInterface> ConvertGeometry(const IInterface &i_img1, PType ptype);

//! Copy i_img1 in o_img, images must have the same type
PP_API void CopyInto(const IInterface &i_img, IInterface &o_img);

//! Copy i_img1 in o_img, @warning with hard casting 
PP_API void ConvertInto(const IInterface &i_img, IInterface &o_img);

// TODO add crop

//! @} doxygroup: image_processing_copy_group
}// namespace poutre
