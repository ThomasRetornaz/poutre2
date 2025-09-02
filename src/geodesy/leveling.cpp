
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
#include <poutre/pixel_processing/copy_convert.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/geodesy/leveling.hpp>
#include <poutre/geodesy/details/leveling_t.hpp>

namespace {
template<ptrdiff_t NumDims, poutre::PType P> void lowLevelingImageDispatch(
  const poutre::IInterface &i_ref, // NOLINT
  const poutre::IInterface &i_marker,   // NOLINT
  poutre::se::Common_NL_SE nl_static,
  poutre::IInterface &o_img
  )
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *imgmarker_t = dynamic_cast<const ImgType *>(&i_marker);
  if (!imgmarker_t) { POUTRE_RUNTIME_ERROR("lowLevelingImageDispatch i_marker downcast fail"); }
  const auto *imgref_t = dynamic_cast<const ImgType *>(&i_ref);
  if (!imgref_t) { POUTRE_RUNTIME_ERROR("lowLevelingImageDispatch i_ref downcast fail"); }
  auto *imgout_t = dynamic_cast<ImgType *>(&o_img);
  if (!imgout_t) { POUTRE_RUNTIME_ERROR("lowLevelingImageDispatch o_img downcast fail"); }

  poutre::geo::details::t_low_leveling(*imgref_t, *imgmarker_t, nl_static, *imgout_t);
}

template<ptrdiff_t NumDims, poutre::PType P> void highLevelingImageDispatch(
  const poutre::IInterface &i_ref, // NOLINT
  const poutre::IInterface &i_marker,   // NOLINT
  poutre::se::Common_NL_SE nl_static,
  poutre::IInterface &o_img
  )
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *imgmarker_t = dynamic_cast<const ImgType *>(&i_marker);
  if (!imgmarker_t) { POUTRE_RUNTIME_ERROR("highLevelingImageDispatch i_marker downcast fail"); }
  const auto *imgref_t = dynamic_cast<const ImgType *>(&i_ref);
  if (!imgref_t) { POUTRE_RUNTIME_ERROR("highLevelingImageDispatch i_ref downcast fail"); }
  auto *imgout_t = dynamic_cast<ImgType *>(&o_img);
  if (!imgout_t) { POUTRE_RUNTIME_ERROR("highLevelingImageDispatch o_img downcast fail"); }

  poutre::geo::details::t_high_leveling(*imgref_t, *imgmarker_t, nl_static, *imgout_t);
}
}

namespace poutre::geo {
void low_leveling(const IInterface &i_ref, const IInterface &i_marker, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("low_leveling");
  switch (i_ref.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("low_leveling Unsupported number of dims:0");
  }
  case 1: {
    switch (i_marker.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      lowLevelingImageDispatch<1, poutre::PType::PType_GrayUINT8>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      lowLevelingImageDispatch<1, poutre::PType::PType_GrayINT32>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      lowLevelingImageDispatch<1, poutre::PType::PType_GrayINT64>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      lowLevelingImageDispatch<1, poutre::PType::PType_F32>(i_ref, i_marker,nl_static, o_img);
    } break;
    case poutre::PType::PType_D64: {
      lowLevelingImageDispatch<1, poutre::PType::PType_D64>(i_ref, i_marker, nl_static, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("low_leveling unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    switch (i_marker.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      lowLevelingImageDispatch<2, poutre::PType::PType_GrayUINT8>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      lowLevelingImageDispatch<2, poutre::PType::PType_GrayINT32>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      lowLevelingImageDispatch<2, poutre::PType::PType_GrayINT64>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      lowLevelingImageDispatch<2, poutre::PType::PType_F32>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_D64: {
      lowLevelingImageDispatch<2, poutre::PType::PType_D64>(i_ref, i_marker, nl_static, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("low_leveling unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_marker.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      lowLevelingImageDispatch<3, poutre::PType::PType_GrayUINT8>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      lowLevelingImageDispatch<3, poutre::PType::PType_GrayINT32>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      lowLevelingImageDispatch<3, poutre::PType::PType_GrayINT64>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      lowLevelingImageDispatch<3, poutre::PType::PType_F32>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_D64: {
      lowLevelingImageDispatch<3, poutre::PType::PType_D64>(i_ref, i_marker, nl_static, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("low_leveling unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("low_leveling Unsupported number of dims");
  }
  }
}

void high_leveling(const IInterface &i_ref, const IInterface &i_marker, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("high_leveling");
  switch (i_ref.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("high_leveling Unsupported number of dims:0");
  }
  case 1: {
    switch (i_marker.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      highLevelingImageDispatch<1, poutre::PType::PType_GrayUINT8>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      highLevelingImageDispatch<1, poutre::PType::PType_GrayINT32>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      highLevelingImageDispatch<1, poutre::PType::PType_GrayINT64>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      highLevelingImageDispatch<1, poutre::PType::PType_F32>(i_ref, i_marker,nl_static, o_img);
    } break;
    case poutre::PType::PType_D64: {
      highLevelingImageDispatch<1, poutre::PType::PType_D64>(i_ref, i_marker, nl_static, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("high_leveling unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    switch (i_marker.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      highLevelingImageDispatch<2, poutre::PType::PType_GrayUINT8>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      highLevelingImageDispatch<2, poutre::PType::PType_GrayINT32>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      highLevelingImageDispatch<2, poutre::PType::PType_GrayINT64>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      highLevelingImageDispatch<2, poutre::PType::PType_F32>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_D64: {
      highLevelingImageDispatch<2, poutre::PType::PType_D64>(i_ref, i_marker, nl_static, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("high_leveling unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_marker.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      highLevelingImageDispatch<3, poutre::PType::PType_GrayUINT8>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      highLevelingImageDispatch<3, poutre::PType::PType_GrayINT32>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      highLevelingImageDispatch<3, poutre::PType::PType_GrayINT64>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      highLevelingImageDispatch<3, poutre::PType::PType_F32>(i_ref, i_marker, nl_static, o_img);
    } break;
    case poutre::PType::PType_D64: {
      highLevelingImageDispatch<3, poutre::PType::PType_D64>(i_ref, i_marker, nl_static, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("high_leveling unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("high_leveling Unsupported number of dims");
  }
  }
}

void leveling(const IInterface &i_ref, const IInterface &i_marker, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("leveling");
  auto img_tmp = poutre::CloneGeometry(i_ref); // NOLINT
  high_leveling(i_ref,i_marker,nl_static,*img_tmp);
  low_leveling(*img_tmp,i_marker,nl_static,o_img);
}

}