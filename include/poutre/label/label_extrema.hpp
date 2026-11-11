/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_LABEL_LABEL_EXTREMA
#define INCLUDE_POUTRE_LABEL_LABEL_EXTREMA

#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/label/label.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace poutre::label {
/**
 * @addtogroup poutre_label_group
 *@{
 */

/*!@brief Label minima with a single "id" per minimum in the output image
 * \warning o_img must be 64 bits (PType_GrayINT64) to avoid overflow
 */
LAB_API void label_minima(const IInterface &i_img, se::Common_NL_SE nl_static, IInterface &o_img);

/*!@brief Label maxima with a single "id" per maximum in the output image
 * \warning o_img must be 64 bits (PType_GrayINT64) to avoid overflow
 */
LAB_API void label_maxima(const IInterface &i_img, se::Common_NL_SE nl_static, IInterface &o_img);

//! @} doxygroup: poutre_label_group
}// namespace poutre::label


#endif /* INCLUDE_POUTRE_LABEL_LABEL_EXTREMA */
