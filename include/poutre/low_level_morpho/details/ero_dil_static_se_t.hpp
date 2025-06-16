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
#include <poutre/base/details/simd/simd_algorithm.hpp>
#include <poutre/pixel_processing/details/arith_op_t.hpp>
#include <poutre/pixel_processing/details/copy_convert_t.hpp>
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
              static_cast<std::size_t>(lenghtline - nbshift) * sizeof(T)); //<---faster than explicit call to simd::transform
}


// helper to extract a slice of image 3d (sliceNumber) and put the result in 2D image
// TODO use view instead ?
template<typename T>
   void t_3DCopyOneSliceTo2D(T *imIn3d,
                                      const std::size_t xSize, const std::size_t ySize,
                                      const std::size_t wxSize, const std::size_t wySize,
                                      const std::size_t sliceNumber,
                                      T *imOut2d)
{

  T *ptrIn = imIn3d + sliceNumber*(xSize*ySize);

  for(std::size_t i = 0 ; i<wySize ; i++) {
    memcpy(imOut2d, ptrIn, wxSize*sizeof(T));
    ptrIn += xSize;
    imOut2d += wxSize;
  }
}

// helper to put a 2D image at slice (sliceNumber) in 3D images
// TODO use view instead ?
template<typename T>
    void t_3DCopyOneSliceFrom2D(T *imIn2d,
                                         const std::size_t xSize, const std::size_t ySize,
                                         const std::size_t wxSize, const std::size_t wySize,
                                         const std::size_t sliceNumber,
                                         T *imOut3d) {

  T *ptrOut = imOut3d + sliceNumber*(xSize*ySize);

  for(std::size_t i = 0 ; i<wySize ; i++) {
    memcpy(ptrOut, imIn2d, wxSize*sizeof(T));
    ptrOut += xSize;
    imIn2d += wxSize;
  }
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
  std::memcpy(lineout, linein + nbshift, static_cast<std::size_t>(lenghtline - nbshift) * sizeof(T));

  // then pad right border
  for( i = lenghtline - nbshift; i < lenghtline; i++ )
  {
    lineout[i] = paddValue;
  }
}

 // Helper structure Shift buffer left then right and apply arithmetic operator
  // Dilate
  template<typename T> struct LineBufferShiftAndArithDilateHelperOp
  {
    public:
    using linePtr       = T *;
    using const_linePtr = const T *;
    using viewIn        = poutre::details::av::array_view<const T, 1>;
    using viewOut       = poutre::details::av::array_view<T, 1>;

    static constexpr T m_paddingValue = poutre::TypeTraits<T>::lowest();
    static constexpr T neutral        = m_paddingValue;
    using self                        = LineBufferShiftAndArithDilateHelperOp<T>;
    static T process(const T &A0, const T &A1) { return std::max<T>(A0, A1); }

    static void ApplyArith(const_linePtr lineIn1, const_linePtr lineIn2, scoord size, linePtr lineout)
    {
      simd::transform(lineIn1, lineIn1 + size, lineIn2, lineout, poutre::details::op_Sup<T, T, T>());
    }
    static void ShiftRightLeftAndArith(const_linePtr linein,
                                       scoord        size,
                                       scoord        nbStepShiftLeft,
                                       scoord        nbStepShiftRight,
                                       linePtr       linetmp, // to reuse external allocation
                                       linePtr       lineout)
    {
      // shift to left and arith
      t_LineBufferShiftLeft<T>(linein, size, nbStepShiftLeft, m_paddingValue, linetmp);
      self::ApplyArith(linein, linetmp, size, lineout);

      // shift to right and arith with previous computed arith
      t_LineBufferShiftRight<T>(linein, size, nbStepShiftRight, m_paddingValue, linetmp);
      self::ApplyArith(lineout, linetmp, size, lineout);
    }
  };

  // Helper structure Shift buffer left then right and apply arithmetic operator
  // Dilate
  template<typename T> struct LineBufferShiftAndArithErodeHelperOp
  {
    public:
    using linePtr       = T *;
    using const_linePtr = const T *;
    using viewIn        = const poutre::details::av::array_view<const T, 1>;
    using viewOut       = poutre::details::av::array_view<T, 1>;

    static constexpr T m_paddingValue = poutre::TypeTraits<T>::sup();
    static constexpr T neutral        = m_paddingValue;
    using self                        = LineBufferShiftAndArithErodeHelperOp<T>;
    static T process(const T &A0, const T &A1) { return std::min<T>(A0, A1); }

    static void ApplyArith(const_linePtr lineIn1, const_linePtr lineIn2, scoord size, linePtr lineout)
    {
      simd::transform(lineIn1, lineIn1 + size, lineIn2, lineout, poutre::details::op_Inf<T, T, T>());
    }
    static void ShiftRightLeftAndArith(const_linePtr linein,
                                       scoord        size,
                                       scoord        nbStepShiftLeft,
                                       scoord        nbStepShiftRight,
                                       linePtr       linetmp, // to reuse external allocation
                                       linePtr       lineout)
    {
      // shift to left and arith
      t_LineBufferShiftLeft<T>(linein, size, nbStepShiftLeft, m_paddingValue, linetmp);
      self::ApplyArith(linein, linetmp, size, lineout);

      // shift to right and arith with previous computed arith
      t_LineBufferShiftRight<T>(linein, size, nbStepShiftRight, m_paddingValue, linetmp);
      self::ApplyArith(lineout, linetmp, size, lineout);
    }
  };

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

  template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateDispatcher<se::Common_NL_SE::SESquare2D, TIn, TOut, 2, poutre::details::av::array_view, poutre::details::av::array_view, HelperOp>
  {
    void operator()(const poutre::details::av::array_view<const TIn, 2> &i_vin, const poutre::details::av::array_view<TOut, 2> &o_vout) const
    {
      POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
      auto   ibd     = i_vin.bound();
      auto   obd     = o_vout.bound(); // NOLINT
      auto   istride = i_vin.stride();
      auto   ostride = o_vout.stride();
      scoord ysize   = ibd[0];
      scoord xsize   = ibd[1];
      POUTRE_CHECK(ibd == obd, "bound not compatible");
      POUTRE_CHECK(istride == ostride, "stride not compatible");
      using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
      using lineView  = TIn *__restrict; // array_view<TIn, 1>;

      tmpBuffer tempLine(static_cast<std::size_t>(xsize)), tempLine1(static_cast<std::size_t>(xsize)), tempLine2(static_cast<std::size_t>(xsize)), tempLine3(static_cast<std::size_t>(xsize));
      lineView  bufTempLine(tempLine.data()), bufTempLine1(tempLine1.data()), bufTempLine2(tempLine2.data()),
          bufTempLine3(tempLine3.data());
      lineView bufInputPreviousLine=nullptr;
      // lineView bufInputCurrentLine=nullptr; // NOLINT used here after
      lineView bufInputNextLine=nullptr;
      lineView bufOuputCurrentLine=nullptr;

      // quick run one line
      if( ysize == 1 )
      {
        bufInputPreviousLine = const_cast<TIn *>(i_vin.data());
        bufOuputCurrentLine  = o_vout.data();
        // dilate/erode line 0
        HelperOp::ShiftRightLeftAndArith(bufInputPreviousLine, xsize, 1, 1, bufTempLine, bufOuputCurrentLine);
        return;
      }

      // compute first line
      // translate to clipped connection
      // x . x
      // x x x
      bufInputPreviousLine = const_cast<TIn *>(i_vin.data());
      // dilate/erode line 0
      HelperOp::ShiftRightLeftAndArith(bufInputPreviousLine, xsize, 1, 1, bufTempLine, bufTempLine1);

      bufInputNextLine    = const_cast<TIn *>(i_vin.data()) + xsize;
      bufOuputCurrentLine = o_vout.data();
      // dilate/erode line 1
      HelperOp::ShiftRightLeftAndArith(bufInputNextLine, xsize, 1, 1, bufTempLine, bufTempLine2);

      // sup(dilate line 1,dilate line 0) or inf(erode line 1,erode line 0)
      HelperOp::ApplyArith(bufTempLine1, bufTempLine2, xsize, bufOuputCurrentLine);

      // bufInputCurrentLine = bufInputNextLine;
      // then loop
      for( scoord y = 2; y < ysize; y++ )
      {
        // Invariant of the loop
        // bufTempLine1 contains dilation/erosion of previous line Y-2
        // bufTempLine2 contains dilation/erosion of current line Y-1
        // bufTempLine3 contains dilation/erosion of next line ie Y

        // actual computation
        // 1 2 3   <--- bufInputPreviousLine y-2
        // 4 5 6   <--- bufInputCurrentLine  y-1
        // 7 8 9   <--- bufInputNextLine     y

        bufOuputCurrentLine = o_vout.data() + (y - 1) * xsize;
        bufInputNextLine    = const_cast<TIn *>(i_vin.data()) + (y)*xsize;
        // dilate/erode(y)
        HelperOp::ShiftRightLeftAndArith(bufInputNextLine, xsize, 1, 1, bufTempLine, bufTempLine3);

        // sup(dilate(y-2),dilate(y-1)) or inf(erode(y-2),erode(y-1))
        HelperOp::ApplyArith(bufTempLine1, bufTempLine2, xsize, bufTempLine);
        // sup(dilate(y-2),dilate(y-1),dilate(y)) or
        // inf(erode(y-2),erode(y-1),erode(y))
        HelperOp::ApplyArith(bufTempLine, bufTempLine3, xsize, bufOuputCurrentLine);

        // so use swap to don't loose ref on bufTempLine3
        std::swap(bufTempLine3, bufTempLine2);
        std::swap(bufTempLine3, bufTempLine1);
      }
      // end last line
      // translate to clipped connection
      // x x x
      // x . x
      bufOuputCurrentLine = o_vout.data() + xsize * (ysize - 1);
      HelperOp::ApplyArith(bufTempLine1, bufTempLine2, xsize, bufOuputCurrentLine);
    }
  };

