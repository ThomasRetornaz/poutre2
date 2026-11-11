/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_LOW_LEVEL_MORPHO_ERO_DIL
#define INCLUDE_POUTRE_LOW_LEVEL_MORPHO_ERO_DIL

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
LLM_API void Erode(const IInterface &i_img, se::Common_NL_SE nl_static, int iter, IInterface &o_img);

//! Dilate iter times i_img regarding the nl_static SE, put the result in o_img
LLM_API void Dilate(const IInterface &i_img, se::Common_NL_SE nl_static, int iter, IInterface &o_img);

//! Erode i_img regarding the compound SE, put the result in o_img
LLM_API void Erode(const IInterface &i_img, se::Compound_NL_SE nl_compound, int size, IInterface &o_img);

//! Dilate i_img regarding the compound SE, put the result in o_img
LLM_API void Dilate(const IInterface &i_img, se::Compound_NL_SE nl_compound, int size, IInterface &o_img);

//! Erode i_img regarding the SE, put the result in o_img
LLM_API void Erode(const IInterface &i_img, const se::IStructuringElement& str_el, IInterface &o_img);

//! Dilate i_img regarding the SE, put the result in o_img
LLM_API void Dilate(const IInterface &i_img, const se::IStructuringElement& str_el, IInterface &o_img);

//! @} doxygroup: image_processing_llm_group
}// namespace poutre


#endif /* INCLUDE_POUTRE_LOW_LEVEL_MORPHO_ERO_DIL */
