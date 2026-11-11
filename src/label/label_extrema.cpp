/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "poutre/label/label_binary.hpp"


#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/base/types.hpp>
#include <poutre/geodesy/extrema.hpp>
#include <poutre/label/label_extrema.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace poutre::label {

void label_minima(const IInterface &i_img, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("label_minima");
  AssertSizesCompatible(i_img, o_img, "label_minima incompatible size");
  AssertImagesAreDifferent(i_img, o_img, "label_minima output must be != than input images");

  auto img_tmp = poutre::CloneGeometry(i_img);// NOLINT
  poutre::geo::h_concave(i_img, CreatePixelValue(1, i_img.GetPType()), nl_static, *img_tmp);
  poutre::label::label_binary(*img_tmp, nl_static, o_img);
}

void label_maxima(const IInterface &i_img, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("label_maxima");
  AssertSizesCompatible(i_img, o_img, "label_maxima incompatible size");
  AssertImagesAreDifferent(i_img, o_img, "label_maxima output must be != than input images");

  auto img_tmp = poutre::CloneGeometry(i_img);// NOLINT
  poutre::geo::h_convex(i_img, CreatePixelValue(1, i_img.GetPType()), nl_static, *img_tmp);
  poutre::label::label_binary(*img_tmp, nl_static, o_img);
}

}// namespace poutre::label