template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateDispatcher<se::Common_NL_SE::SECross2D, TIn, TOut, 2, poutre::details::av::array_view, poutre::details::av::array_view, HelperOp>
  {
    void operator()(const poutre::details::av::array_view<const TIn, 2> &i_vin, const poutre::details::av::array_view<TOut, 2> &o_vout) const
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
      using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
      // using lineView = array_view<TIn, 1>;
      using lineView = TIn *__restrict; // array_view<TIn, 1>;

      tmpBuffer tempLine(static_cast<std::size_t>(xsize)), tempLine2(static_cast<std::size_t>(xsize));
      lineView  bufTempLine(tempLine.data()), bufTempLine2(tempLine2.data());
      lineView bufInputPreviousLine = nullptr;
      lineView bufInputCurrentLine= nullptr;
      lineView bufInputNextLine= nullptr;
      lineView bufOuputCurrentLine= nullptr;

      // quick run one line
      if( ysize == 1 )
      {
        bufInputPreviousLine = const_cast<TIn *>(i_vin.data());
        bufOuputCurrentLine  = o_vout.data();
        // dilate/erode line 0
        HelperOp::ShiftRightLeftAndArith(bufInputPreviousLine, xsize, 1, 1, bufTempLine, bufOuputCurrentLine);
        return;
      }

      // compute first line
      // translate to clipped connection
      // x . x
      // ? x ?
      bufInputPreviousLine = const_cast<TIn *>(i_vin.data());
      // dilate/erode line 0
      HelperOp::ShiftRightLeftAndArith(bufInputPreviousLine, xsize, 1, 1, bufTempLine, bufTempLine2);

      bufInputNextLine    = const_cast<TIn *>(i_vin.data()) + xsize;
      bufOuputCurrentLine = o_vout.data();

      // inf/sup between dilate line 0 and line 1
      HelperOp::ApplyArith(bufTempLine2, bufInputNextLine, xsize, bufOuputCurrentLine);

      // then loop
      bufInputCurrentLine = bufInputNextLine;
      for( scoord y = 2; y < ysize; y++ )
      {
        // actual computation
        // 1 2 3   <--- bufInputPreviousLine y-2
        // 4 5 6   <--- bufInputCurrentLine  y-1
        // 7 8 9   <--- bufInputNextLine     y
        bufOuputCurrentLine = o_vout.data() + (y - 1) * xsize;
        bufInputNextLine    = const_cast<TIn *>(i_vin.data()) + (y)*xsize;
        // dilate(y-1)/erode(y-1)
        HelperOp::ShiftRightLeftAndArith(bufInputCurrentLine, xsize, 1, 1, bufTempLine, bufTempLine2);

        // sup(y-2,dilate(y-1)),inf(y-2,erode(y-1))
        HelperOp::ApplyArith(bufInputPreviousLine, bufTempLine2, xsize, bufTempLine);
        // sup(sup(y-2,dilate(y-1),y) || inf(inf(y-2,erode(y-1),y)
        HelperOp::ApplyArith(bufTempLine, bufInputNextLine, xsize, bufOuputCurrentLine);
        // swap
        bufInputPreviousLine = bufInputCurrentLine; // previous <--current
        bufInputCurrentLine  = bufInputNextLine;    // current <-- next
      }
      // end last line
      // translate to clipped connection
      //. x .
      // x x x
      // note that bufInputCurrentLine already point on ySize-1 (see above)
      bufOuputCurrentLine = o_vout.data() + xsize * (ysize - 1);
      // dilate/erode line y-1
      HelperOp::ShiftRightLeftAndArith(bufInputCurrentLine, xsize, 1, 1, bufTempLine, bufTempLine2);
      HelperOp::ApplyArith(bufInputPreviousLine, bufTempLine2, xsize, bufOuputCurrentLine);
    }
  };

  template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateDispatcher<se::Common_NL_SE::SESegmentX2D, TIn, TOut, 2,  poutre::details::av::array_view, poutre::details::av::array_view, HelperOp>
  {
    void operator()(const poutre::details::av::array_view<const TIn, 2> &i_vin, const poutre::details::av::array_view<TOut, 2> &o_vout) const
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
      using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
      using lineView  = TIn *__restrict; // array_view<TIn, 1>;

      tmpBuffer tempLine(static_cast<std::size_t>(xsize));
      lineView  bufTempLine(tempLine.data());
      lineView  bufInputCurrentLine = nullptr;
      lineView  bufOuputCurrentLine = nullptr;

      // quick exit one column
      if( xsize == 1 )
      {
        poutre::details::t_Copy(i_vin, o_vout);
        return;
      }

      for( scoord y = 0; y < ysize; y++ )
      {
        bufInputCurrentLine = const_cast<TIn*>(i_vin.data()) + y * xsize;
        bufOuputCurrentLine = o_vout.data() + y * xsize;
        HelperOp::ShiftRightLeftAndArith(bufInputCurrentLine, xsize, 1, 1, bufTempLine, bufOuputCurrentLine);
      }
    }
  };

  template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateDispatcher<se::Common_NL_SE::SESegmentY2D, TIn, TOut, 2, poutre::details::av::array_view, poutre::details::av::array_view, HelperOp>
  {
    void operator()(const poutre::details::av::array_view<const TIn, 2> &i_vin, const poutre::details::av::array_view<TOut, 2> &o_vout) const
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
      using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
      // using lineView = array_view<TIn, 1>;
      using lineView = TIn *__restrict;
      tmpBuffer tempLine(static_cast<std::size_t>(xsize));
      lineView  bufTempLine(tempLine.data());
      lineView  bufInputPreviousLine = nullptr;
      lineView  bufInputCurrentLine = nullptr;
      lineView  bufInputNextLine = nullptr;
      lineView  bufOuputCurrentLine = nullptr;

      // quick run one line
      if( ysize == 1 )
      {
        poutre::details::t_Copy(i_vin, o_vout);
        return;
      }

      // compute first line
      // translate to clipped connection
      // ? . ?
      // ? x ?
      bufInputPreviousLine = const_cast<TIn *>(i_vin.data());
      bufInputNextLine     = const_cast<TIn *>(i_vin.data()) + xsize;
      ;
      bufOuputCurrentLine = o_vout.data();
      // inf/sup between dilate line 0 and line 1
      HelperOp::ApplyArith(bufInputPreviousLine, bufInputNextLine, xsize, bufOuputCurrentLine);

      // then loop
      bufInputCurrentLine = bufInputNextLine;
      for( scoord y = 2; y < ysize; y++ )
      {
        // actual computation
        // 1   <--- bufInputPreviousLine y-2
        // 4   <--- bufInputCurrentLine  y-1
        // 7   <--- bufInputNextLine     y
        bufOuputCurrentLine = o_vout.data() + xsize * (y - 1);

        bufInputNextLine     = const_cast<TIn *>(i_vin.data()) + xsize * (y);
        bufInputPreviousLine = const_cast<TIn *>(i_vin.data()) + xsize * (y - 2);
        bufInputCurrentLine  = const_cast<TIn *>(i_vin.data()) + xsize * (y - 1);

        HelperOp::ApplyArith(bufInputPreviousLine, bufInputCurrentLine, xsize, bufTempLine);
        HelperOp::ApplyArith(bufTempLine, bufInputNextLine, xsize, bufOuputCurrentLine);
      }
      // end last line
      // translate to clipped connection
      //? x ?
      //? x ?
      bufInputPreviousLine = const_cast<TIn *>(i_vin.data()) + xsize * (ysize - 2);
      // cppcheck-suppress redundantAssignment
      bufInputCurrentLine  = const_cast<TIn *>(i_vin.data()) + xsize * (ysize - 1);
      bufOuputCurrentLine  = o_vout.data() + xsize * (ysize - 1);
      HelperOp::ApplyArith(bufInputPreviousLine, bufInputCurrentLine, xsize, bufOuputCurrentLine);
    }
  };

