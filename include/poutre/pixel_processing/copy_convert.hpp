/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_PIXEL_PROCESSING_COPY_CONVERT
#define INCLUDE_POUTRE_PIXEL_PROCESSING_COPY_CONVERT

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


#endif /* INCLUDE_POUTRE_PIXEL_PROCESSING_COPY_CONVERT */
