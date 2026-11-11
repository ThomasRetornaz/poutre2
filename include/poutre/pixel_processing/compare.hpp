/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_PIXEL_PROCESSING_COMPARE
#define INCLUDE_POUTRE_PIXEL_PROCESSING_COMPARE

#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>//for ScalarTypeVariant
#include <poutre/pixel_processing/pixel_processing.hpp>

namespace poutre {

/**
 * @addtogroup image_processing_compare_group
 * @ingroup image_processing_group
 *@{
 */

PP_API void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const ScalarTypeVariant &i_valtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img);


PP_API void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const IInterface &i_imgtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img);

PP_API void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const IInterface &i_imgtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img);


PP_API void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const IInterface &i_imgtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img);

PP_API void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const ScalarTypeVariant &i_valtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img);


PP_API void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const ScalarTypeVariant &i_valtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img);


PP_API void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const ScalarTypeVariant &i_valtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img);

PP_API void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const IInterface &i_imgtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img);

//! @} doxygroup: image_processing_compare_group
}// namespace poutre


#endif /* INCLUDE_POUTRE_PIXEL_PROCESSING_COMPARE */