// Helper structure
template<  typename T,
           //ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut> struct Dilate2DFor3DSquare
{
public:
  using LineOp        = LineBufferShiftAndArithDilateHelperOp<T>;

  static void ApplyArith(const ViewIn<const T, 2> &i_vin1, const ViewIn<const T, 2> &i_vin2, const ViewOut<T, 2> &o_vout)
  {
    poutre::details::t_ArithSup(i_vin1,i_vin2,o_vout);
  }
  static void EroDilOp2D(const ViewIn<const T, 2> &i_vin, const ViewOut<T, 2> &o_vout)
  {
    t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare2D, T, T, 2, ViewIn, ViewOut, LineOp> dispatcher;
    dispatcher(i_vin, o_vout);
  }
};

template<  typename T,
           //ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut> struct Dilate2DFor3DCross
{
public:
  using LineOp        = LineBufferShiftAndArithDilateHelperOp<T>;

  static void ApplyArith(const ViewIn<const T, 2> &i_vin1, const ViewIn<const T, 2> &i_vin2, const ViewOut<T, 2> &o_vout)
  {
    poutre::details::t_ArithSup(i_vin1,i_vin2,o_vout);
  }
  static void EroDilOp2D(const ViewIn<const T, 2> &i_vin, const ViewOut<T, 2> &o_vout)
  {
    t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross2D, T, T, 2, ViewIn, ViewOut, LineOp> dispatcher;
    dispatcher(i_vin, o_vout);
  }
};

