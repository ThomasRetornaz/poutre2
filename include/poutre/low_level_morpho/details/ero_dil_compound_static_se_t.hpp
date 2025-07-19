//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   ero_dil_compound_static_se_t.hpp
 * @author Thomas Retornaz
 * @brief  Erode dilate with static SE
 *
 *
 */

#include <memory>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/pixel_processing/details/copy_convert_t.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/low_level_morpho/details/ero_dil_static_se_t.hpp>
#include <poutre/base/trace.hpp>

namespace poutre::llm::details {
/**
 * @addtogroup poutre_llm_group
 *@{
 */

template<typename TIn, typename TOut>
void t_DilateOctagon(const poutre::details::image_t<TIn, 2> &i_img,const int size, poutre::details::image_t<TOut, 2> &o_img)
{
  if ( size == 0 )
  {
    poutre::details::t_Copy(i_img, o_img);
    return;
  }

  if (size == 1) {
    t_Dilate(i_img, se::Common_NL_SE::SECross2D, o_img);
    return;
  }
  const double nb_square_dbl = ((static_cast<double>(size)) / (1 + sqrt(2)));
  const double nb_square_floor = floor(nb_square_dbl);
  int nb_square = 0 ;
  nb_square = static_cast<int>(((nb_square_dbl - nb_square_floor) < 0.5) ? (nb_square_floor) : (nb_square_floor+1));

  auto tmpImg_t= poutre::details::t_CloneGeometry(i_img);  // NOLINT

  t_Dilate(i_img, se::Common_NL_SE::SECross2D, o_img);

  for(int i=1 ; i <size-nb_square ; i++) {
    t_Dilate(o_img, se::Common_NL_SE::SECross2D, *tmpImg_t);
    tmpImg_t->swap(o_img);
  }

  for(int i=0 ; i<nb_square ; i++) {
    t_Dilate(o_img, se::Common_NL_SE::SESquare2D, *tmpImg_t);
    tmpImg_t->swap(o_img);
  }
}

template<typename TIn, typename TOut>
void t_DilateRhombicuboctahedron(const poutre::details::image_t<TIn, 3> &i_img,const int size, poutre::details::image_t<TOut, 3> &o_img)
{
  if ( size == 0 )
  {
    poutre::details::t_Copy(i_img, o_img);
    return;
  }

  if (size == 1) {
    t_Dilate(i_img, se::Common_NL_SE::SECross2D, o_img);
    return;
  }
  const double nb_square_dbl = ((static_cast<double>(size)) / (1 + sqrt(2)));
  const double nb_square_floor = floor(nb_square_dbl);
  int nb_square = 0 ;
  nb_square = static_cast<int>(((nb_square_dbl - nb_square_floor) < 0.5) ? (nb_square_floor) : (nb_square_floor+1));

  auto tmpImg_t= poutre::details::t_CloneGeometry(i_img);  // NOLINT

  t_Dilate(i_img, se::Common_NL_SE::SECross3D, o_img);

  for(int i=1 ; i <size-nb_square ; i++) {
    t_Dilate(o_img, se::Common_NL_SE::SECross3D, *tmpImg_t);
    tmpImg_t->swap(o_img);
  }

  for(int i=0 ; i<nb_square ; i++) {
    t_Dilate(o_img, se::Common_NL_SE::SESquare3D, *tmpImg_t);
    tmpImg_t->swap(o_img);
  }
}

template<typename TIn, typename TOut>
void t_ErodeOctagon(const poutre::details::image_t<TIn, 2> &i_img,const int size, poutre::details::image_t<TOut, 2> &o_img)
{
  if ( size == 0 )
  {
    poutre::details::t_Copy(i_img, o_img);
    return;
  }

  if (size == 1) {
    t_Erode(i_img, se::Common_NL_SE::SECross2D, o_img);
    return;
  }
  const double nb_square_dbl = ((static_cast<double>(size)) / (1 + sqrt(2)));
  const double nb_square_floor = floor(nb_square_dbl);
  int nb_square = 0 ;
  nb_square = static_cast<int>(((nb_square_dbl - nb_square_floor) < 0.5) ? (nb_square_floor) : (nb_square_floor+1));

  auto tmpImg_t= poutre::details::t_CloneGeometry(i_img);  // NOLINT

  t_Erode(i_img, se::Common_NL_SE::SECross2D, o_img);

  for(int i=1 ; i <size-nb_square ; i++) {
    t_Erode(o_img, se::Common_NL_SE::SECross2D, *tmpImg_t);
    tmpImg_t->swap(o_img);
  }

  for(int i=0 ; i<nb_square ; i++) {
    t_Erode(o_img, se::Common_NL_SE::SESquare2D, *tmpImg_t);
    tmpImg_t->swap(o_img);
  }
}

template<typename TIn, typename TOut>
void t_ErodeRhombicuboctahedron(const poutre::details::image_t<TIn, 3> &i_img,const int size, poutre::details::image_t<TOut, 3> &o_img)
{
  if ( size == 0 )
  {
    poutre::details::t_Copy(i_img, o_img);
    return;
  }

  if (size == 1) {
    t_Erode(i_img, se::Common_NL_SE::SECross2D, o_img);
    return;
  }
  const double nb_square_dbl = ((static_cast<double>(size)) / (1 + sqrt(2)));
  const double nb_square_floor = floor(nb_square_dbl);
  int nb_square = 0 ;
  nb_square = static_cast<int>(((nb_square_dbl - nb_square_floor) < 0.5) ? (nb_square_floor) : (nb_square_floor+1));

  auto tmpImg_t= poutre::details::t_CloneGeometry(i_img);  // NOLINT

  t_Erode(i_img, se::Common_NL_SE::SECross3D, o_img);

  for(int i=1 ; i <size-nb_square ; i++) {
    t_Erode(o_img, se::Common_NL_SE::SECross3D, *tmpImg_t);
    tmpImg_t->swap(o_img);
  }

  for(int i=0 ; i<nb_square ; i++) {
    t_Erode(o_img, se::Common_NL_SE::SESquare3D, *tmpImg_t);
    tmpImg_t->swap(o_img);
  }
}


template<typename TIn, typename TOut, ptrdiff_t Rank>
void t_Dilate(const poutre::details::image_t<TIn, Rank> &i_img,
  const poutre::se::Compound_NL_SE compound_nl, const int size, poutre::details::image_t<TOut, Rank> &o_img)
{
  AssertSizesCompatible(i_img, o_img, "t_Dilate incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "t_Dilate incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "t_Dilate output must be != than input images");
  if constexpr (Rank == 2) {
    switch(compound_nl) {
    case poutre::se::Compound_NL_SE::Octagon: {
      return t_DilateOctagon(i_img, size,o_img);
    }
    default: {
      POUTRE_RUNTIME_ERROR("t_Dilate 2D unsupported compound_nl");
    }
    }
  }
  if constexpr (Rank == 3) {
  switch(compound_nl) {
    case poutre::se::Compound_NL_SE::Rhombicuboctahedron: {
      return t_DilateRhombicuboctahedron(i_img, size,o_img);
    }
    default: {
      POUTRE_RUNTIME_ERROR("t_Dilate 3D unsupported compound_nl");
    }
    }
  }
  POUTRE_RUNTIME_ERROR("t_Dilate unsupported compound_nl");
}

template<typename TIn, typename TOut, ptrdiff_t Rank>
void t_Erode(const poutre::details::image_t<TIn, Rank> &i_img,
  const poutre::se::Compound_NL_SE compound_nl, const int size, poutre::details::image_t<TOut, Rank> &o_img)
{
  AssertSizesCompatible(i_img, o_img, "t_Erode incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "t_Erode incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "t_Erode output must be != than input images");
  if constexpr (Rank == 2) {
    switch(compound_nl) {
    case poutre::se::Compound_NL_SE::Octagon: {
      return t_ErodeOctagon(i_img, size,o_img);
    }
    default: {
      POUTRE_RUNTIME_ERROR("t_Erode 2D unsupported compound_nl");
    }
    }
  }
  if constexpr (Rank == 3) {
    switch(compound_nl) {
    case poutre::se::Compound_NL_SE::Rhombicuboctahedron: {
      return t_ErodeRhombicuboctahedron(i_img, size,o_img);
    }
    default: {
      POUTRE_RUNTIME_ERROR("t_Erode 3D unsupported compound_nl");
    }
    }
  }
  POUTRE_RUNTIME_ERROR("t_Erode unsupported compound_nl");
}
//! @} doxygroup: poutre_llm_group
}//poutre::llm::details