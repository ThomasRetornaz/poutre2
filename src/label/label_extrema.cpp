
// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "poutre/label/label_binary.hpp"


#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/base/types.hpp>
#include <poutre/label/label_extrema.hpp>
#include <poutre/geodesy/extrema.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace poutre::label {

void label_minima(const IInterface &i_img,  se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("label_minima");
  AssertSizesCompatible(i_img, o_img, "label_minima incompatible size");
  AssertImagesAreDifferent(i_img, o_img, "label_minima output must be != than input images");

  auto img_tmp = poutre::CloneGeometry(i_img); // NOLINT
  poutre::geo::h_concave(i_img, CreatePixelValue(1,i_img.GetPType()),nl_static, *img_tmp);
  poutre::label::label_binary(*img_tmp, nl_static, o_img);
}

void label_maxima(const IInterface &i_img,  se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("label_maxima");
  AssertSizesCompatible(i_img, o_img, "label_maxima incompatible size");
  AssertImagesAreDifferent(i_img, o_img, "label_maxima output must be != than input images");

  auto img_tmp = poutre::CloneGeometry(i_img); // NOLINT
  poutre::geo::h_convex(i_img, CreatePixelValue(1,i_img.GetPType()),nl_static, *img_tmp);
  poutre::label::label_binary(*img_tmp, nl_static, o_img);
}

}
