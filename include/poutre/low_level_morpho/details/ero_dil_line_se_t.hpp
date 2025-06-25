//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   ero_dil_line_se_t.hpp
 * @author Thomas Retornaz
 * @brief  Erode dilate with line shaped structuring element
 *
 *
 */

#include <algorithm>
#include <numeric>
#include <memory>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/pixel_processing/details/copy_convert_t.hpp>

namespace poutre::llm::details {
/**
 * @addtogroup poutre_llm_group
 *@{
 */

template<typename T> struct BinOpInfLine
{
public:
  static constexpr T               neutral = std::numeric_limits<T>::max();
  static T operator()(const T &A0, const T &A1) { return std::min<T>(A0, A1); }
};

template<typename T> struct BinOpSupLine
{
public:
  static constexpr T               neutral = std::numeric_limits<T>::lowest();
  static T operator()(const T &A0, const T &A1) { return std::max<T>(A0, A1);}
};

// todo bench against Lemonier algorithm
template<class T, class BinOp>
void van_herck_1d(T *__restrict f,
                  T *__restrict g,
                  T *__restrict h,
                  ptrdiff_t      size_line,
                  ptrdiff_t      size_line_segment,
                  BinOp op)
{
  if( size_line == 0 )
    return;

  const ptrdiff_t alpha = 2 * size_line_segment + 1;
  const ptrdiff_t size  = size_line + 2 * size_line_segment;

  // Forward pass
  {
    ptrdiff_t chunk_start = -size_line_segment;
    ptrdiff_t rem         = size;

    for( ; rem > 0; chunk_start += alpha, rem -= alpha )
    {
      ptrdiff_t chunk_size = std::min(rem, alpha);
      std::partial_sum(f + chunk_start, f + chunk_start + chunk_size, g + chunk_start, op);
    }
  }

  // Backward pass
  {
    ptrdiff_t chunk_start = -size_line_segment;
    ptrdiff_t rem         = size;

    for( ; rem > 0; chunk_start += alpha, rem -= alpha )
    {
      ptrdiff_t chunk_size = std::min(alpha, rem);
      std::partial_sum(std::make_reverse_iterator(f + chunk_start + chunk_size),
                       std::make_reverse_iterator(f + chunk_start),
                       std::make_reverse_iterator(h + chunk_start + chunk_size),
                       op);
    }
  }
  {
    for( ptrdiff_t i = 0; i < size_line; ++i ) f[i] = op(h[i - size_line_segment], g[i + size_line_segment]);
  }
}

template<typename T1,
         typename T2,
         ptrdiff_t Rank,
         template<typename, ptrdiff_t>
         class View1,
         template<typename, ptrdiff_t>
         class View2,
         class BinOp>
struct t_ErodeDilateXOpLineDispatcher
{
  // generic case not supported yet
  static_assert(false, "To be implemented for generic views");
};

template<typename T,  class BinOp> struct t_ErodeDilateXOpLineDispatcher<T, T, 1, poutre::details::av::array_view, poutre::details::av::array_view, BinOp>
{
  void operator()(const poutre::details::av::array_view<const T, 1> &i_vin, ptrdiff_t size_line_segment, const poutre::details::av::array_view<T, 1> &o_vout) const
  {
    BinOp op;
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    auto   ibd = i_vin.bound();
    auto   obd = o_vout.bound();
    POUTRE_CHECK(ibd == obd, "bound not compatible");
    scoord size_line   = ibd[0];
    if( size_line == 0 )
      return;
    if( size_line_segment <= 0 )
    {
      std::memcpy(reinterpret_cast<void *>(o_vout.data()), reinterpret_cast<const void *>(i_vin.data()), sizeof(T) * i_vin.size());
      return;
    }
    const T * rawLineIn  = i_vin.data();
    T *      rawLineOut  = o_vout.data();

    // need auxiliary buffer
    using tmpBuffer = std::vector<T>;
    const size_t padded_size = static_cast<size_t>(size_line + 2 * size_line_segment);
    tmpBuffer f(padded_size), g(padded_size), h(padded_size);
    std::fill_n(f.begin(), f.size(), BinOp::neutral);
    std::memcpy(f.data() + size_line_segment, rawLineIn, static_cast<size_t>(size_line) * sizeof(T));
    van_herck_1d(f.data() + size_line_segment, g.data() + size_line_segment, h.data() + size_line_segment, size_line, size_line_segment, op);
    std::memcpy(rawLineOut, f.data() + size_line_segment, static_cast<size_t>(size_line) * sizeof(T));
  }
};

template<typename T, class BinOp> struct t_ErodeDilateXOpLineDispatcher<T, T, 2, poutre::details::av::array_view, poutre::details::av::array_view, BinOp>
{
  void operator()(const poutre::details::av::array_view<const T, 2> &i_vin,  ptrdiff_t size_line_segment, poutre::details::av::array_view<T, 2> &o_vout) const
  {
    BinOp op;
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    auto   ibd     = i_vin.bound();
    auto   obd     = o_vout.bound();
    auto   istride = i_vin.stride();
    auto   ostride = o_vout.stride();
    scoord ysize   = ibd[0];
    scoord xsize   = ibd[1];
    POUTRE_CHECK(ibd == obd, "bound not compatible");
    POUTRE_CHECK(istride == ostride, "stride not compatible");
    if( xsize == 0 || ysize == 0 )
      return;

    if( size_line_segment <= 0 )
    {
      poutre::details::t_Copy(i_vin, o_vout);
      return;
    }
    using lineView = T *;
    using const_lineView = const T *;
    ptrdiff_t size_line    = xsize;
    // need auxiliary buffer
    using tmpBuffer = std::vector<T>;
    const size_t padded_size = static_cast<size_t>(size_line + 2 * size_line_segment);
    tmpBuffer f(padded_size), g(padded_size), h(padded_size);

    for( scoord y = 0; y < ysize; y++ )
    {
      const_lineView bufInputCurrentLine = i_vin.data() + y * xsize;
      lineView bufOutputCurrentLine = o_vout.data() + y * xsize;
      std::fill_n(f.begin(), f.size(), BinOp::neutral);
      std::memcpy(f.data() + size_line_segment, bufInputCurrentLine, static_cast<size_t>(size_line) * sizeof(T));
      van_herck_1d(f.data() + size_line_segment, g.data() + size_line_segment, h.data() + size_line_segment, size_line, size_line_segment, op);
      std::memcpy(bufOutputCurrentLine, f.data() + size_line_segment, static_cast<size_t>(size_line) * sizeof(T));
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
void t_ErodeX(const ViewIn<const TIn, Rank> &i_vin, ptrdiff_t size_segment, ViewOut<TOut, Rank> &o_vout)
{
  POUTRE_ENTERING("t_ErodeX view dispatcher");
  POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
  using BinOp = BinOpInfLine<TIn> ;
  t_ErodeDilateXOpLineDispatcher<TIn, TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
  dispatcher(i_vin, size_segment, o_vout);
}

template<typename TIn, typename TOut, ptrdiff_t Rank>
void t_ErodeX(const poutre::details::image_t<TIn, Rank> &i_img, ptrdiff_t size_segment, poutre::details::image_t<TOut, Rank> &o_img)
{
  POUTRE_ENTERING("t_ErodeX");
  AssertSizesCompatible(i_img, o_img, "t_ErodeX incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "t_ErodeX incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "t_ErodeX output must be != than input images");
  auto viewIn = view(i_img);
  auto viewOut = view(o_img);
  return t_ErodeX(viewIn, size_segment, viewOut);
}

template<typename TIn,
         typename TOut,
         ptrdiff_t Rank,
         template<typename, ptrdiff_t>
         class ViewIn,
         template<typename, ptrdiff_t>
         class ViewOut>
void t_DilateX(const ViewIn<const TIn, Rank> &i_vin, ptrdiff_t size_segment, ViewOut<TOut, Rank> &o_vout)
{
  POUTRE_ENTERING("t_DilateX view dispatcher");
  POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
  using BinOp = BinOpSupLine<TIn> ;
  t_ErodeDilateXOpLineDispatcher<TIn, TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
  dispatcher(i_vin, size_segment, o_vout);
}

template<typename TIn, typename TOut, ptrdiff_t Rank>
void t_DilateX(const poutre::details::image_t<TIn, Rank> &i_img, ptrdiff_t size_segment, poutre::details::image_t<TOut, Rank> &o_img)
{
  POUTRE_ENTERING("t_DilateX");
  AssertSizesCompatible(i_img, o_img, "t_Dilate incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "t_Dilate incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "t_Dilate output must be != than input images");
  auto viewIn = view(i_img);
  auto viewOut = view(o_img);
  return t_DilateX(viewIn, size_segment, viewOut);
}
//! @} doxygroup: poutre_llm_group
}//poutre::llm::details