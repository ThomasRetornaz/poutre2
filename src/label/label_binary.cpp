
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
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/label/label_binary.hpp>
#include <poutre/label/details/label_t.hpp>

namespace {
template<ptrdiff_t NumDims, poutre::PType P> size_t label_binaryImageDispatch(
    const poutre::IInterface &i_img,
    poutre::se::Common_NL_SE nl_static,
    poutre::IInterface &o_img)
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *imgin_t = dynamic_cast<const ImgType *>(&i_img);
  if (!imgin_t) { POUTRE_RUNTIME_ERROR("label_binaryImageDispatch i_ref downcast fail"); }
  using OImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_GrayINT64>::type, NumDims>;
  auto *imgout_t = dynamic_cast<OImgType *>(&o_img);
  if (!imgout_t) { POUTRE_RUNTIME_ERROR("label_binaryImageDispatch o_img downcast fail"); }

  return poutre::label::details::t_label_binary(*imgin_t, nl_static, *imgout_t);
}
}
namespace poutre::label {

size_t label_binary(const IInterface &i_img,  se::Common_NL_SE nl_static, IInterface &o_img)
{
  POUTRE_ENTERING("label_binary");
  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("label_binary Unsupported number of dims:0");
  }
  case 1: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      return label_binaryImageDispatch<1, poutre::PType::PType_GrayUINT8>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_GrayINT32: {
      return label_binaryImageDispatch<1, poutre::PType::PType_GrayINT32>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_GrayINT64: {
      return label_binaryImageDispatch<1, poutre::PType::PType_GrayINT64>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_F32: {
      return label_binaryImageDispatch<1, poutre::PType::PType_F32>(i_img, nl_static, o_img);
    } break;
    case poutre::PType::PType_D64: {
      return label_binaryImageDispatch<1, poutre::PType::PType_D64>(i_img, nl_static, o_img);
    }
    default: {
      POUTRE_RUNTIME_ERROR("label_binary unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      return label_binaryImageDispatch<2, poutre::PType::PType_GrayUINT8>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_GrayINT32: {
      return label_binaryImageDispatch<2, poutre::PType::PType_GrayINT32>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_GrayINT64: {
      return label_binaryImageDispatch<2, poutre::PType::PType_GrayINT64>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_F32: {
      return label_binaryImageDispatch<2, poutre::PType::PType_F32>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_D64: {
      return label_binaryImageDispatch<2, poutre::PType::PType_D64>(i_img, nl_static, o_img);
    }
    default: {
      POUTRE_RUNTIME_ERROR("label_binary unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      return label_binaryImageDispatch<3, poutre::PType::PType_GrayUINT8>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_GrayINT32: {
      return label_binaryImageDispatch<3, poutre::PType::PType_GrayINT32>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_GrayINT64: {
      return label_binaryImageDispatch<3, poutre::PType::PType_GrayINT64>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_F32: {
      return label_binaryImageDispatch<3, poutre::PType::PType_F32>(i_img, nl_static, o_img);
    }
    case poutre::PType::PType_D64: {
      return label_binaryImageDispatch<3, poutre::PType::PType_D64>(i_img, nl_static, o_img);
    }
    default: {
      POUTRE_RUNTIME_ERROR("label_binary unsupported PTYPE");
    }
    }
  }
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("label_binary Unsupported number of dims");
  }
  }
}
}