//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   ero_dil_runtime_nl_se_t.hpp
 * @author Thomas Retornaz
 * @brief  Erode dilate with runtime neighor list SE
 *
 *
 */

#include <algorithm>
#include <memory>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
//#include <poutre/pixel_processing/details/copy_convert_t.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/structuring_element/details/neighbor_list_se_t.hpp>
#include <poutre/base/trace.hpp>

namespace poutre::llm::details {
/**
 * @addtogroup poutre_llm_group
 *@{
 */

template<typename T1, typename T2> struct BinOpInfRuntime
{
public:
  static constexpr T1                neutral = std::numeric_limits<T1>::max();
  static T2 process(const T1 &A0, const T1 &A1) { return static_cast<T2>(std::min<T1>(A0, A1)); }
};

template<typename T1, typename T2> struct BinOpSupRuntime
{
public:
  static constexpr T1                neutral = std::numeric_limits<T1>::lowest();
  static T2 process(const T1 &A0, const T1 &A1) { return static_cast<T2>(std::max<T1>(A0, A1)); }
};

//-----STRIDED VIEW OR Image_t generic with static SE
  template<typename T1,
           typename T2,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2,
           class BinOp>
  struct t_ErodeDilateDispatcherRuntime
{
    void operator()(const View1<const T1, Rank> &i_vin, const poutre::se::details::neighbor_list_t<Rank>& nl_runtime, const View2<T2, Rank> &o_vout) const
    {
      POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");

      // More runtime dispatch
      auto vInbound   = i_vin.bound();
      auto vOutbound  = o_vout.bound();
      auto stridevIN  = i_vin.stride();
      auto stridevOut = o_vout.stride();
      if( vInbound == vOutbound && stridevIN == stridevOut ) // same bound + same stride -> one idx
      {
        auto beg1 = begin(vInbound);
        auto end1 = end(vInbound);
        for( ; beg1 != end1; ++beg1 )
        {
          auto val = BinOp::neutral;
          for( const auto &idxnl : nl_runtime )
          {
            auto currentidx = *beg1 + idxnl;
            if( !vInbound.contains(currentidx) )
            {continue;}
            val = BinOp::process(val, i_vin[currentidx]);
          }
          o_vout[*beg1] = static_cast<T2>(val);
        }
        // TODO stride==1 in least significant dimension
      }
      else // default two idx
      {
        auto beg1 = begin(vInbound);
        auto end1 = end(vInbound);
        auto beg2 = begin(vOutbound);
        for( ; beg1 != end1; ++beg1, ++beg2 )
        {
          auto val = BinOp::neutral;
          for( const auto &idxnl : nl_runtime )
          {
            auto currentidx = *beg1 + idxnl;
            if( !vInbound.contains(currentidx) )
            {continue;}
            val = BinOp::process(val, i_vin[currentidx]);
          }
          o_vout[*beg2] = static_cast<T2>(val);
        }
      }
    }
  };


// specialisation array_view 2d
//----2D array_view/DenseImage<T,2>
template<typename T1, typename T2, class BinOp>
void t_ErodeDilateIterateBorderArrayView2DRuntimeHelper(const T1 *&i_vin,
                                                 const poutre::se::details::neighbor_list_t<2>& nl_runtime,
                                                 T2 *&      o_vout,
                                                 scoord     xsize, //NOLINT
                                                 scoord     ysize, //NOLINT
                                                 scoord     stride,
                                                 ptrdiff_t  i_XCenter,
                                                 ptrdiff_t  i_YCenter)
{
  auto      i_vinbeg   = i_vin;
  auto      o_voutbeg  = o_vout;
  auto      val        = BinOp::neutral;
  poutre::details::av::idx2d     center{i_YCenter, i_XCenter};
  for( const auto &idxnl : nl_runtime )
  {
    scoord const currentx = center[1] + idxnl[1];
    scoord const currenty = center[0] + idxnl[0];
    if( currentx < 0 ) {
      continue;
    }
    if( currenty < 0 ) {
      continue;
    }
    if( currentx >= xsize ) {
      continue;
    }
    if( currenty >= ysize ) {
      continue;
    }
    val = BinOp::process(val, i_vinbeg[(stride * currenty) + currentx]);
  }
  o_voutbeg[(stride * i_YCenter) + i_XCenter] = static_cast<T2>(val);
}

template<typename T1, typename T2, class BinOp>
struct t_ErodeDilateDispatcherRuntime<T1, T2, 2, poutre::details::av::array_view, poutre::details::av::array_view, BinOp>
{
  // cppcheck-suppress constParameterReference
  void operator()(const poutre::details::av::array_view<const T1, 2> &i_vin,  const poutre::se::details::neighbor_list_t<2>& nl_runtime, poutre::details::av::array_view<T2, 2> &o_vout)
  {
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    auto   ibd     = i_vin.bound();
    auto   obd     = o_vout.bound();
    auto   istride = i_vin.stride();
    auto   ostride = o_vout.stride();
    scoord ysize   = ibd[0];
    scoord xsize   = ibd[1];

    POUTRE_CHECK(ibd == obd, "bound not compatible");
    POUTRE_CHECK(istride == ostride, "stride not compatible");
    auto [min_extension,max_extension]  = nl_runtime.maximum_extension();
    auto half_size = std::max(max_extension[0], max_extension[1]); //TODO REFINE THIS
    auto      i_vinbeg            = i_vin.data();
    auto      o_voutbeg           = o_vout.data();
    // handling the upper lines
    for( ptrdiff_t y = 0; y < half_size; ++y )
    {
      for( ptrdiff_t x = 0; x < xsize; ++x )
      {
        t_ErodeDilateIterateBorderArrayView2DRuntimeHelper<T1, T2, BinOp>(i_vinbeg, nl_runtime, o_voutbeg, xsize, ysize, istride[0], x, y);
      }
    }
    // handling the lower lines
    for( ptrdiff_t y = ysize - half_size; y < ysize; ++y )
    {
      for( ptrdiff_t x = 0; x < xsize; ++x )
      {
        t_ErodeDilateIterateBorderArrayView2DRuntimeHelper<T1, T2, BinOp>(i_vinbeg, nl_runtime, o_voutbeg, xsize, ysize, istride[0], x, y);
      }
    }
    // Main lines area
    for( ptrdiff_t y = half_size; y < ysize - half_size; ++y )
    {
      // handling the first columns
      for( ptrdiff_t x = 0; x < half_size; ++x )
      {
        t_ErodeDilateIterateBorderArrayView2DRuntimeHelper<T1, T2, BinOp>(i_vinbeg, nl_runtime, o_voutbeg, xsize, ysize, istride[0], x, y);
      }
      for( ptrdiff_t x = half_size; x < xsize - half_size; ++x )
      {
        auto  val = BinOp::neutral;
        poutre::details::av::idx2d center{y, x};

        for( const auto &idxnl : nl_runtime )
        {
          scoord const currentx = center[1] + idxnl[1];
          scoord const currenty = center[0] + idxnl[0];
          val             = BinOp::process(val, i_vinbeg[(xsize * currenty) + currentx]);
        }
        o_voutbeg[(xsize * y) + x] = static_cast<T2>(val);
      }
      // handling the last columns
      for( ptrdiff_t x = xsize - half_size; x < xsize; ++x )
      {
        t_ErodeDilateIterateBorderArrayView2DRuntimeHelper<T1, T2, BinOp>(i_vinbeg, nl_runtime, o_voutbeg, xsize, ysize, istride[0], x, y);
      }
    }
  }
};

template<typename TIn,
           typename TOut,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_Dilate(const ViewIn<const TIn, Rank> &i_vin,
  const poutre::se::details::neighbor_list_t<Rank>& nl, ViewOut<TOut, Rank> &o_vout)
{
  POUTRE_ENTERING("t_Dilate view dispatcher");
  POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
  using BinOp = BinOpSupRuntime<TIn, TOut> ;
  t_ErodeDilateDispatcherRuntime<TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
  dispatcher(i_vin, nl, o_vout);
}

template<typename TIn, typename TOut, ptrdiff_t Rank>
void t_Dilate(const poutre::details::image_t<TIn, Rank> &i_img, const poutre::se::details::neighbor_list_t<Rank>& nl, poutre::details::image_t<TOut, Rank> &o_img)
{
  AssertSizesCompatible(i_img, o_img, "t_Dilate incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "t_Dilate incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "t_Dilate output must be != than input images");
  auto viewIn = view(i_img);
  auto viewOut = view(o_img);
  return t_Dilate(viewIn, nl, viewOut);
}

template<typename TIn,
           typename TOut,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_Erode(const ViewIn<const TIn, Rank> &i_vin,
  const poutre::se::details::neighbor_list_t<Rank>& nl, ViewOut<TOut, Rank> &o_vout)
{
  POUTRE_ENTERING("t_Erode view dispatcher");
  POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
  using BinOp = BinOpInfRuntime<TIn, TOut> ;
  t_ErodeDilateDispatcherRuntime<TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
  dispatcher(i_vin, nl, o_vout);
}

template<typename TIn, typename TOut, ptrdiff_t Rank>
void t_Erode(const poutre::details::image_t<TIn, Rank> &i_img, const poutre::se::details::neighbor_list_t<Rank>& nl, poutre::details::image_t<TOut, Rank> &o_img)
{
  AssertSizesCompatible(i_img, o_img, "t_Erode incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "t_Erode incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "t_Erode output must be != than input images");
  auto viewIn = view(i_img);
  auto viewOut = view(o_img);
  return t_Erode(viewIn, nl, viewOut);
}

//! @} doxygroup: poutre_llm_group
}//poutre::llm::details