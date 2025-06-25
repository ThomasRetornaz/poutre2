//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   ero_dil_line.hpp
 * @author Thomas Retornaz
 * @brief  Erosion/Dilatation with line structuring elements over images
 *
 *
 */

#include <poutre/base/image_interface.hpp>
#include <poutre/low_level_morpho/low_level_morpho.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/structuring_element/se_interface.hpp>

namespace poutre {
/**
 * @addtogroup image_processing_llm_group
 * @ingroup image_processing_group
 *@{
 */

/*!@brief Erode i_img regarding a line segment SE (SE size = 2* size_half_segment+1), put the result in o_img
 * @warning 1D and 2D only
 */
LLM_API void ErodeX(const IInterface &i_img, const ptrdiff_t size_half_segment, IInterface &o_img);

/*!@brief Dilate i_img regarding a line segment SE (SE size = 2* size_half_segment+1), put the result in o_img
 * @warning 1D and 2D only
 */
LLM_API void DilateX(const IInterface &i_img, const ptrdiff_t size_half_segment, IInterface &o_img);


//! @} doxygroup: image_processing_llm_group
}// namespace poutre