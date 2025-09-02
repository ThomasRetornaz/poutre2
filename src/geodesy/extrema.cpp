
// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/base/types.hpp>
#include <poutre/geodesy/extrema.hpp>
#include <poutre/geodesy/mreconstruct.hpp>
#include <poutre/pixel_processing/arith.hpp>
#include <poutre/pixel_processing/compare.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace poutre::geo
{

void h_minima(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("h_minima");
  AssertSizesCompatible(i_img, o_img, "h_minima incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "h_minima incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "h_minima output must be != than input images");

  auto img_tmp = poutre::CloneGeometry(i_img); // NOLINT
  poutre::ArithSaturatedAddConstant(i_img, pvalue, *img_tmp);
  poutre::geo::Reconstruction(poutre::geo::reconstruction_type::erode,*img_tmp,i_img, nl_static, o_img);
}

void h_maxima(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("h_maxima");
  AssertSizesCompatible(i_img, o_img, "h_maxima incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "h_maxima incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "h_maxima output must be != than input images");

  auto img_tmp = poutre::CloneGeometry(i_img); // NOLINT
  poutre::ArithSaturatedSubConstant(i_img, pvalue, *img_tmp);
  poutre::geo::Reconstruction(poutre::geo::reconstruction_type::dilate,*img_tmp,i_img, nl_static, o_img);
}

void h_concave(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("h_concave");
  AssertSizesCompatible(i_img, o_img, "h_concave incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "h_concave incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "h_concave output must be != than input images");

  auto img_tmp = poutre::CloneGeometry(i_img); // NOLINT
  h_minima(i_img, pvalue, nl_static, *img_tmp);
  poutre::ArithSaturatedSubImage(*img_tmp,i_img, o_img);
}

void h_convex(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("h_convex");
  AssertSizesCompatible(i_img, o_img, "h_convex incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "h_convex incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "h_convex output must be != than input images");

  auto img_tmp = poutre::CloneGeometry(i_img); // NOLINT
  h_maxima(i_img, pvalue, nl_static, *img_tmp);
  poutre::ArithSaturatedSubImage(i_img, *img_tmp, o_img);
}

void dynamic_pseudo_opening(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("dynamic_pseudo_opening");
  AssertSizesCompatible(i_img, o_img, "dynamic_pseudo_opening incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "dynamic_pseudo_opening incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "dynamic_pseudo_opening output must be != than input images");

  auto img_tmp = poutre::CloneGeometry(i_img); // NOLINT
  auto img_extrema = poutre::CloneGeometry(i_img); // NOLINT
  auto img_mask = poutre::CloneGeometry(i_img); // NOLINT

  poutre::ArithSaturatedSubConstant(i_img, pvalue, *img_tmp);
  poutre::geo::Reconstruction(poutre::geo::reconstruction_type::dilate,*img_tmp,i_img, nl_static, *img_extrema);
  // Create marker : look for remaining maxima, buggy for signed type ?
  poutre::CompareImage(*img_extrema, poutre::CompOpType::CompOpEqual, *img_tmp, i_img,get_lowest(i_img.GetPType()), *img_mask);
  // remove test with lowest
  poutre::CompareImage(*img_extrema, poutre::CompOpType::CompOpDiff, get_lowest(i_img.GetPType()), *img_mask, get_lowest(i_img.GetPType()), *img_tmp);
  poutre::geo::Reconstruction(poutre::geo::reconstruction_type::dilate,*img_tmp,i_img, nl_static, o_img);
}

void dynamic_pseudo_closing(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("dynamic_pseudo_closing");
  AssertSizesCompatible(i_img, o_img, "dynamic_pseudo_closing incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "dynamic_pseudo_closing incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "dynamic_pseudo_closing output must be != than input images");

  auto img_tmp = poutre::CloneGeometry(i_img); // NOLINT
  auto img_extrema = poutre::CloneGeometry(i_img); // NOLINT
  auto img_mask = poutre::CloneGeometry(i_img); // NOLINT

  poutre::ArithSaturatedAddConstant(i_img, pvalue, *img_tmp);
  poutre::geo::Reconstruction(poutre::geo::reconstruction_type::erode,*img_tmp,i_img, nl_static, *img_extrema);
  // Create marker : look for remaining maxima, buggy for signed type ?

  poutre::CompareImage(*img_extrema, poutre::CompOpType::CompOpEqual, *img_tmp, i_img,  get_highest(i_img.GetPType()), *img_mask);
  // remove test with lowest
  poutre::CompareImage(*img_extrema, poutre::CompOpType::CompOpDiff, get_highest(i_img.GetPType()), *img_mask, get_highest(i_img.GetPType()), *img_tmp);
  poutre::geo::Reconstruction(poutre::geo::reconstruction_type::erode,*img_tmp,i_img, nl_static, o_img);
}
}