template<  typename T,
           //ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut> struct Erode2DFor3DSquare
{
public:
  using LineOp        = LineBufferShiftAndArithErodeHelperOp<T>;

  static void ApplyArith(const ViewIn<const T, 2> &i_vin1, const ViewIn<const T, 2> &i_vin2, const ViewOut<T, 2> &o_vout)
  {
    poutre::details::t_ArithInf(i_vin1,i_vin2,o_vout);
  }
  static void EroDilOp2D(const ViewIn<const T, 2> &i_vin, const ViewOut<T, 2> &o_vout)
  {
    t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare2D, T, T, 2, ViewIn, ViewOut, LineOp> dispatcher;
    dispatcher(i_vin, o_vout);
  }
};


template<  typename T,
           //ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut> struct Erode2DFor3DCross
{
public:
  using LineOp        = LineBufferShiftAndArithErodeHelperOp<T>;

  static void ApplyArith(const ViewIn<const T, 2> &i_vin1, const ViewIn<const T, 2> &i_vin2, const ViewOut<T, 2> &o_vout)
  {
    poutre::details::t_ArithInf(i_vin1,i_vin2,o_vout);
  }
  static void EroDilOp2D(const ViewIn<const T, 2> &i_vin, const ViewOut<T, 2> &o_vout)
  {
    t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross2D, T, T, 2, ViewIn, ViewOut, LineOp> dispatcher;
    dispatcher(i_vin, o_vout);
  }
};

