//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file transpose_op_t.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief helper to transpose images
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/types.hpp>
#include <poutre/base/types_traits.hpp>
#include <poutre/base/trace.hpp>

namespace poutre::details {
/**
 * @addtogroup image_processing_isometry_group Image Processing transpose helpers
 * @ingroup image_processing_group
 *@{
 */


template<typename T,
         ptrdiff_t Rank,
         template<typename, ptrdiff_t>
         class View1,
         template<typename, ptrdiff_t>
         class View2>
struct t_transposeDispatcher
{
  // generic case not supported yet
  static_assert(false, "To be implemented for generic views");
};

template<typename T> struct t_transposeDispatcher<T, 2, av::array_view, av::array_view>
{
  void operator()(const av::array_view<const T, 2> &i_vin, const av::array_view<T, 2> &o_vout) const
  {
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    // check bound compatibility
    auto   ibd    = i_vin.bound();
    auto   obd    = o_vout.bound();

    scoord oysize = obd[1];
    scoord oxsize = obd[0];
    scoord ysize  = ibd[1];
    scoord xsize  = ibd[0];

    POUTRE_CHECK(oysize == xsize, "ibd[0]!=obd[1] bound not compatible");
    POUTRE_CHECK(oxsize == ysize, "ibd[1]!=obd[0] bound not compatible");

    auto i_vinbeg  = i_vin.data();
    auto o_voutbeg = o_vout.data();

    for( scoord y = 0; y < oysize; y++ ) {
      for( scoord x = 0; x < oxsize; x++ ) {
        o_voutbeg[y*oxsize + x] = i_vinbeg[x*oysize+y];
      }
    }
  }
};


// todo SIMD ?

template<typename T,
         ptrdiff_t Rank,
         template<typename, ptrdiff_t>
         class ViewIn,
         template<typename, ptrdiff_t>
         class ViewOut>
  void t_transpose(const ViewIn<const T, Rank> &i_vin, ViewOut<T, Rank> &o_vout)
{
  t_transposeDispatcher<T, Rank, ViewIn, ViewOut> dispatcher;
  dispatcher(i_vin, o_vout);
}

template<typename T, ptrdiff_t Rank>
void t_transpose(const image_t<T, Rank> &i_img, image_t<T, Rank> &o_img)
{
  POUTRE_ENTERING("t_transpose");
  AssertAsTypesCompatible(i_img, o_img, "t_transpose incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "t_transpose output must be != than input images");
  auto i_vin = view(i_img);
  auto o_vout = view(o_img);
  t_transpose(i_vin,o_vout);
}
//! @} doxygroup: image_processing_arith_group
}// namespace poutre::details
