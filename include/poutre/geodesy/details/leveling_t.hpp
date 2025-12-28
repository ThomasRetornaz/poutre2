
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file  leveling_t.hpp
 * @author Thomas Retornaz
 * @brief  Leveling operator
 *
 *
 */

#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/data_structures/pq.hpp>
#include <poutre/low_level_morpho/details/ero_dil_static_se_t.hpp>
#include <poutre/pixel_processing/details/arith_op_t.hpp>
#include <poutre/structuring_element/details/neighbor_list_static_se_t.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace poutre::geo::details {
/**
 * @addtogroup poutre_geodesy_group
 *@{
*/

template< poutre::se::Common_NL_SE nl_static,
          typename Tref,
          typename Tmarker,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewRef,
          template<typename, ptrdiff_t>
          class ViewMarker,
          template<typename, ptrdiff_t>
          class ViewOut>
struct t_low_levelingHelper
{
  static_assert(Rank == poutre::se::details::static_se_traits<nl_static>::rank, "SE and view have not the same Rank");
  void operator()(const ViewRef<const Tref, Rank> &i_vref,
                  const ViewMarker<const Tmarker, Rank> &i_vmarker,
                  ViewOut<Tout, Rank> &o_vout) const
  {
    POUTRE_CHECK(i_vref.size() == i_vmarker.size(), "Incompatible views size");
    POUTRE_CHECK(i_vmarker.size() == o_vout.size(), "Incompatible views size");

    // More check
    auto vRefbound   = i_vref.bound();
    auto vMarkerbound   = i_vmarker.bound();
    auto vOutbound  = o_vout.bound();
    auto stridevRef  = i_vref.stride();
    auto stridevMarker  = i_vmarker.stride();
    auto stridevOut = o_vout.stride();
    POUTRE_CHECK(vMarkerbound==vRefbound, "Incompatible bound");
    POUTRE_CHECK(vOutbound==vMarkerbound, "Incompatible bound");

    POUTRE_CHECK(stridevMarker==stridevRef, "Incompatible stride");
    POUTRE_CHECK(stridevMarker==stridevOut, "Incompatible stride");

    // copy marker -> out
    // out will be modified in place
    poutre::details::t_Copy(i_vmarker,o_vout);

    constexpr auto nl_coord     = poutre::se::details::static_se_traits<nl_static>::coordinates_no_center;
    poutre::details::poutre_pq<poutre::details::av::index<Rank>, Tout> pqueue;

    // forward scan
    {
      auto beg1 = begin(vMarkerbound);
      auto end1 = end(vMarkerbound);
      for( ; beg1 != end1; ++beg1 )
      {
        if (o_vout[*beg1]<i_vref[*beg1]) {
          auto curr_max = o_vout[*beg1];
          for( const auto &idx_nl : nl_coord )
          {
            const auto delta_nl_idx = *beg1 + idx_nl;
            if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
            curr_max = std::max(curr_max, o_vout[delta_nl_idx]) ;
          }
          if (curr_max != o_vout[*beg1]) {
            o_vout[*beg1] = std::min(i_vref[*beg1], curr_max);
            pqueue.emplace(*beg1,o_vout[*beg1]);
          }
        }
      }
    }

    // Loop until all pixel have been examined
    while (!pqueue.empty()) {
      auto [idx,_] = pqueue.top();
      pqueue.pop();
      for( const auto &idx_nl : nl_coord ) {
        const auto delta_nl_idx = idx + idx_nl;
        if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
        if ( o_vout[delta_nl_idx] < o_vout[idx] && o_vout[delta_nl_idx]<i_vref[delta_nl_idx]) {
          auto new_value = std::min(i_vref[delta_nl_idx], o_vout[idx]);
          if (o_vout[delta_nl_idx]!= new_value) {
            o_vout[delta_nl_idx] = new_value;
            pqueue.emplace(delta_nl_idx, new_value);
          }
        }
      }
    }

    // Post-processing
    poutre::details::t_ArithInf(o_vout,i_vref,o_vout);
  }
};

