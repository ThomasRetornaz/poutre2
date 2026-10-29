//
// Created by thomas on 27/12/2025.
//

#pragma once

/**
 * @file  mdistance_t.hpp
 * @author Thomas Retornaz
 * @brief  Morphological distance operator
 *
 *
 */


#include "poutre/pixel_processing/details/compare_op_t.hpp"


#include <deque>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/structuring_element/details/neighbor_list_static_se_t.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace poutre::dist::details {
/**
 * @addtogroup poutre_distance_group
 *@{
 */

template< poutre::se::Common_NL_SE nl_static,
          typename Tmarker,
          typename Tout,
          ptrdiff_t Rank,
          template< typename, ptrdiff_t > class ViewMarker,
          template< typename, ptrdiff_t > class ViewOut >
struct t_MDistanceViewDispatcher
{
  static_assert(Rank == poutre::se::details::static_se_traits< nl_static >::rank, "SE and view have not the same Rank");
  void operator()(const ViewMarker< const Tmarker, Rank > &i_vmarker, const ViewOut< Tout, Rank > &o_vout) const
  {
    POUTRE_CHECK(i_vmarker.size() == o_vout.size(), "Incompatible views size");
    // More check
    auto vMarkerbound = i_vmarker.bound();
    auto vOutbound = o_vout.bound();
    auto stridevMarker = i_vmarker.stride();
    auto stridevOut = o_vout.stride();
    POUTRE_CHECK(vMarkerbound == vOutbound, "Incompatible bound");
    POUTRE_CHECK(stridevMarker == stridevOut, "Incompatible stride");

    constexpr auto nl_coord = poutre::se::details::static_se_traits< nl_static >::coordinates_no_center;
    std::deque< poutre::details::av::index< Rank > > current_front;
    std::vector< poutre::LabelType > work(vMarkerbound.size(), poutre::LabelType::Candidate);
    poutre::details::av::strided_array_view< poutre::LabelType, Rank > v_work(work.data(), vMarkerbound, stridevMarker);

    const auto background = poutre::TypeTraits< Tmarker >::background();

    // first scan, put candidate pixels in front
    // set zero pixels to done
    {
      auto beg1 = begin(vMarkerbound);
      auto end1 = end(vMarkerbound);
      for (; beg1 != end1; ++beg1) {
        if (i_vmarker[*beg1] == background) {
          o_vout[*beg1] = static_cast< Tout >(0);
          v_work[*beg1] = poutre::LabelType::Done;
          for (const auto &idx_nl : nl_coord) {
            const auto delta_nl_idx = *beg1 + idx_nl;
            if (!vMarkerbound.contains(delta_nl_idx)) {
              continue;
            }
            if (v_work[delta_nl_idx] != poutre::LabelType::Done && i_vmarker[delta_nl_idx] != background) {
              current_front.push_back(delta_nl_idx);
              v_work[delta_nl_idx] = poutre::LabelType::Done;
            }
          }
        }
      }
    }

    {
      std::deque< poutre::details::av::index< Rank > > next_front;
      Tout current_value = static_cast< Tout >(1);
      while (!(current_front.empty() && next_front.empty())) {
        while (!current_front.empty()) {
          const auto current_idx = current_front.front();
          current_front.pop_front();
          o_vout[current_idx] = current_value;
          for (const auto &idx_nl : nl_coord) {
            const auto delta_nl_idx = current_idx + idx_nl;
            if (!vMarkerbound.contains(delta_nl_idx)) {
              continue;
            }
            if (v_work[delta_nl_idx] != poutre::LabelType::Done && i_vmarker[delta_nl_idx] != background) {
              next_front.push_back(delta_nl_idx);
              v_work[delta_nl_idx] = poutre::LabelType::Done;
            }
          }
        }//// while 1 !empty
        current_front.swap(next_front);
        ++current_value;
      }// while 1|2 !empty
    }

    // poutre::details::ViewCompare_sii_dispatch(
    //   v_work, CompOpType::CompOpEqual, poutre::LabelType::Candidate, 0, o_vout, o_vout)
    {
      auto beg1 = begin(vMarkerbound);
      auto end1 = end(vMarkerbound);
      for (; beg1 != end1; ++beg1) {
        if (v_work[*beg1] == poutre::LabelType::Candidate) {
          o_vout[*beg1] = static_cast< Tout >(0);
        }
      }
    }
  }
};


template< typename Tin,
          typename Tout,
          ptrdiff_t Rank,
          template< typename, ptrdiff_t > class ViewIn,
          template< typename, ptrdiff_t > class ViewOut >