template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateDispatcher<se::Common_NL_SE::SESquare3D, TIn, TOut, 3, poutre::details::av::array_view, poutre::details::av::array_view, HelperOp>
{
  void operator()(const poutre::details::av::array_view<const TIn, 3> &i_vin, const poutre::details::av::array_view<TOut, 3> &o_vout) const
  {
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    auto   ibd     = i_vin.bound();
    auto   obd     = o_vout.bound(); // NOLINT
    auto   istride = i_vin.stride();
    auto   ostride = o_vout.stride();
    const std::size_t xsize   = static_cast<std::size_t>(ibd[0]);
    const std::size_t ysize   = static_cast<std::size_t>(ibd[1]);
    const std::size_t zsize   = static_cast<std::size_t>(ibd[2]);
    POUTRE_CHECK(ibd == obd, "bound not compatible");
    POUTRE_CHECK(istride == ostride, "stride not compatible");

    TIn *psrc  = const_cast<TIn *>(i_vin.data());
    TOut *pdest = o_vout.data();

    auto imtmp1 = poutre::details::image_t<TIn, 2>( {xsize, ysize});
    auto imtmp2 = poutre::details::image_t<TIn, 2>( {xsize, ysize});
    auto imtmp3 = poutre::details::image_t<TIn, 2>( {xsize, ysize});
    auto imtmpL = poutre::details::image_t<TIn, 2>( {xsize, ysize});
    auto imtmp  = poutre::details::image_t<TIn, 2>( {xsize, ysize});
    TIn *ppivot  = nullptr;

    TIn *ptmp1 = imtmp1.data();
    TIn *ptmp2 = imtmp2.data();
    TIn *ptmp3 = imtmp3.data();
    TIn *ptmpL = imtmpL.data();

    // first at boundary clip SE
    t_3DCopyOneSliceTo2D(psrc, xsize, ysize, xsize, ysize, 0, ptmp1);
    t_3DCopyOneSliceTo2D(psrc, xsize, ysize, xsize, ysize, 1, ptmp2);

    HelperOp::EroDilOp2D(view(imtmp1), view(imtmp));
    imtmp.swap(imtmp1);
    HelperOp::EroDilOp2D(view(imtmp2), view(imtmp));
    imtmp.swap(imtmp2);

    HelperOp::ApplyArith(view(imtmp1),view(imtmp2),view(imtmpL));
    //copy back slice to 3D
    t_3DCopyOneSliceFrom2D(ptmpL, xsize, ysize, xsize, ysize, 0, pdest);

    // main loop
    for(std::size_t i = 2 ; i< zsize ; i++) {
      t_3DCopyOneSliceTo2D(psrc, xsize, ysize, xsize, ysize, i, ptmp3);

      HelperOp::EroDilOp2D(view(imtmp3), view(imtmp));
      imtmp.swap(imtmp3);

      HelperOp::ApplyArith(view(imtmp1),view(imtmp2),view(imtmpL));
      HelperOp::ApplyArith(view(imtmpL),view(imtmp3),view(imtmp));
      imtmp.swap(imtmpL);

      t_3DCopyOneSliceFrom2D(ptmpL, xsize,ysize,xsize, ysize, i-1, pdest);

      ppivot = ptmp1;
      ptmp1 = ptmp2;
      ptmp2 = ptmp3;
      ptmp3 = ppivot;

      imtmp = imtmp1;
      imtmp1 = imtmp2;
      imtmp2 = imtmp3;
      imtmp3 = imtmp;
    }
    HelperOp::ApplyArith(view(imtmp1),view(imtmp2),view(imtmpL));

    t_3DCopyOneSliceFrom2D(ptmpL, xsize, ysize,xsize,ysize, zsize-1, pdest);
  }
};

