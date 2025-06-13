//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   ero_dil_static_se_t.hpp
 * @author Thomas Retornaz
 * @brief  Erode dilate with static SE
 *
 *
 */

#include <algorithm>
#include <memory>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/structuring_element/details/neighbor_list_static_se_t.hpp>
#include <poutre/base/trace.hpp>

namespace poutre::llm::details {
/**
 * @addtogroup poutre_llm_group
 *@{
 */

template<typename T1, typename T2> struct BinOpInf
{
public:
  static constexpr T1                neutral = std::numeric_limits<T1>::max();
  static T2 process(const T1 &A0, const T1 &A1) { return static_cast<T2>(std::min<T1>(A0, A1)); }
};

template<typename T1, typename T2> struct BinOpSup
{
public:
  static constexpr T1                neutral = std::numeric_limits<T1>::lowest();
  static T2 process(const T1 &A0, const T1 &A1) { return static_cast<T2>(std::max<T1>(A0, A1)); }
};

template<typename T>
void t_LineBufferShiftRight(const T *i_viewlinein, scoord lenghtline, scoord nbshift, T paddValue, T *o_viewlineout)
{
  scoord   i;
  const T *linein  = i_viewlinein;
  T *      lineout = o_viewlineout;

  // Case handling error
  if( nbshift >= lenghtline )
  {
    // then pad right border
    for( i = 0; i < lenghtline; i++ )
    {
      lineout[i] = paddValue;
    }
    return;
  }

  // pad left border before shift
  for( i = 0; i < nbshift; i++ )
  {
    lineout[i] = paddValue;
  }
  // shift to right
  std::memcpy(lineout + nbshift,
              linein,
              (lenghtline - nbshift) * sizeof(T)); //<---faster than explicit call to simd::transform
}

template<typename T>
void t_LineBufferShiftLeft(const T *i_viewlinein, scoord lenghtline, scoord nbshift, T paddValue, T *o_viewlineout)
{
  scoord   i;
  const T *linein  = i_viewlinein;
  T *      lineout = o_viewlineout;
  // Case handling error
  if( nbshift >= lenghtline )
  {
    // then pad right border
    for( i = 0; i < lenghtline; i++ )
    {
      lineout[i] = paddValue;
    }
    return;
  }

  // shift to left
  std::memcpy(lineout, linein + nbshift, (lenghtline - nbshift) * sizeof(T));

  // then pad right border
  for( i = lenghtline - nbshift; i < lenghtline; i++ )
  {
    lineout[i] = paddValue;
  }
}

