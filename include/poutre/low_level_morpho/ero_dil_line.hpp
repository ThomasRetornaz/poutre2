
/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
 
#ifndef INCLUDE_POUTRE_LOW_LEVEL_MORPHO_ERO_DIL_LINE
#define INCLUDE_POUTRE_LOW_LEVEL_MORPHO_ERO_DIL_LINE

#include <poutre/base/image_interface.hpp>
#include <poutre/low_level_morpho/low_level_morpho.hpp>

namespace poutre {
/**
 * @addtogroup image_processing_llm_group
 * @ingroup image_processing_group
 *@{
 */

/*!@brief Erode i_img along X regarding a line segment SE (SE size = 2* size_half_segment+1), put the result in o_img
 * @warning 1D and 2D only
 */
LLM_API void ErodeX(const IInterface &i_img, ptrdiff_t size_half_segment, IInterface &o_img);

/*!@brief Dilate i_img along X regarding a line segment SE (SE size = 2* size_half_segment+1), put the result in o_img
 * @warning 1D and 2D only
 */
LLM_API void DilateX(const IInterface &i_img, ptrdiff_t size_half_segment, IInterface &o_img);

/*!@brief Erode i_img along Y regarding a line segment SE (SE size = 2* size_half_segment+1), put the result in o_img
 * @warning 2D only
 */
LLM_API void ErodeY(const IInterface &i_img, ptrdiff_t size_half_segment, IInterface &o_img);

/*!@brief Dilate i_img along Y regarding a line segment SE (SE size = 2* size_half_segment+1), put the result in o_img
 * @warning 1D and 2D only
 */
LLM_API void DilateY(const IInterface &i_img, ptrdiff_t size_half_segment, IInterface &o_img);


//! @} doxygroup: image_processing_llm_group
}// namespace poutre


#endif /* INCLUDE_POUTRE_LOW_LEVEL_MORPHO_ERO_DIL_LINE */