template< poutre::se::Common_NL_SE nl_static,
          typename Tref,
          typename Tmarker,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewRef,
          template<typename, ptrdiff_t>
          class ViewMarker,
          template<typename, ptrdiff_t>
          class ViewOut>
struct t_high_levelingHelper
{
  static_assert(Rank == poutre::se::details::static_se_traits<nl_static>::rank, "SE and view have not the same Rank");
  void operator()(const ViewRef<const Tref, Rank> &i_vref,
                  const ViewMarker<const Tmarker, Rank> &i_vmarker,
                  ViewOut<Tout, Rank> &o_vout) const
  {
    POUTRE_CHECK(i_vref.size() == i_vmarker.size(), "Incompatible views size");
    POUTRE_CHECK(i_vmarker.size() == o_vout.size(), "Incompatible views size");

    // More check
    auto vRefbound   = i_vref.bound();
    auto vMarkerbound   = i_vmarker.bound();
    auto vOutbound  = o_vout.bound();
    auto stridevRef  = i_vref.stride();
    auto stridevMarker  = i_vmarker.stride();
    auto stridevOut = o_vout.stride();
    POUTRE_CHECK(vMarkerbound==vRefbound, "Incompatible bound");
    POUTRE_CHECK(vOutbound==vMarkerbound, "Incompatible bound");

    POUTRE_CHECK(stridevMarker==stridevRef, "Incompatible stride");
    POUTRE_CHECK(stridevMarker==stridevOut, "Incompatible stride");

    // copy marker -> out
    // out will be modified in place
    poutre::details::t_Copy(i_vmarker,o_vout);

    constexpr auto nl_coord     = poutre::se::details::static_se_traits<nl_static>::coordinates_no_center;
    poutre::details::poutre_pq<poutre::details::av::index<Rank>, Tout> pqueue;

    // forward scan
    {
      auto beg1 = begin(vMarkerbound);
      auto end1 = end(vMarkerbound);
      for( ; beg1 != end1; ++beg1 )
      {
        if (o_vout[*beg1]>i_vref[*beg1]) {
          auto curr_min = o_vout[*beg1];
          for( const auto &idx_nl : nl_coord )
          {
            const auto delta_nl_idx = *beg1 + idx_nl;
            if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
            curr_min = std::min(curr_min, o_vout[delta_nl_idx]) ;
          }
          if (curr_min != o_vout[*beg1]) {
            o_vout[*beg1] = std::max(i_vref[*beg1], curr_min);
            pqueue.emplace(*beg1,o_vout[*beg1]);
          }
        }
      }
    }

    // Loop until all pixel have been examined
    while (!pqueue.empty()) {
      auto [idx,_] = pqueue.top();
      pqueue.pop();
      for( const auto &idx_nl : nl_coord ) {
        const auto delta_nl_idx = idx + idx_nl;
        if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
        if ( o_vout[delta_nl_idx] > o_vout[idx] && o_vout[delta_nl_idx] > i_vref[delta_nl_idx]) {
          auto new_value = std::max(i_vref[delta_nl_idx], o_vout[idx]);
          if (o_vout[delta_nl_idx]!= new_value) {
            o_vout[delta_nl_idx] = new_value;
            pqueue.emplace(delta_nl_idx, new_value);
          }
        }
      }
    }

    // Post-processing
    poutre::details::t_ArithSup(o_vout,i_vref,o_vout);
  }
};

template< typename Tref,
          typename Tmarker,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewRef,
          template<typename, ptrdiff_t>
          class ViewMarker,
          template<typename, ptrdiff_t>
          class ViewOut>
