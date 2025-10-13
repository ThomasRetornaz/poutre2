//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file unary_op_t.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief Generic unary operations over images or views
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <algorithm>//transform
#include <iostream>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/details/simd/simd_algorithm.hpp>//simd transform

namespace poutre::details {

/**
 * @addtogroup image_processing_linear_group
 *@{
 */

/****************************************************************************************/
/*                               PixelWiseUnaryOp                                        */
/****************************************************************************************/
// primary use strided view
template<typename TIn,
  typename TOut,
  ptrdiff_t Rank,
  template<typename, ptrdiff_t>
  class ViewIn,
  template<typename, ptrdiff_t>
  class ViewOut,
  // template<typename, typename>
  class UnOp,
  typename = void>
struct PixelWiseUnaryOpDispatcher
{
  static_assert((std::is_same_v<ViewIn<TIn, Rank>, av::strided_array_view<TIn, Rank>>
                  || std::is_same_v<ViewIn<TIn, Rank>, av::strided_array_view<const TIn, Rank>>
                  || std::is_same_v<ViewOut<TOut, Rank>, av::strided_array_view<TOut, Rank>>),
    "strided view only specialization fail for array view");

  void operator()(const ViewIn<TIn, Rank> &i_vin, UnOp op, const ViewOut<TOut, Rank> &o_vout) const
  {
    POUTRE_ENTERING("PixelWiseUnaryOpDispatcher generic case");
    // std::cout << "\n" << "call PixelWiseUnaryOpDispatcher strided view";
    // More runtime dispatch
    auto vInbound = i_vin.bound();
    auto vOutbound = o_vout.bound();
    auto stridevIN = i_vin.stride();
    auto stridevOut = o_vout.stride();

    if (vInbound == vOutbound && stridevIN == stridevOut)// same bound + same stride -> one idx
    {
      // std::cout << "\n" << "call UnaryOpDispatcher one idx";
      auto beg1 = begin(vInbound);
      auto end1 = end(vInbound);
      for (; beg1 != end1; ++beg1) { o_vout[*beg1] = static_cast<TOut>(op(i_vin[*beg1])); }
      // TODO stride==1 in least significant dimension
    } else// default two idx
    {
      // std::cout << "\n" << "call UnaryOpDispatcher primary template default two idx";
      auto beg1 = begin(vInbound);
      auto end1 = end(vInbound);
      auto beg2 = begin(vOutbound);
      for (; beg1 != end1; ++beg1, ++beg2) { o_vout[*beg2] = static_cast<TOut>(op(i_vin[*beg1])); }
    }
  }
};

// template specialization both array_view != type
template<typename TIn,
  typename TOut,
  ptrdiff_t Rank,
  /* template<typename, typename>*/
  class UnOp>
struct PixelWiseUnaryOpDispatcher<TIn,
  TOut,
  Rank,
  av::array_view,
  av::array_view,
  UnOp,
  std::enable_if_t<!std::is_same_v<std::remove_const_t<TIn>, std::remove_const_t<TOut>>>>
{
  void operator()(const av::array_view<TIn, Rank> &i_vin, UnOp op, const av::array_view<TOut, Rank> &o_vout) const
  {
    POUTRE_ENTERING("PixelWiseUnaryOpDispatcher both array view != types");
    auto i_vinbeg = i_vin.data();
    auto i_vinend = i_vin.data() + i_vin.size();
    auto o_voutbeg = o_vout.data();
    std::transform(i_vinbeg, i_vinend, o_voutbeg, op);
  }
};

template<typename TIn, typename TOut, ptrdiff_t Rank, /*template<typename, typename> */ class UnOp>
struct PixelWiseUnaryOpDispatcher<TIn,
  TOut,
  Rank,
  av::array_view,
  av::array_view,
  UnOp,
  std::enable_if_t<std::is_same_v<std::remove_const_t<TIn>, std::remove_const_t<TOut>> && std::is_arithmetic_v<TIn>>>

{
  void operator()(const av::array_view<TIn, Rank> &i_vin, UnOp op, const av::array_view<TOut, Rank> &o_vout) const
  {
    POUTRE_ENTERING("PixelWiseUnaryOpDispatcher both array view same types");
    // specialization same type,fall back ptr + SIMD";

    auto i_vinbeg = i_vin.data();
    auto i_vinend = i_vin.data() + i_vin.size();
    auto i_voutbeg = o_vout.data();

    const auto length    = i_vin.size();
    if( !length ) {
      return;
    }
    simd::transform(i_vinbeg, i_vinend, i_voutbeg, op);
    // work but speedup is only 20% for 1024 images and degrade for smaller size
    /* const auto block_size = 2048 * poutre::TypeTraits<TIn>::simd_loop_step;
    const auto num_blocks = (length + block_size - 1) / block_size;
    if (num_blocks<=1) {
      simd::transform(i_vinbeg, i_vinend, i_voutbeg, op);
      return;
    }
    std::vector<poutre::thread::TaskFuture<TOut*>> futures;
    auto pool = poutre::thread::TreadPool(num_blocks);

    TIn * block_start    = i_vinbeg;
    TOut *block_startout = i_voutbeg;

    TIn *block_end = nullptr;
    TOut *block_out_end = nullptr;

    for( size_t i = 0; i < (num_blocks - 1); ++i )
    {
      block_end = block_start;
      block_out_end = block_startout;
      std::advance(block_end, block_size);
      std::advance(block_out_end, block_size);
      futures.push_back(pool.submit([=] {return simd::transform(block_start, block_end, block_startout, op); }));
      block_start = block_end;
      block_startout = block_out_end;
      //std::advance(block_startout, block_size);
    }
    simd::transform(block_start, i_vinend, block_startout, op);// epilogue last part
    for( auto &f : futures )
    {
      f.get();
    }
    */
  }
};

template<typename TIn,
  typename TOut,
  ptrdiff_t Rank,
  template<typename, ptrdiff_t>
  class ViewIn,
  template<typename, ptrdiff_t>
  class ViewOut,
  class UnOp>
void t_unary_op(const ViewIn<TIn, Rank> &i_vin, UnOp op, const ViewOut<TOut, Rank> &o_vout) POUTRE_NOEXCEPTONLYNDEBUG
{
  POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
  PixelWiseUnaryOpDispatcher<TIn, TOut, Rank, ViewIn, ViewOut, UnOp> dispatcher;
  dispatcher(i_vin, op, o_vout);
}

//! @} doxygroup: image_processing_linear_group

}// namespace poutre::details