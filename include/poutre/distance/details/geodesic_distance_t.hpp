//
// Created by thomas on 27/12/2025.
//

#pragma once

/**
 * @file  geodesic_distance_t.hpp
 * @author Thomas Retornaz
 * @brief  Morphological geodesic distance operator
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

template< se::Common_NL_SE nl_static,
          typename Tmarker,
          typename Tmask,
          typename Tout,
          ptrdiff_t Rank,
          template< typename, ptrdiff_t > class ViewMarker,
          template< typename, ptrdiff_t > class ViewMask,
          template< typename, ptrdiff_t > class ViewOut >
struct t_GeodesicDistanceViewDispatcher
{
  static_assert(Rank == se::details::static_se_traits< nl_static >::rank, "SE and view have not the same Rank");
  void operator()(const ViewMarker< const Tmarker, Rank > &i_vmarker,
                  const ViewMask< const Tmask, Rank > &i_vmask,
                  const ViewOut< Tout, Rank > &o_vout) const
  {
    POUTRE_CHECK(i_vmarker.size() == o_vout.size(), "Incompatible views size");
    POUTRE_CHECK(i_vmask.size() == o_vout.size(), "Incompatible views size");
    // More check
    auto vMarkerbound = i_vmarker.bound();
    auto vMask = i_vmask.bound();
    auto vOutbound = o_vout.bound();
    auto stridevMarker = i_vmarker.stride();
    auto stridevMask = i_vmask.stride();
    auto stridevOut = o_vout.stride();
    POUTRE_CHECK(vMarkerbound == vOutbound, "Incompatible bound");
    POUTRE_CHECK(vMask == vOutbound, "Incompatible bound");
    POUTRE_CHECK(stridevMarker == stridevOut, "Incompatible stride");
    POUTRE_CHECK(stridevMask == stridevOut, "Incompatible stride");

    constexpr auto nl_coord = se::details::static_se_traits< nl_static >::coordinates_no_center;
    std::deque< poutre::details::av::index< Rank > > current_front;
    std::deque< poutre::details::av::index< Rank > > next_front;
    std::vector< LabelType > work(vMarkerbound.size(), LabelType::Candidate);
    poutre::details::av::strided_array_view< LabelType, Rank > v_work(work.data(), vMarkerbound, stridevMarker);

    const auto background = poutre::TypeTraits< Tmarker >::background();
    const auto backgroundMask = poutre::TypeTraits< Tmask >::background();
    Tout current_value = static_cast< Tout >(0);

    {
      auto beg1 = begin(vMarkerbound);
      auto end1 = end(vMarkerbound);
      for (; beg1 != end1; ++beg1) {
        if (i_vmarker[*beg1] != background) {
          if (i_vmask[*beg1] != backgroundMask) {
            o_vout[*beg1] = current_value;
            v_work[*beg1] = LabelType::Done;
            current_front.push_back(*beg1);
          }
        }
      }
    }
    ++current_value;
    while (!(current_front.empty() && next_front.empty())) {
      while (!current_front.empty()) {
        const auto current_idx = current_front.front();
        current_front.pop_front();
        for (const auto &idx_nl : nl_coord) {
          const auto delta_nl_idx = current_idx + idx_nl;
          {
            if (!vMarkerbound.contains(delta_nl_idx)) {
              continue;
            }
            if (i_vmask[delta_nl_idx] != backgroundMask) {
              if (v_work[delta_nl_idx] != LabelType::Done) {
                next_front.push_back(delta_nl_idx);
                v_work[delta_nl_idx] = LabelType::Done;
                o_vout[delta_nl_idx] = current_value;
              }
            }
          }
        }
      }
      current_front.swap(next_front);
      ++current_value;
    }
    {
      auto beg1 = begin(vMarkerbound);
      auto end1 = end(vMarkerbound);
      for (; beg1 != end1; ++beg1) {
        if (v_work[*beg1] == LabelType::Candidate) {
          o_vout[*beg1] = static_cast< Tout >(0);
        }
      }
    }
  }
};

template< typename Tmarker,
          typename Tmask,
          typename Tout,
          ptrdiff_t Rank,
          template< typename, ptrdiff_t > class ViewMarker,
          template< typename, ptrdiff_t > class ViewMask,
          template< typename, ptrdiff_t > class ViewOut >
void t_geodesic_distanceDispatch(const ViewMarker< const Tmarker, Rank > &i_vmarker,
                                 const ViewMask< const Tmask, Rank > &i_vmask,
                                 const se::Common_NL_SE nl_static,
                                 const ViewOut< Tout, Rank > &o_vout)
{
  POUTRE_CHECK(i_vmarker.size() == o_vout.size(), "t_geodesic_distanceDispatch Incompatible views size");
  POUTRE_CHECK(i_vmask.size() == o_vout.size(), "t_geodesic_distanceDispatch Incompatible views size");

  if constexpr (Rank == 1) {
    switch (nl_static) {
    case poutre::se::Common_NL_SE::SESegmentX1D: {
      // FIXME RLE must be far faster
      t_GeodesicDistanceViewDispatcher< se::Common_NL_SE::SESegmentX1D,
                                        Tmarker,
                                        Tmask,
                                        Tout,
                                        Rank,
                                        ViewMarker,
                                        ViewMask,
                                        ViewOut >
        oper;
      oper(i_vmarker, i_vmask, o_vout);
      break;
    }
    default: {
      POUTRE_RUNTIME_ERROR("t_geodesic_distanceDispatch unsupported nl_static");
    }
    }
    return;
  }
  if constexpr (Rank == 2) {
    switch (nl_static) {
    case se::Common_NL_SE::SESquare2D: {
      t_GeodesicDistanceViewDispatcher< se::Common_NL_SE::SESquare2D,
                                        Tmarker,
                                        Tmask,
                                        Tout,
                                        Rank,
                                        ViewMarker,
                                        ViewMask,
                                        ViewOut >
        oper;
      oper(i_vmarker, i_vmask, o_vout);

      break;
    }
    case se::Common_NL_SE::SECross2D: {
      t_GeodesicDistanceViewDispatcher< se::Common_NL_SE::SECross2D,
                                        Tmarker,
                                        Tmask,
                                        Tout,
                                        Rank,
                                        ViewMarker,
                                        ViewMask,
                                        ViewOut >
        oper;
      oper(i_vmarker, i_vmask, o_vout);
      break;
    }
    case se::Common_NL_SE::SESegmentX2D: {
      t_GeodesicDistanceViewDispatcher< se::Common_NL_SE::SESegmentX2D,
                                        Tmarker,
                                        Tmask,
                                        Tout,
                                        Rank,
                                        ViewMarker,
                                        ViewMask,
                                        ViewOut >
        oper;
      oper(i_vmarker, i_vmask, o_vout);
      break;
    }
    case se::Common_NL_SE::SESegmentY2D: {
      t_GeodesicDistanceViewDispatcher< se::Common_NL_SE::SESegmentY2D,
                                        Tmarker,
                                        Tmask,
                                        Tout,
                                        Rank,
                                        ViewMarker,
                                        ViewMask,
                                        ViewOut >
        oper;
      oper(i_vmarker, i_vmask, o_vout);
      break;
    }
    default: {
      POUTRE_RUNTIME_ERROR("t_geodesic_distanceDispatch unsupported nl_static");
    }
    }
    return;
  }
  if constexpr (Rank == 3) {
    switch (nl_static) {
    case se::Common_NL_SE::SECross3D: {
      t_GeodesicDistanceViewDispatcher< se::Common_NL_SE::SECross3D,
                                        Tmarker,
                                        Tmask,
                                        Tout,
                                        Rank,
                                        ViewMarker,
                                        ViewMask,
                                        ViewOut >
        oper;
      oper(i_vmarker, i_vmask, o_vout);
      break;
    }
    case se::Common_NL_SE::SESquare3D: {
      t_GeodesicDistanceViewDispatcher< se::Common_NL_SE::SESquare3D,
                                        Tmarker,
                                        Tmask,
                                        Tout,
                                        Rank,
                                        ViewMarker,
                                        ViewMask,
                                        ViewOut >
        oper;
      oper(i_vmarker, i_vmask, o_vout);
      break;
    }
    case se::Common_NL_SE::SESegmentX3D: {
      t_GeodesicDistanceViewDispatcher< se::Common_NL_SE::SESegmentX3D,
                                        Tmarker,
                                        Tmask,
                                        Tout,
                                        Rank,
                                        ViewMarker,
                                        ViewMask,
                                        ViewOut >
        oper;
      oper(i_vmarker, i_vmask, o_vout);
      break;
    }
    case se::Common_NL_SE::SESegmentY3D: {
      t_GeodesicDistanceViewDispatcher< se::Common_NL_SE::SESegmentY3D,
                                        Tmarker,
                                        Tmask,
                                        Tout,
                                        Rank,
                                        ViewMarker,
                                        ViewMask,
                                        ViewOut >
        oper;
      oper(i_vmarker, i_vmask, o_vout);
      break;
    }
    case se::Common_NL_SE::SESegmentZ3D: {
      t_GeodesicDistanceViewDispatcher< se::Common_NL_SE::SESegmentZ3D,
                                        Tmarker,
                                        Tmask,
                                        Tout,
                                        Rank,
                                        ViewMarker,
                                        ViewMask,
                                        ViewOut >
        oper;
      oper(i_vmarker, i_vmask, o_vout);
      break;
    }
    default: {
      POUTRE_RUNTIME_ERROR("t_geodesic_distanceDispatch unsupported nl_static");
    }
    }
    return;
  }
  // std::unreachable();
  // POUTRE_RUNTIME_ERROR("t_geodesic_distanceDispatch unsupported nb dims");
  // cppcheck-suppress missingReturn
  // return 0; //NOLINT
}

template< typename Tmarker, typename Tmask, typename Tout, ptrdiff_t Rank >
void t_geodesic_distance(const poutre::details::image_t< Tmarker, Rank > &i_marker,
                         const poutre::details::image_t< Tmask, Rank > &i_mask,
                         poutre::se::Common_NL_SE nl_static,
                         poutre::details::image_t< Tout, Rank > &o_img)
{
  AssertSizesCompatible(i_marker, o_img, "t_geodesic_distance incompatible size");
  AssertSizesCompatible(i_mask, o_img, "t_geodesic_distance incompatible size");

  AssertAsTypesCompatible(i_marker, o_img, "t_geodesic_distance incompatible types");
  AssertAsTypesCompatible(i_mask, o_img, "t_geodesic_distance incompatible types");

  AssertImagesAreDifferent(i_marker, o_img, "t_geodesic_distance output must be != than input images");
  AssertImagesAreDifferent(i_mask, o_img, "t_geodesic_distance output must be != than input images");

  auto viewMarker = view(i_marker);
  auto viewMask = view(i_mask);
  auto viewOut = view(o_img);

  t_geodesic_distanceDispatch(viewMarker, viewMask, nl_static, viewOut);
}

//! @} doxygroup: poutre_distance_group
}// namespace poutre::dist::details