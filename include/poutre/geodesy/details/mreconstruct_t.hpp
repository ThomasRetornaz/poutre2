
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   mreconstruct_t.hpp
 * @author Thomas Retornaz
 * @brief  Reconstruction operator aka geodesic dilate/erode
 *
 *
 */

#include <algorithm>
#include <deque>
#include <filesystem>
#include <memory>
#include <numeric>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/geodesy/geodesy.hpp>
#include <poutre/low_level_morpho/details/ero_dil_static_se_t.hpp>
#include <poutre/pixel_processing/details/arith_op_t.hpp>
#include <poutre/structuring_element/details/neighbor_list_static_se_t.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <queue>
#include <vector>

namespace poutre::geo::details {
/**
 * @addtogroup poutre_geo_group
 *@{
*/

template<typename Tmarker,
          typename Tmask,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewMarker,
          template<typename, ptrdiff_t>
          class ViewMask,
          template<typename, ptrdiff_t>
          class ViewOut> struct OpRecDilate
{
  static void select_marker(
                    const ViewMarker<const Tmarker, Rank> &i_vmarker,
                    const ViewMask<const Tmask, Rank> &i_vmask,
                    const ViewOut<Tout, Rank> &o_vout)
  {
    poutre::details::t_ArithInf(i_vmarker,i_vmask,o_vout);
  }
  static Tout select_marker(Tout val_out, Tmask val_mask) { return std::min<Tout>(val_out, val_mask); }
  static Tout select_se(Tout val_out, Tmask val_mask) { return std::max<Tout>(val_out, val_mask);}
  static bool should_enqueue(Tout val_out, Tmask val_mask) { return val_out<val_mask ; }
};

template<typename Tmarker,
          typename Tmask,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewMarker,
          template<typename, ptrdiff_t>
          class ViewMask,
          template<typename, ptrdiff_t>
          class ViewOut> struct OpRecErode
{
  static void select_marker(
                    const ViewMarker<const Tmarker, Rank> &i_vmarker,
                    const ViewMask<const Tmask, Rank> &i_vmask,
                    const ViewOut<Tout, Rank> &o_vout)
  {
    poutre::details::t_ArithSup(i_vmarker,i_vmask,o_vout);
  }
  static Tout select_marker(Tout val_out, Tmask val_mask) { return std::max<Tout>(val_out, val_mask); }
  static Tout select_se(Tout val_out, Tmask val_mask) { return std::min<Tout>(val_out, val_mask);}
  static bool should_enqueue(Tout val_out, Tmask val_mask) { return val_out>val_mask ; }
};

template< poutre::se::Common_NL_SE nl_static,
          typename Tmarker,
          typename Tmask,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewMarker,
          template<typename, ptrdiff_t>
          class ViewMask,
          template<typename, ptrdiff_t>
          class ViewOut,
          class HelperOp>
struct t_ReconstructionViewDispatcher
{
  static_assert(Rank == poutre::se::details::static_se_traits<nl_static>::rank, "SE and view have not the same Rank");
  void operator()(const ViewMarker<const Tmarker, Rank> &i_vmarker,
                  const ViewMask<const Tmask, Rank> &i_vmask,
                  ViewOut<Tout, Rank> &o_vout) const
  {
    POUTRE_CHECK(i_vmarker.size() == i_vmask.size(), "Incompatible views size");
    POUTRE_CHECK(i_vmask.size() == o_vout.size(), "Incompatible views size");

    // More check
    auto vMarkerbound   = i_vmarker.bound();
    auto vMaskbound   = i_vmask.bound();
    auto vOutbound  = o_vout.bound();
    auto stridevMarker  = i_vmarker.stride();
    auto stridevMask  = i_vmask.stride();
    auto stridevOut = o_vout.stride();
    POUTRE_CHECK(vMarkerbound==vMaskbound, "Incompatible bound");
    POUTRE_CHECK(vOutbound==vMaskbound, "Incompatible bound");

    POUTRE_CHECK(stridevMarker==stridevMask, "Incompatible stride");
    POUTRE_CHECK(stridevMask==stridevOut, "Incompatible stride");

    constexpr auto nl_coord     = poutre::se::details::static_se_traits<nl_static>::coordinates_no_center;
    auto [nl_upper, nl_lower]   = poutre::se::details::static_se_traits<nl_static>::split_coordinates_upper_lower();
    std::queue<poutre::details::av::index<Rank>> queue;

    HelperOp::select_marker(i_vmarker, i_vmask, o_vout);

    // forward scan
    {
      auto beg1 = begin(vMarkerbound);
      auto end1 = end(vMarkerbound);
      for( ; beg1 != end1; ++beg1 )
      {
        auto curr_val = o_vout[*beg1];
        for( const auto &idx_nl_upper : nl_upper )
        {
          const auto delta_nl_idx = *beg1 + idx_nl_upper;
          if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
          curr_val = HelperOp::select_se(curr_val, o_vout[delta_nl_idx]);
        }
        o_vout[*beg1] = HelperOp::select_marker(curr_val, i_vmask[*beg1]);
      }
    }

    //backward scan
    {
      auto rbeg1 = rbegin(vMarkerbound);
      auto rend1 = rend(vMarkerbound);
      for( ; rbeg1 != rend1; ++rbeg1 ) {
        auto curr_val = o_vout[*rbeg1];
        for( const auto &idx_nl_lower : nl_lower ) {
          auto delta_nl_idx = *rbeg1 + idx_nl_lower;
          if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
          curr_val = HelperOp::select_se(curr_val, o_vout[delta_nl_idx]);
        }
        o_vout[*rbeg1] = HelperOp::select_marker(curr_val, i_vmask[*rbeg1]);

        for( const auto &idx_nl_lower : nl_lower ) {
          auto delta_nl_idx = *rbeg1 + idx_nl_lower;
          if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
          if (
            HelperOp::should_enqueue(o_vout[delta_nl_idx], o_vout[*rbeg1])
            && HelperOp::should_enqueue(o_vout[delta_nl_idx], i_vmask[delta_nl_idx])) {
            queue.push(*rbeg1);
            }
        }
      }
    }

    // // temp display
    // {
    //   std::cout<<"After forward, backward \n";
    //   std::cout<<"out \n";
    //   {
    //     auto begit = begin(vMarkerbound);
    //     auto endit = end(vMarkerbound);
    //     for( ; begit != endit; ++begit ) {
    //       std::cout << static_cast<int>(o_vout[*begit]) << " ";
    //     }
    //     std::cout<<"\n";
    //   }
    //   std::cout<<"mask \n";
    //   {
    //     auto begit = begin(vMarkerbound);
    //     auto endit = end(vMarkerbound);
    //     for( ; begit != endit; ++begit ) {
    //       std::cout << static_cast<int>(i_vmask[*begit]) << " ";
    //     }
    //     std::cout<<"\n";
    //   }
    // }
  while( !queue.empty() ) {
    const auto idx = queue.front();
    queue.pop();
    // scoord const centerx = idx[1];
    // scoord const centery = idx[0];
    // std::cout <<"pop:"<< centerx << ", " << centery << "\n";
    // std::cout <<"vout[center]="<<o_vout[idx]<< "\n";
    for( const auto &idx_nl : nl_coord ) {
      const auto delta_nl_idx = idx + idx_nl;
      if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
      if (
        i_vmask[delta_nl_idx] != o_vout[delta_nl_idx]
        &&
        HelperOp::should_enqueue(o_vout[delta_nl_idx],o_vout[idx]))
        {
        o_vout[delta_nl_idx] = HelperOp::select_marker(o_vout[idx], i_vmask[delta_nl_idx]);
        // std::cout <<"push:"<< delta_nl_idx[1] << ", " << delta_nl_idx[0] << "\n";
        // std::cout <<"vout[delta]="<<o_vout[delta_nl_idx]<< "\n";
        // std::cout <<"mask[delta]="<<i_vmask[delta_nl_idx]<< "\n";
        queue.push(delta_nl_idx);
      }
    }
  }
  }
};

/*
// specialisation array_view 2D
template< poutre::se::Common_NL_SE nl_static,
          typename Tmarker,
          typename Tmask,
          typename Tout,
          class HelperOp>
struct t_ReconstructionViewDispatcher<nl_static,Tmarker,Tmask,Tout,2, poutre::details::av::array_view, poutre::details::av::array_view, poutre::details::av::array_view, HelperOp>
{
  static_assert(2 == poutre::se::details::static_se_traits<nl_static>::rank, "SE and view have not the same Rank");
  void operator()(const poutre::details::av::array_view<const Tmarker, 2> &i_vmarker,
                  const poutre::details::av::array_view<const Tmask, 2> &i_vmask,
                  poutre::details::av::array_view<Tout, 2> &o_vout) const
  {
    POUTRE_CHECK(i_vmarker.size() == i_vmask.size(), "Incompatible views size");
    POUTRE_CHECK(i_vmask.size() == o_vout.size(), "Incompatible views size");

    // More check
    auto vMarkerbound   = i_vmarker.bound();
    auto vMaskbound   = i_vmask.bound();
    auto vOutbound  = o_vout.bound();
    auto stridevMarker  = i_vmarker.stride();
    auto stridevMask  = i_vmask.stride();
    auto stridevOut = o_vout.stride();
    POUTRE_CHECK(vMarkerbound==vMaskbound, "Incompatible bound");
    POUTRE_CHECK(vOutbound==vMaskbound, "Incompatible bound");

    POUTRE_CHECK(stridevMarker==stridevMask, "Incompatible stride");
    POUTRE_CHECK(stridevMask==stridevOut, "Incompatible stride");

    scoord ysize   = vMarkerbound[0];
    scoord xsize   = vMarkerbound[1];

    //auto      ptrMarkerbeg  = i_vmarker.data();
    auto      ptrMaskbeg      = i_vmask.data();
    auto      ptrOutbeg             = o_vout.data();

    // constexpr auto nl_coord     = poutre::se::details::static_se_traits<nl_static>::coordinates;
    auto [nl_upper, nl_lower]   = poutre::se::details::static_se_traits<nl_static>::split_coordinates_upper_lower();
    std::queue<poutre::details::av::index<2>> queue;

    HelperOp::select_marker(i_vmarker, i_vmask, o_vout);

    // temp display
    {
      std::cout<<"After clamp \n";
      auto begit = begin(vMarkerbound);
      auto endit = end(vMarkerbound);
      for( ; begit != endit; ++begit ) {
        std::cout << static_cast<int>(o_vout[*begit]) << " ";
      }
      std::cout<<"\n";
    }
    // forward scan
    for( ptrdiff_t y = 0; y < ysize; ++y ) {
      for( ptrdiff_t x = 0; x < xsize ; ++x ) {
        scoord const centerx = x;
        scoord const centery = y;
        auto curr_val = ptrOutbeg[(xsize * centery) + centerx];
        const poutre::details::av::idx2d center{y, x};
        for( const auto &idx_nl_upper : nl_upper ) {
          scoord const currentx = center[1] + idx_nl_upper[1];
          scoord const currenty = center[0] + idx_nl_upper[0];
          if (currentx<0 || currentx>=xsize || currenty<0 || currenty>=ysize) {continue;}
          std::cout<<"Forward loop curval"<< static_cast<int>(curr_val)<< " " <<static_cast<int>(ptrOutbeg[(xsize * currenty) + currentx])<<"\n" ;
          curr_val = HelperOp::select_se(curr_val, ptrOutbeg[(xsize * currenty) + currentx]);
        }
        std::cout<<"Forward loop out "<< static_cast<int>(curr_val)<< " " <<static_cast<int>(ptrMaskbeg[(xsize * centery) + centerx])<<"\n" ;
        ptrOutbeg[(xsize * centery) + centerx] = HelperOp::select_marker(curr_val, ptrMaskbeg[(xsize * centery) + centerx]);

        {
          std::cout<<"Forward loop \n";
          auto begit = begin(vMarkerbound);
          auto endit = end(vMarkerbound);
          for( ; begit != endit; ++begit ) {
            std::cout << static_cast<int>(o_vout[*begit]) << " ";
          }
          std::cout<<"\n";
        }
      }
    }
    //backward scan
    for( ptrdiff_t y = ysize-1; y >= 0; --y ) {
      for( ptrdiff_t x = xsize-1; x >= 0 ; --x ) {
        scoord const centerx = x;
        scoord const centery = y;
        auto curr_val = ptrOutbeg[(xsize * centery) + centerx];
        const poutre::details::av::idx2d center{y, x};
        for( const auto &idx_nl_lower : nl_lower )
        {
          scoord const currentx = center[1] + idx_nl_lower[1];
          scoord const currenty = center[0] + idx_nl_lower[0];
          if (currentx<0 || currentx>=xsize || currenty<0 || currenty>=ysize) {continue;}
          curr_val = HelperOp::select_se(curr_val, ptrOutbeg[(xsize * currenty) + currentx]);
        }
        ptrOutbeg[(xsize * centery) + centerx] = HelperOp::select_marker(curr_val, ptrMaskbeg[(xsize * centery) + centerx]);
        for( const auto &idx_nl_lower : nl_lower )
        {
          scoord const currentx = center[1] + idx_nl_lower[1];
          scoord const currenty = center[0] + idx_nl_lower[0];
          if (currentx<0 || currentx>=xsize || currenty<0 || currenty>=ysize) {continue;}
          if (
          HelperOp::should_enqueue(ptrOutbeg[(xsize * currenty) + currentx], ptrOutbeg[(xsize * centery) + centerx])
          && HelperOp::should_enqueue(ptrOutbeg[(xsize * currenty) + currentx], ptrMaskbeg[(xsize * currenty) + currentx])) {
            queue.push(center);
          }
        }
      }
    }

    auto beg1 = begin(vMarkerbound);
    auto end1 = end(vMarkerbound);
    for( ; beg1 != end1; ++beg1 )
    {
      auto curr_val = o_vout[*beg1];
      for( const auto &idx_nl_upper : nl_upper )
      {
        const auto delta_nl_idx = *beg1 + idx_nl_upper;
        if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
        curr_val = HelperOp::select_se(curr_val, o_vout[delta_nl_idx]);
      }
      o_vout[*beg1] = HelperOp::select_marker(curr_val, i_vmask[*beg1]);
    }

    //backward scan
    auto rbeg1 = rbegin(vMarkerbound);
    auto rend1 = rend(vMarkerbound);
    for( ; rbeg1 != rend1; ++rbeg1 ) {
      auto curr_val = o_vout[*rbeg1];
      for( const auto &idx_nl_lower : nl_lower ) {
        auto delta_nl_idx = *rbeg1 + idx_nl_lower;
        if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
        curr_val = HelperOp::select_se(curr_val, o_vout[delta_nl_idx]);
      }
      o_vout[*rbeg1] = HelperOp::select_marker(curr_val, i_vmask[*rbeg1]);

      // temp display
      {
        std::cout<<"Backward loop \n";
        auto begit = begin(vMarkerbound);
        auto endit = end(vMarkerbound);
        for( ; begit != endit; ++begit ) {
          std::cout << static_cast<int>(o_vout[*begit]) << " ";
        }
        std::cout<<"\n";
      }

      for( const auto &idx_nl_lower : nl_lower ) {
        auto delta_nl_idx = *rbeg1 + idx_nl_lower;
        if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
        if (
          HelperOp::should_enqueue(o_vout[delta_nl_idx], o_vout[*rbeg1])
          && HelperOp::should_enqueue(o_vout[delta_nl_idx], i_vmask[delta_nl_idx])) {
          queue.push(*rbeg1);
        }
    }
  }
  while( !queue.empty() ) {
    auto idx = queue.front();
    queue.pop();
    for( const auto &idx_nl : nl_coord ) {
      auto delta_nl_idx = idx + idx_nl;
      if( !vMarkerbound.contains(delta_nl_idx) ) {continue;}
      if (
        HelperOp::should_enqueue(o_vout[delta_nl_idx],o_vout[idx]
          && i_vmask[delta_nl_idx] != o_vout[delta_nl_idx])) {
        o_vout[delta_nl_idx] = HelperOp::select_marker(o_vout[idx], i_vmask[delta_nl_idx]);
        queue.push(delta_nl_idx);
      }
    }
  }
  }
};
*/

template<poutre::se::Common_NL_SE nl_static,
          typename Tmarker,
          typename Tmask,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewMarker,
          template<typename, ptrdiff_t>
          class ViewMask,
          template<typename, ptrdiff_t>
          class ViewOut>
void t_ReconstructionDispatchRectType(
    reconstruction_type rect_type,
    const ViewMarker<const Tmarker, Rank> &i_vmarker,
    const ViewMask<const Tmask, Rank> &i_vmask,
    ViewOut<Tout, Rank> &o_vout)
{
  switch(rect_type) {
  case reconstruction_type::erode: {
    using HelperOp = OpRecErode<Tmarker,Tmask,Tout, Rank,ViewMarker, ViewMask, ViewOut>;
    t_ReconstructionViewDispatcher<nl_static, Tmarker,Tmask, Tout, Rank, ViewMarker, ViewMask, ViewOut, HelperOp> dispatcher;
    dispatcher(i_vmarker, i_vmask, o_vout);
  }break;
  case reconstruction_type::dilate: {
    using HelperOp = OpRecDilate<Tmarker,Tmask,Tout, Rank,ViewMarker, ViewMask, ViewOut>;
    t_ReconstructionViewDispatcher<nl_static, Tmarker,Tmask, Tout, Rank, ViewMarker, ViewMask, ViewOut, HelperOp> dispatcher;
    dispatcher(i_vmarker, i_vmask, o_vout);
  }break;
  // case reconstruction_type::dual: {
  //   using HelperOp = OpRecDilate<Tmarker,Tmask,Tout, Rank,ViewMarker, ViewMask, ViewOut>;
  //   t_ReconstructionViewDispatcher<nl_static, Tmarker,Tmask, Tout, Rank, ViewMarker, ViewMask, ViewOut, HelperOp> dispatcher;
  //   dispatcher(i_vmarker, i_vmask, o_vout);
  // }break;
  default: {
    POUTRE_RUNTIME_ERROR("t_ReconstructionDispatchRectType unsupported reconstruction_type");
  }
  }
}


template<
          typename Tmarker,
          typename Tmask,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewMarker,
          template<typename, ptrdiff_t>
          class ViewMask,
          template<typename, ptrdiff_t>
          class ViewOut>
void t_ReconstructionDispatch(
    reconstruction_type rect_type,
    const ViewMarker<const Tmarker, Rank> &i_vmarker,
    const ViewMask<const Tmask, Rank> &i_vmask,
    const poutre::se::Common_NL_SE nl_static,
    ViewOut<Tout, Rank> &o_vout)
{
  POUTRE_CHECK(i_vmarker.size() == i_vmask.size(), "t_ReconstructionDispatchNl Incompatible views size");
  POUTRE_CHECK(i_vmask.size() == o_vout.size(), "t_ReconstructionDispatchNl Incompatible views size");;

  if constexpr (Rank == 1) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SESegmentX1D: {
      //<poutre::se::Common_NL_SE::SESegmentX1D, Tmarker,Tmask,Tout, Rank, ViewMarker,ViewMask, ViewOut> dispatcher;
      t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESegmentX1D>(rect_type, i_vmarker,i_vmask, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_ReconstructionDispatchNl unsupported nl_static");}
    }
    return;
  }
  if constexpr (Rank == 2) {
    switch(nl_static) {
    // case poutre::se::Common_NL_SE::SESegmentX2D: {
    //   //using dispatcher = t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESegmentX2D, Tmarker,Tmask,Tout, Rank, ViewMarker,ViewMask, ViewOut>;
    //   t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESegmentX2D>(rect_type, i_vmarker,i_vmask, o_vout);
    // } break;
    // case poutre::se::Common_NL_SE::SESegmentY2D: {
    //   // t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESegmentY2D, Tmarker,Tmask,Tout, Rank, ViewMarker,ViewMask, ViewOut> dispatcher;
    //   t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESegmentY2D>(rect_type, i_vmarker,i_vmask, o_vout);
    // } break;
    case poutre::se::Common_NL_SE::SESquare2D: {
      //t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESquare2D, Tmarker,Tmask,Tout, Rank, ViewMarker,ViewMask, ViewOut> dispatcher;
      t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESquare2D>(rect_type, i_vmarker,i_vmask, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SECross2D: {
      t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SECross2D>(rect_type, i_vmarker,i_vmask, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_ReconstructionDispatchNl unsupported nl_static");}
    }
    //return;
  }
  if constexpr (Rank == 3) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SECross3D: {
      t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SECross3D>(rect_type, i_vmarker,i_vmask, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare3D: {
      t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESquare3D>(rect_type, i_vmarker,i_vmask, o_vout);
    } break;
    // case poutre::se::Common_NL_SE::SESegmentX3D: {
    //   t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESegmentX3D>(rect_type, i_vmarker,i_vmask, o_vout);
    // } break;
    // case poutre::se::Common_NL_SE::SESegmentY3D: {
    //   t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESegmentY3D>(rect_type, i_vmarker,i_vmask, o_vout);
    // } break;
    // case poutre::se::Common_NL_SE::SESegmentZ3D: {
    //   t_ReconstructionDispatchRectType<poutre::se::Common_NL_SE::SESegmentZ3D>(rect_type, i_vmarker,i_vmask, o_vout);
    // } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_ReconstructionDispatchNl unsupported nl_static");}
    }
    //return;
  }
}

template<typename Tmarker, typename Tmask, typename Tout, ptrdiff_t Rank>
void t_Reconstruct(
    reconstruction_type rect_type,
    const poutre::details::image_t<Tmarker, Rank> &i_marker,
    const poutre::details::image_t<Tmask, Rank> &i_mask,
    poutre::se::Common_NL_SE nl_static,
    poutre::details::image_t<Tout, Rank> &o_img)
{
  AssertSizesCompatible(i_marker, o_img, "t_Reconstruct incompatible size");
  AssertSizesCompatible(i_mask, o_img, "t_Reconstruct incompatible size");

  AssertAsTypesCompatible(i_marker, o_img, "t_Reconstruct incompatible types");
  AssertAsTypesCompatible(i_mask, o_img, "t_Reconstruct incompatible types");

  AssertImagesAreDifferent(i_marker, o_img, "t_Reconstruct output must be != than input images");
  AssertImagesAreDifferent(i_mask, o_img, "t_Reconstruct output must be != than input images");

  auto viewMarker= view(i_marker);
  auto viewMask= view(i_mask);
  auto viewOut = view(o_img);
  return t_ReconstructionDispatch( rect_type,viewMarker,viewMask, nl_static, viewOut);
}

//! @} doxygroup: poutre_geo_group
}//poutre::geo::details