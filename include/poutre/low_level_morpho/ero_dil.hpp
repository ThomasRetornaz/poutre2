//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   ero_dil.hpp
 * @author Thomas Retornaz
 * @brief  Erosion/Dilatation over images
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

//! Erode iter times i_img regarding the nl_static SE, put the result in o_img
LLM_API void Erode(const IInterface &i_img, se::Common_NL_SE nl_static, const int iter, IInterface &o_img);

//! Dilate iter times i_img regarding the nl_static SE, put the result in o_img
LLM_API void Dilate(const IInterface &i_img, se::Common_NL_SE nl_static, const int iter, IInterface &o_img);

//! Erode i_img regarding the compound SE, put the result in o_img
LLM_API void Erode(const IInterface &i_img, se::Compound_NL_SE nl_compound, const int size, IInterface &o_img);

//! Dilate i_img regarding the compound SE, put the result in o_img
LLM_API void Dilate(const IInterface &i_img, se::Compound_NL_SE nl_compound, const int size, IInterface &o_img);

//! Erode i_img regarding the SE, put the result in o_img
LLM_API void Erode(const IInterface &i_img, const se::IStructuringElement& str_el, IInterface &o_img);

//! Dilate i_img regarding the SE, put the result in o_img
LLM_API void Dilate(const IInterface &i_img, const se::IStructuringElement& str_el, IInterface &o_img);

//! @} doxygroup: image_processing_llm_group
}// namespace poutre