  //-----STRIDED VIEW OR Image_t generic with static SE
  template<poutre::se::Common_NL_SE nl_static,
           typename T1,
           typename T2,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2,
           class BinOp>
  struct t_ErodeDilateDispatcher
  {
    static_assert(Rank == poutre::se::details::static_se_traits<nl_static>::rank, "SE and view have not the same Rank");
    void operator()(const View1<const T1, Rank> &i_vin, const View2<T2, Rank> &o_vout) const
    {
      POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");

      // More runtime dispatch
      auto vInbound   = i_vin.bound();
      auto vOutbound  = o_vout.bound();
      auto stridevIN  = i_vin.stride();
      auto stridevOut = o_vout.stride();
      constexpr auto nl_coord     = poutre::se::details::static_se_traits<nl_static>::coordinates;
      if( vInbound == vOutbound && stridevIN == stridevOut ) // same bound + same stride -> one idx
      {
        auto beg1 = begin(vInbound);
        auto end1 = end(vInbound);
        for( ; beg1 != end1; ++beg1 )
        {
          auto val = BinOp::neutral;
          for( const auto &idxnl : nl_coord )
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
          for( const auto &idxnl : nl_coord )
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
template<poutre::se::Common_NL_SE nl_static, typename T1, typename T2, class BinOp>
void t_ErodeDilateIterateBorderArrayView2DHelper(const T1 *&i_vin,
                                                 T2 *&      o_vout,
                                                 scoord     xsize, //NOLINT
                                                 scoord     ysize, //NOLINT
                                                 scoord     stride,
                                                 ptrdiff_t  i_XCenter,
                                                 ptrdiff_t  i_YCenter)
{
  constexpr auto NlList = poutre::se::details::static_se_traits<nl_static>::coordinates;
  const ptrdiff_t NbNeighbor = poutre::se::details::static_se_traits<nl_static>::size;
  auto      i_vinbeg   = i_vin;
  auto      o_voutbeg  = o_vout;
  auto      val        = BinOp::neutral;
  poutre::details::av::idx2d     center{i_YCenter, i_XCenter};
  for( ptrdiff_t idxNB = 0; idxNB < NbNeighbor; ++idxNB )
  {
    auto  idxnl(NlList[static_cast<std::size_t>(idxNB)]);
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

  template<poutre::se::Common_NL_SE nl_static, typename T1, typename T2, class BinOp>
  struct t_ErodeDilateDispatcher<nl_static, T1, T2, 2, poutre::details::av::array_view, poutre::details::av::array_view, BinOp>
  {
    static_assert(2 == poutre::se::details::static_se_traits<nl_static>::rank, "SE and view have not the same Rank==2");
    // cppcheck-suppress constParameterReference
    void operator()(const poutre::details::av::array_view<const T1, 2> &i_vin, poutre::details::av::array_view<T2, 2> &o_vout)
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
      constexpr auto nl_coord     = poutre::se::details::static_se_traits<nl_static>::coordinates;
      constexpr auto nl_size      = poutre::se::details::static_se_traits<nl_static>::size;
      ptrdiff_t half_size = poutre::se::details::static_se_traits<nl_static>::maximum_extension();
      auto      i_vinbeg            = i_vin.data();
      auto      o_voutbeg           = o_vout.data();
      // handling the upper lines
      for( ptrdiff_t y = 0; y < half_size; ++y )
      {
        for( ptrdiff_t x = 0; x < xsize; ++x )
        {
          t_ErodeDilateIterateBorderArrayView2DHelper<nl_static, T1, T2, BinOp>(i_vinbeg, o_voutbeg, xsize, ysize, istride[0], x, y);
        }
      }
      // handling the lower lines
      for( ptrdiff_t y = ysize - half_size; y < ysize; ++y )
      {
        for( ptrdiff_t x = 0; x < xsize; ++x )
        {
          t_ErodeDilateIterateBorderArrayView2DHelper<nl_static, T1, T2, BinOp>(i_vinbeg, o_voutbeg, xsize, ysize, istride[0], x, y);
        }
      }
      // Main lines area
      for( ptrdiff_t y = half_size; y < ysize - half_size; ++y )
      {
        // handling the first columns
        for( ptrdiff_t x = 0; x < half_size; ++x )
        {
          t_ErodeDilateIterateBorderArrayView2DHelper<nl_static, T1, T2, BinOp>(i_vinbeg, o_voutbeg, xsize, ysize, istride[0], x, y);
        }
        for( ptrdiff_t x = half_size; x < xsize - half_size; ++x )
        {
          auto  val = BinOp::neutral;
          poutre::details::av::idx2d center{y, x};
          // ver slow
          // for (const auto& idxnl : NlList)
          //{
          //    auto currentidx = center+ idxnl;
          //    max = std::max<T1>(max, i_vinbeg[xsize*currentidx[0]+
          //    currentidx[1]]);
          //}
          for( ptrdiff_t idxNB = 0; idxNB < nl_size; ++idxNB )
          {
            poutre::details::av::idx2d  idxnl(nl_coord[static_cast<std::size_t>(idxNB)]);
            scoord const currentx = center[1] + idxnl[1];
            scoord const currenty = center[0] + idxnl[0];
            val             = BinOp::process(val, i_vinbeg[(xsize * currenty) + currentx]);
          }
          o_voutbeg[(xsize * y) + x] = static_cast<T2>(val);
        }
        // handling the last columns
        for( ptrdiff_t x = xsize - half_size; x < xsize; ++x )
        {
          t_ErodeDilateIterateBorderArrayView2DHelper<nl_static, T1, T2, BinOp>(i_vinbeg, o_voutbeg, xsize, ysize, istride[0], x, y);
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
    const poutre::se::Common_NL_SE nl_static, ViewOut<TOut, Rank> &o_vout)
{
  POUTRE_ENTERING("t_Dilate view dispatcher");
  POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
  using BinOp = BinOpSup<TIn, TOut> ;

  if constexpr (Rank == 1) {
    switch(nl_static) {
      case poutre::se::Common_NL_SE::SESegmentX1D: {
        t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentX1D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
        dispatcher(i_vin, o_vout);
      } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_Dilate unsupported nl_static");}
    }
    return;
  }
  if constexpr (Rank == 2) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SESegmentX2D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentX2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentY2D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentY2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare2D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SECross2D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_Dilate unsupported nl_static");}
    }
    return;
  }
  if constexpr (Rank == 3) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SECross3D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare3D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentX3D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentX3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentY3D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentY3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentZ3D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentZ3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_Dilate unsupported nl_static");}
    }
    return;
  }
    POUTRE_RUNTIME_ERROR("t_Dilate unsupported nl_static");
  }

  template<typename TIn,
           typename TOut,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_Erode(const ViewIn<const TIn, Rank> &i_vin,
  const poutre::se::Common_NL_SE nl_static, ViewOut<TOut, Rank> &o_vout)
  {
    POUTRE_ENTERING("t_Erode view dispatcher");
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
  using BinOp = BinOpInf<TIn, TOut> ;
  if constexpr (Rank == 1) {
    switch(nl_static) {
      case poutre::se::Common_NL_SE::SESegmentX1D: {
        t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentX1D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
        dispatcher(i_vin, o_vout);
      } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_Erode unsupported nl_static");}
    }
    return;
  }
  if constexpr (Rank == 2) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SESegmentX2D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentX2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentY2D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentY2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare2D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SECross2D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_Erode unsupported nl_static");}
    }
    return;
  }
  if constexpr (Rank == 3) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SECross3D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare3D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentX3D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentX3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentY3D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentY3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentZ3D: {
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentZ3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_Erode unsupported nl_static");}
    }
    return;
  }
    POUTRE_RUNTIME_ERROR("t_Erode unsupported nl_static");
  }

template<typename T1, typename T2, ptrdiff_t Rank>
void t_Erode(const poutre::details::image_t<T1, Rank> &i_img, poutre::se::Common_NL_SE nl_static, poutre::details::image_t<T2, Rank> &o_img)
{
  AssertSizesCompatible(i_img, o_img, "t_Erode incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "t_Erode incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "t_Erode output must be != than input images");
  auto viewIn = view(i_img);
  auto viewOut = view(o_img);
  return t_Erode(viewIn, nl_static, viewOut);
}

template<typename TIn, typename TOut, ptrdiff_t Rank>
void t_Dilate(const poutre::details::image_t<TIn, Rank> &i_img, poutre::se::Common_NL_SE nl_static, poutre::details::image_t<TOut, Rank> &o_img)
{
  AssertSizesCompatible(i_img, o_img, "t_Dilate incompatible size");
  AssertAsTypesCompatible(i_img, o_img, "t_Dilate incompatible types");
  AssertImagesAreDifferent(i_img, o_img, "t_Dilate output must be != than input images");
  auto viewIn = view(i_img);
  auto viewOut = view(o_img);
  return t_Dilate(viewIn, nl_static, viewOut);
}

//! @} doxygroup: poutre_llm_group
}//poutre::llm::details