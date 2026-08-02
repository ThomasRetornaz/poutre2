
// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <cstddef>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/base/types.hpp>
#include <poutre/base/types_traits.hpp>
#include <poutre/distance/details/geodesic_distance_t.hpp>
#include <poutre/distance/geodesic_distance.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace {
template< std::ptrdiff_t NumDims, poutre::PType P >
void GeodesicDistanceImageDispatch(const poutre::IInterface &i_marker,// NOLINT
                                   const poutre::IInterface &i_mask,// NOLINT
                                   poutre::se::Common_NL_SE nl_static,
                                   poutre::IInterface &o_img)
{
  using ImgType =
    poutre::details::image_t< typename poutre::enum_to_type< poutre::CompoundType::CompoundType_Scalar, P >::type,
                              NumDims >;
  const auto *imgmarker_t = dynamic_cast< const ImgType * >(&i_marker);
  if (!imgmarker_t) {
    POUTRE_RUNTIME_ERROR("GeodesicDistanceImageDispatch i_marker downcast fail");
  }
  const auto *imgmask_t = dynamic_cast< const ImgType * >(&i_mask);
  if (!imgmask_t) {
    POUTRE_RUNTIME_ERROR("GeodesicDistanceImageDispatch i_mask downcast fail");
  }
  auto *imgout_t = dynamic_cast< ImgType * >(&o_img);
  if (!imgout_t) {
    POUTRE_RUNTIME_ERROR("GeodesicDistanceImageDispatch o_img downcast fail");
  }

  poutre::dist::details::t_geodesic_distance(*imgmarker_t, *imgmask_t, nl_static, *imgout_t);
}
}// namespace

namespace poutre::dist {
void geodesic_distance(const IInterface &i_marker,
                       const IInterface &i_mask,
                       se::Common_NL_SE nl_static,
                       IInterface &o_img)
{
  POUTRE_ENTERING("geodesic_distance");
  switch (i_marker.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("geodesic_distance Unsupported number of dims:0");
  }
  case 1: {
    switch (i_marker.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      GeodesicDistanceImageDispatch< 1, poutre::PType::PType_GrayUINT8 >(i_marker, i_mask, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      GeodesicDistanceImageDispatch< 1, poutre::PType::PType_GrayINT32 >(i_marker, i_mask, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      GeodesicDistanceImageDispatch< 1, poutre::PType::PType_GrayINT64 >(i_marker, i_mask, nl_static, o_img);
    } break;
    // case poutre::PType::PType_F32: {
    //   ReconstructionImageDispatch<1, poutre::PType::PType_F32>(rect_type,i_marker, i_mask, nl_static, o_img);
    // } break;
    // case poutre::PType::PType_D64: {
    //   ReconstructionImageDispatch<1, poutre::PType::PType_D64>(rect_type,i_marker, i_mask, nl_static, o_img);
    // } break;
    default: {
      POUTRE_RUNTIME_ERROR("geodesic_distance unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    switch (i_marker.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      GeodesicDistanceImageDispatch< 2, poutre::PType::PType_GrayUINT8 >(i_marker, i_mask, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      GeodesicDistanceImageDispatch< 2, poutre::PType::PType_GrayINT32 >(i_marker, i_mask, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      GeodesicDistanceImageDispatch< 2, poutre::PType::PType_GrayINT64 >(i_marker, i_mask, nl_static, o_img);
    } break;
    // case poutre::PType::PType_F32: {
    //   ReconstructionImageDispatch<2, poutre::PType::PType_F32>(rect_type,i_marker, i_mask, nl_static, o_img);
    // } break;
    // case poutre::PType::PType_D64: {
    //   ReconstructionImageDispatch<2, poutre::PType::PType_D64>(rect_type,i_marker, i_mask, nl_static, o_img);
    // } break;
    default: {
      POUTRE_RUNTIME_ERROR("geodesic_distance unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_marker.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      GeodesicDistanceImageDispatch< 3, poutre::PType::PType_GrayUINT8 >(i_marker, i_mask, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      GeodesicDistanceImageDispatch< 3, poutre::PType::PType_GrayINT32 >(i_marker, i_mask, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      GeodesicDistanceImageDispatch< 3, poutre::PType::PType_GrayINT64 >(i_marker, i_mask, nl_static, o_img);
    } break;
    // case poutre::PType::PType_F32: {
    //   ReconstructionImageDispatch<3, poutre::PType::PType_F32>(rect_type,i_marker, i_mask, nl_static, o_img);
    // } break;
    // case poutre::PType::PType_D64: {
    //   ReconstructionImageDispatch<3, poutre::PType::PType_D64>(rect_type,i_marker, i_mask, nl_static, o_img);
    // } break;
    default: {
      POUTRE_RUNTIME_ERROR("geodesic_distance unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("geodesic_distance Unsupported number of dims");
  }
  }
}
}// namespace poutre::dist