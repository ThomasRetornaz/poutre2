
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
#include <poutre/distance/details/mdistance_t.hpp>
#include <poutre/distance/mdistance.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace {
template<std::ptrdiff_t NumDims, poutre::PType P>
void MDistanceImageDispatch(const poutre::IInterface &i_marker,// NOLINT
  poutre::se::Common_NL_SE nl_static,
  poutre::IInterface &o_img)
{
  using ImgType =
    poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type,
      NumDims>;
  const auto *imgin_t = dynamic_cast<const ImgType *>(&i_marker);
  if (!imgin_t) { POUTRE_RUNTIME_ERROR("MDistanceImageDispatch i_marker downcast fail"); }
  using OImgType = poutre::details::image_t<
    typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_GrayINT64>::type,
    NumDims>;
  auto *imgout_t = dynamic_cast<OImgType *>(&o_img);
  if (!imgout_t) { POUTRE_RUNTIME_ERROR("MDistanceImageDispatch o_img downcast fail, we expect PType_GrayINT64"); }

  poutre::dist::details::t_mdistance(*imgin_t, nl_static, *imgout_t);
}
}// namespace

namespace poutre::dist {

void mdistance(const IInterface &i_img, se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("mdistance");
  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("mdistance Unsupported number of dims:0");
  }
  case 1: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      MDistanceImageDispatch<1, poutre::PType::PType_GrayUINT8>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_GrayINT32: {
      MDistanceImageDispatch<1, poutre::PType::PType_GrayINT32>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_GrayINT64: {
      MDistanceImageDispatch<1, poutre::PType::PType_GrayINT64>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_F32: {
      MDistanceImageDispatch<1, poutre::PType::PType_F32>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_D64: {
      MDistanceImageDispatch<1, poutre::PType::PType_D64>(i_img, nl_static, o_img);
      break;
    }
    default: {
      POUTRE_RUNTIME_ERROR("mdistance unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      MDistanceImageDispatch<2, poutre::PType::PType_GrayUINT8>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_GrayINT32: {
      MDistanceImageDispatch<2, poutre::PType::PType_GrayINT32>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_GrayINT64: {
      MDistanceImageDispatch<2, poutre::PType::PType_GrayINT64>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_F32: {
      MDistanceImageDispatch<2, poutre::PType::PType_F32>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_D64: {
      MDistanceImageDispatch<2, poutre::PType::PType_D64>(i_img, nl_static, o_img);
      break;
    }
    default: {
      POUTRE_RUNTIME_ERROR("mdistance unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      MDistanceImageDispatch<3, poutre::PType::PType_GrayUINT8>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_GrayINT32: {
      MDistanceImageDispatch<3, poutre::PType::PType_GrayINT32>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_GrayINT64: {
      MDistanceImageDispatch<3, poutre::PType::PType_GrayINT64>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_F32: {
      MDistanceImageDispatch<3, poutre::PType::PType_F32>(i_img, nl_static, o_img);
      break;
    }
    case poutre::PType::PType_D64: {
      MDistanceImageDispatch<3, poutre::PType::PType_D64>(i_img, nl_static, o_img);
      break;
    }
    default: {
      POUTRE_RUNTIME_ERROR("mdistance unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // }
  default: {
    POUTRE_RUNTIME_ERROR("mdistance Unsupported number of dims");
  }
  }
}
}// namespace poutre::dist