template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateDispatcher<se::Common_NL_SE::SECross3D, TIn, TOut, 3, poutre::details::av::array_view, poutre::details::av::array_view, HelperOp>
{
  void operator()(const poutre::details::av::array_view<const TIn, 3> &i_vin, const poutre::details::av::array_view<TOut, 3> &o_vout) const
  {
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    auto   ibd     = i_vin.bound();
    auto   obd     = o_vout.bound(); // NOLINT
    auto   istride = i_vin.stride();
    auto   ostride = o_vout.stride();
    const std::size_t xsize   = static_cast<std::size_t>(ibd[0]);
    const std::size_t ysize   = static_cast<std::size_t>(ibd[1]);
    const std::size_t zsize   = static_cast<std::size_t>(ibd[2]);
    POUTRE_CHECK(ibd == obd, "bound not compatible");
    POUTRE_CHECK(istride == ostride, "stride not compatible");

    TIn *psrc  = const_cast<TIn *>(i_vin.data());
    TOut *pdest = o_vout.data();

    auto imtmp1 = poutre::details::image_t<TIn, 2>( {xsize, ysize});
    auto imtmp2 = poutre::details::image_t<TIn, 2>( {xsize, ysize});
    auto imtmp3 = poutre::details::image_t<TIn, 2>( {xsize, ysize});
    auto imtmpL = poutre::details::image_t<TIn, 2>( {xsize, ysize});
    auto imtmp  = poutre::details::image_t<TIn, 2>( {xsize, ysize});
    TIn *ppivot  = nullptr;

    TIn *ptmp1 = imtmp1.data();
    TIn *ptmp2 = imtmp2.data();
    TIn *ptmp3 = imtmp3.data();
    TIn *ptmpL = imtmpL.data();

    t_3DCopyOneSliceTo2D(psrc, xsize, ysize, xsize, ysize, 0, ptmp1);
    t_3DCopyOneSliceTo2D(psrc, xsize, ysize, xsize, ysize,  1, ptmp2);

    HelperOp::EroDilOp2D(view(imtmp1), view(imtmpL));

    HelperOp::ApplyArith(view(imtmpL),view(imtmp2),view(imtmp));
    imtmp.swap(imtmpL);

    t_3DCopyOneSliceFrom2D(ptmpL, xsize,ysize,xsize,ysize, 0, pdest);

    for(std::size_t i = 2 ; i< zsize ; i++) {
      t_3DCopyOneSliceTo2D(psrc, xsize, ysize, xsize, ysize, i, ptmp3);

      HelperOp::EroDilOp2D(view(imtmp2), view(imtmpL));

      HelperOp::ApplyArith(view(imtmpL),view(imtmp1),view(imtmpL));
      HelperOp::ApplyArith(view(imtmpL),view(imtmp3),view(imtmp));
      imtmp.swap(imtmpL);

      t_3DCopyOneSliceFrom2D(ptmpL, xsize,ysize,xsize, ysize, i-1, pdest);

      ppivot = ptmp1;
      ptmp1 = ptmp2;
      ptmp2 = ptmp3;
      ptmp3 = ppivot;

      imtmp = imtmp1;
      imtmp1 = imtmp2;
      imtmp2 = imtmp3;
      imtmp3 = imtmp;

    }
    HelperOp::EroDilOp2D(view(imtmp2), view(imtmpL));
    HelperOp::ApplyArith(view(imtmpL),view(imtmp1),view(imtmpL));

    t_3DCopyOneSliceFrom2D(ptmpL,  xsize, ysize,xsize, ysize,  zsize-1, pdest);
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
      // t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentX2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      // dispatcher(i_vin, o_vout);
      using LineOp = LineBufferShiftAndArithDilateHelperOp<TIn>;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentX2D, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentY2D: {
      // t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentY2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      // dispatcher(i_vin, o_vout);
      using LineOp = LineBufferShiftAndArithDilateHelperOp<TIn>;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentY2D, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare2D: {
      //t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      //dispatcher(i_vin, o_vout);
      using LineOp = LineBufferShiftAndArithDilateHelperOp<TIn>;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare2D, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SECross2D: {
      // t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      // dispatcher(i_vin, o_vout);
      using LineOp = LineBufferShiftAndArithDilateHelperOp<TIn>;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross2D, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
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
      //t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      //dispatcher(i_vin, o_vout);
      using Helper2DOp = Dilate2DFor3DCross<TIn,ViewIn, ViewOut> ;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross3D, TIn,TOut, Rank, ViewIn, ViewOut, Helper2DOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare3D: {
      using Helper2DOp = Dilate2DFor3DSquare<TIn,ViewIn, ViewOut> ;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare3D, TIn,TOut, Rank, ViewIn, ViewOut, Helper2DOp> dispatcher;
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
      // t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentX2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      // dispatcher(i_vin, o_vout);
      using LineOp = LineBufferShiftAndArithErodeHelperOp<TIn>;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentX2D, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentY2D: {
      // t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentY2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      // dispatcher(i_vin, o_vout);
      using LineOp = LineBufferShiftAndArithErodeHelperOp<TIn>;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESegmentY2D, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare2D: {
      // t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      // dispatcher(i_vin, o_vout);
      using LineOp = LineBufferShiftAndArithErodeHelperOp<TIn>;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare2D, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SECross2D: {
      // t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross2D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      // dispatcher(i_vin, o_vout);
      using LineOp = LineBufferShiftAndArithErodeHelperOp<TIn>;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross2D, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
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
      //t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      //dispatcher(i_vin, o_vout);
      using Helper2DOp = Erode2DFor3DCross<TIn,ViewIn, ViewOut> ;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SECross3D, TIn,TOut, Rank, ViewIn, ViewOut, Helper2DOp> dispatcher;
      dispatcher(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare3D: {
      // t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare3D, TIn,TOut, Rank, ViewIn, ViewOut, BinOp> dispatcher;
      // dispatcher(i_vin, o_vout);
      using Helper2DOp = Erode2DFor3DSquare<TIn,ViewIn, ViewOut> ;
      t_ErodeDilateDispatcher<poutre::se::Common_NL_SE::SESquare3D, TIn,TOut, Rank, ViewIn, ViewOut, Helper2DOp> dispatcher;
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