void t_mdistanceDispatch(const ViewIn< const Tin, Rank > &i_vin,
                         const poutre::se::Common_NL_SE nl_static,
                         const ViewOut< Tout, Rank > &o_vout)
{
  POUTRE_CHECK(i_vin.size() == o_vout.size(), "t_mdistanceDispatch Incompatible views size");

  if constexpr (Rank == 1) {
    switch (nl_static) {
    case poutre::se::Common_NL_SE::SESegmentX1D: {
      // FIXME RLE must be far faster
      t_MDistanceViewDispatcher< poutre::se::Common_NL_SE::SESegmentX1D, Tin, Tout, Rank, ViewIn, ViewOut > oper;
      oper(i_vin, o_vout);
      break;
    }
    default: {
      POUTRE_RUNTIME_ERROR("t_mdistanceDispatch unsupported nl_static");
    }
    }
    return;
  }
  if constexpr (Rank == 2) {
    switch (nl_static) {
    case poutre::se::Common_NL_SE::SESquare2D: {
      t_MDistanceViewDispatcher< poutre::se::Common_NL_SE::SESquare2D, Tin, Tout, Rank, ViewIn, ViewOut > oper;
      oper(i_vin, o_vout);
      break;
    }
    case poutre::se::Common_NL_SE::SECross2D: {
      t_MDistanceViewDispatcher< poutre::se::Common_NL_SE::SECross2D, Tin, Tout, Rank, ViewIn, ViewOut > oper;
      oper(i_vin, o_vout);
      break;
    }
    case poutre::se::Common_NL_SE::SESegmentX2D: {
      t_MDistanceViewDispatcher< poutre::se::Common_NL_SE::SESegmentX2D, Tin, Tout, Rank, ViewIn, ViewOut > oper;
      oper(i_vin, o_vout);
      break;
    }
    case poutre::se::Common_NL_SE::SESegmentY2D: {
      t_MDistanceViewDispatcher< poutre::se::Common_NL_SE::SESegmentY2D, Tin, Tout, Rank, ViewIn, ViewOut > oper;
      oper(i_vin, o_vout);
      break;
    }
    default: {
      POUTRE_RUNTIME_ERROR("t_mdistanceDispatch unsupported nl_static");
    }
    }
    return;
  }
  if constexpr (Rank == 3) {
    switch (nl_static) {
    case poutre::se::Common_NL_SE::SECross3D: {
      t_MDistanceViewDispatcher< poutre::se::Common_NL_SE::SECross3D, Tin, Tout, Rank, ViewIn, ViewOut > oper;
      oper(i_vin, o_vout);
      break;
    }
    case poutre::se::Common_NL_SE::SESquare3D: {
      t_MDistanceViewDispatcher< poutre::se::Common_NL_SE::SESquare3D, Tin, Tout, Rank, ViewIn, ViewOut > oper;
      oper(i_vin, o_vout);
      break;
    }
    case poutre::se::Common_NL_SE::SESegmentX3D: {
      t_MDistanceViewDispatcher< poutre::se::Common_NL_SE::SESegmentX3D, Tin, Tout, Rank, ViewIn, ViewOut > oper;
      oper(i_vin, o_vout);
      break;
    }
    case poutre::se::Common_NL_SE::SESegmentY3D: {
      t_MDistanceViewDispatcher< poutre::se::Common_NL_SE::SESegmentY3D, Tin, Tout, Rank, ViewIn, ViewOut > oper;
      oper(i_vin, o_vout);
      break;
    }
    case poutre::se::Common_NL_SE::SESegmentZ3D: {
      t_MDistanceViewDispatcher< poutre::se::Common_NL_SE::SESegmentZ3D, Tin, Tout, Rank, ViewIn, ViewOut > oper;
      oper(i_vin, o_vout);
      break;
    }
    default: {
      POUTRE_RUNTIME_ERROR("t_mdistanceDispatch unsupported nl_static");
    }
    }
    return;
  }
  // std::unreachable();
  // POUTRE_RUNTIME_ERROR("t_mdistanceDispatch unsupported nb dims");
  // cppcheck-suppress missingReturn
  // return 0; //NOLINT
}


template< typename Tin, typename Tout, ptrdiff_t Rank >
void t_mdistance(const poutre::details::image_t< Tin, Rank > &i_img,
                 poutre::se::Common_NL_SE nl_static,
                 poutre::details::image_t< Tout, Rank > &o_img)
{
  AssertSizesCompatible(i_img, o_img, "t_mdistance incompatible size");
  AssertImagesAreDifferent(i_img, o_img, "t_mdistance output must be != than input images");

  auto viewIn = view(i_img);
  auto viewOut = view(o_img);

  t_mdistanceDispatch(viewIn, nl_static, viewOut);
}


//! @} doxygroup: poutre_distance_group
}// namespace poutre::dist::details