void t_low_levelingDispatch(
    const ViewRef<const Tmarker, Rank> &i_vref,
    const ViewMarker<const Tref, Rank> &i_vmarker,
    const poutre::se::Common_NL_SE nl_static,
    ViewOut<Tout, Rank> &o_vout)
{
  POUTRE_CHECK(i_vmarker.size() == i_vref.size(), "t_low_levelingDispatch Incompatible views size");
  POUTRE_CHECK(i_vref.size() == o_vout.size(), "t_low_levelingDispatch Incompatible views size");;

  if constexpr (Rank == 1) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SESegmentX1D: {
      t_low_levelingHelper<poutre::se::Common_NL_SE::SESegmentX1D, Tref, Tmarker, Tout, Rank, ViewRef, ViewMarker, ViewOut> op;
      op(i_vref, i_vmarker, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_low_levelingDispatch unsupported nl_static");}
    }
    return;
  }
  if constexpr (Rank == 2) {
    switch(nl_static) {
    // case poutre::se::Common_NL_SE::SESegmentX2D: {
    //   t_low_levelingHelper<poutre::se::Common_NL_SE::SESegmentX2D>(i_vref,i_vmarker, o_vout);
    // } break;
    // case poutre::se::Common_NL_SE::SESegmentY2D: {
    //   t_low_levelingHelper<poutre::se::Common_NL_SE::SESegmentY2D>(i_vref,i_vmarker, o_vout);
    // } break;
    case poutre::se::Common_NL_SE::SESquare2D: {
      //t_low_levelingHelper<poutre::se::Common_NL_SE::SESquare2D, Tref,Tmarker,Tout, Rank, ViewRef,ViewMarker, ViewOut> dispatcher;
      t_low_levelingHelper<poutre::se::Common_NL_SE::SESquare2D, Tref, Tmarker, Tout, Rank, ViewRef, ViewMarker, ViewOut> op;
      op(i_vref, i_vmarker, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SECross2D: {
      t_low_levelingHelper<poutre::se::Common_NL_SE::SECross2D, Tref, Tmarker, Tout, Rank, ViewRef, ViewMarker, ViewOut> op;
      op(i_vref, i_vmarker, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_low_levelingDispatch unsupported nl_static");}
    }
    //return;
  }
  if constexpr (Rank == 3) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SECross3D: {
      t_low_levelingHelper<poutre::se::Common_NL_SE::SECross3D,Tref, Tmarker, Tout, Rank, ViewRef, ViewMarker, ViewOut> op;
      op(i_vref, i_vmarker, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare3D: {
      t_low_levelingHelper<poutre::se::Common_NL_SE::SESquare3D, Tref, Tmarker, Tout, Rank, ViewRef, ViewMarker, ViewOut> op;
      op(i_vref, i_vmarker, o_vout);
    } break;
    // case poutre::se::Common_NL_SE::SESegmentX3D: {
    //   t_low_levelingHelper<poutre::se::Common_NL_SE::SESegmentX3D>(i_vref,i_vmarker, o_vout);
    // } break;
    // case poutre::se::Common_NL_SE::SESegmentY3D: {
    //   t_low_levelingHelper<poutre::se::Common_NL_SE::SESegmentY3D>(i_vref,i_vmarker, o_vout);
    // } break;
    // case poutre::se::Common_NL_SE::SESegmentZ3D: {
    //   t_low_levelingHelper<poutre::se::Common_NL_SE::SESegmentZ3D>(i_vref,i_vmarker, o_vout);
    // } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_low_levelingDispatch unsupported nl_static");}
    }
    //return;
  }
}


template< typename Tref,
          typename Tmarker,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewRef,
          template<typename, ptrdiff_t>
          class ViewMarker,
          template<typename, ptrdiff_t>
          class ViewOut>
void t_high_levelingDispatch(
    const ViewRef<const Tmarker, Rank> &i_vref,
    const ViewMarker<const Tref, Rank> &i_vmarker,
    const poutre::se::Common_NL_SE nl_static,
    ViewOut<Tout, Rank> &o_vout)
{
  POUTRE_CHECK(i_vmarker.size() == i_vref.size(), "t_high_levelingDispatch Incompatible views size");
  POUTRE_CHECK(i_vref.size() == o_vout.size(), "t_high_levelingDispatch Incompatible views size");;

  if constexpr (Rank == 1) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SESegmentX1D: {
      t_high_levelingHelper<poutre::se::Common_NL_SE::SESegmentX1D, Tref, Tmarker, Tout, Rank, ViewRef, ViewMarker, ViewOut> op;
      op(i_vref, i_vmarker, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_high_levelingDispatch unsupported nl_static");}
    }
    return;
  }
  if constexpr (Rank == 2) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SESquare2D: {
      //t_low_levelingHelper<poutre::se::Common_NL_SE::SESquare2D, Tref,Tmarker,Tout, Rank, ViewRef,ViewMarker, ViewOut> dispatcher;
      t_high_levelingHelper<poutre::se::Common_NL_SE::SESquare2D, Tref, Tmarker, Tout, Rank, ViewRef, ViewMarker, ViewOut> op;
      op(i_vref, i_vmarker, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SECross2D: {
      t_high_levelingHelper<poutre::se::Common_NL_SE::SECross2D, Tref, Tmarker, Tout, Rank, ViewRef, ViewMarker, ViewOut> op;
      op(i_vref, i_vmarker, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_high_levelingDispatch unsupported nl_static");}
    }
  }
  if constexpr (Rank == 3) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SECross3D: {
      t_high_levelingHelper<poutre::se::Common_NL_SE::SECross3D,Tref, Tmarker, Tout, Rank, ViewRef, ViewMarker, ViewOut> op;
      op(i_vref, i_vmarker, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare3D: {
      t_high_levelingHelper<poutre::se::Common_NL_SE::SESquare3D, Tref, Tmarker, Tout, Rank, ViewRef, ViewMarker, ViewOut> op;
      op(i_vref, i_vmarker, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_high_levelingDispatch unsupported nl_static");}
    }
  }
}
template<typename Tref, typename Tmarker, typename Tout, ptrdiff_t Rank>
void t_low_leveling(
    const poutre::details::image_t<Tref, Rank> &i_ref,
    const poutre::details::image_t<Tmarker, Rank> &i_marker,
    poutre::se::Common_NL_SE nl_static,
    poutre::details::image_t<Tout, Rank> &o_img)
{
  AssertSizesCompatible(i_ref, o_img, "t_low_leveling incompatible size");
  AssertSizesCompatible(i_marker, o_img, "t_low_leveling incompatible size");

  AssertAsTypesCompatible(i_ref, o_img, "t_low_leveling incompatible types");
  AssertAsTypesCompatible(i_marker, o_img, "t_low_leveling incompatible types");

  AssertImagesAreDifferent(i_ref, o_img, "t_low_leveling output must be != than input images");
  AssertImagesAreDifferent(i_marker, o_img, "t_low_leveling output must be != than input images");

  auto viewRef= view(i_ref);
  auto viewMarker= view(i_marker);
  auto viewOut = view(o_img);
  return t_low_levelingDispatch(viewRef, viewMarker, nl_static, viewOut);
}

template<typename Tref, typename Tmarker, typename Tout, ptrdiff_t Rank>
void t_high_leveling(
    const poutre::details::image_t<Tref, Rank> &i_ref,
    const poutre::details::image_t<Tmarker, Rank> &i_marker,
    poutre::se::Common_NL_SE nl_static,
    poutre::details::image_t<Tout, Rank> &o_img)
{
  AssertSizesCompatible(i_ref, o_img, "t_high_leveling incompatible size");
  AssertSizesCompatible(i_marker, o_img, "t_high_leveling incompatible size");

  AssertAsTypesCompatible(i_ref, o_img, "t_high_leveling incompatible types");
  AssertAsTypesCompatible(i_marker, o_img, "t_high_leveling incompatible types");

  AssertImagesAreDifferent(i_ref, o_img, "t_high_leveling output must be != than input images");
  AssertImagesAreDifferent(i_marker, o_img, "t_high_leveling output must be != than input images");

  auto viewRef= view(i_ref);
  auto viewMarker= view(i_marker);
  auto viewOut = view(o_img);
  return t_high_levelingDispatch(viewRef, viewMarker, nl_static, viewOut);
}
//! @} doxygroup: poutre_geo_group
}//poutre::geo::details