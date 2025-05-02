//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   arith.hpp
 * @author Thomas Retornaz
 * @brief  Arithmetics operators over images
 *
 *
 */

#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp> //for ScalarTypeVariant
#include <poutre/pixel_processing/pixel_processing.hpp>

namespace poutre {
/**
 * @addtogroup image_processing_arith_group
 * @ingroup image_processing_group
 *@{
 */

//! Negate i_img in o_img, images must have the same type and must be scalar
PP_API void ArithInvertImage(const IInterface &i_img, IInterface &o_img);

//! Compute the supremum of i_img1, i_img2 put the result in o_img, images must have the same type and must be scalar
PP_API void ArithSupImage(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img);

//! Compute the supremum of i_img1, i_img2 put the result in o_img, images must have the same type and must be scalar
PP_API void ArithInfImage(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img);

//! Compute the saturated add of i_img1, i_img2 put the result in o_img, images must have the same type and must be
//! scalar
PP_API void ArithSaturatedAddImage(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img);

//! Compute the saturated diff of i_img1, i_img2 put the result in o_img, images must have the same type and must be
//! scalar
PP_API void ArithSaturatedSubImage(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img);

//! Compute the saturated add of i_img, and constant pvalue and put the result in o_img, images must have the same type
//! and must be scalar
PP_API void ArithSaturatedAddConstant(const IInterface &i_img, const ScalarTypeVariant &pvalue, IInterface &o_img);

//! Compute the saturated add of i_img, and constant pvalue and put the result in o_img, images must have the same type
//! and must be scalar
PP_API void ArithSaturatedSubConstant(const IInterface &i_img, const ScalarTypeVariant &pvalue, IInterface &o_img);

//! @} doxygroup: image_processing_arith_group
}// namespace poutre