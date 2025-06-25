
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
#include <poutre/low_level_morpho/details/ero_dil_line_se_t.hpp>
#include <poutre/low_level_morpho/ero_dil_line.hpp>

namespace {
template<ptrdiff_t NumDims, poutre::PType P> void ErodeImageLineXDispatch(const poutre::IInterface &i_img, ptrdiff_t size_segment, poutre::IInterface &o_img)
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ErodeImageLineXDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ErodeImageLineXDispatch img2_t downcast fail"); }
  poutre::llm::details::t_ErodeX(*img1_t,size_segment,*img2_t);
}

template<ptrdiff_t NumDims, poutre::PType P> void DilateImageLineXDispatch(const poutre::IInterface &i_img, ptrdiff_t size_segment, poutre::IInterface &o_img)
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("DilateImageLineXDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("DilateImageLineXDispatch img2_t downcast fail"); }
  poutre::llm::details::t_DilateX(*img1_t,size_segment,*img2_t);
}
}

namespace poutre {
void DilateX(const poutre::IInterface &i_img, const ptrdiff_t size_half_segment,  poutre::IInterface &o_img)
{
  POUTRE_ENTERING("DilateX");
  AssertSizesCompatible(i_img, o_img, "DilateX images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "DilateX images must have compatible types");
  AssertImagesAreDifferent(i_img, o_img, "DilateX images input output images must be different");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("DilateX Unsupported number of dims:0");
  }
  case 1: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      DilateImageLineXDispatch<1, poutre::PType::PType_GrayUINT8>(i_img, size_half_segment, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      DilateImageLineXDispatch<1, poutre::PType::PType_GrayINT32>(i_img,size_half_segment, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      DilateImageLineXDispatch<1, poutre::PType::PType_GrayINT64>(i_img,size_half_segment, o_img);
    } break;
    case poutre::PType::PType_F32: {
      DilateImageLineXDispatch<1, poutre::PType::PType_F32>(i_img,size_half_segment, o_img);
    } break;
    case poutre::PType::PType_D64: {
      DilateImageLineXDispatch<1, poutre::PType::PType_D64>(i_img, size_half_segment, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("DilateX unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      DilateImageLineXDispatch<2, poutre::PType::PType_GrayUINT8>(i_img, size_half_segment, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      DilateImageLineXDispatch<2, poutre::PType::PType_GrayINT32>(i_img,size_half_segment, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      DilateImageLineXDispatch<2, poutre::PType::PType_GrayINT64>(i_img,size_half_segment, o_img);
    } break;
    case poutre::PType::PType_F32: {
      DilateImageLineXDispatch<2, poutre::PType::PType_F32>(i_img, size_half_segment,o_img);
    } break;
    case poutre::PType::PType_D64: {
      DilateImageLineXDispatch<2, poutre::PType::PType_D64>(i_img, size_half_segment,o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("DilateX unsupported PTYPE");
    }
    }
  } break;
    // case 4: {
    //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
    // } break;
  default: {
    POUTRE_RUNTIME_ERROR("DilateX Unsupported number of dims");
  }
  }
}

void ErodeX(const poutre::IInterface &i_img,  const ptrdiff_t size_half_segment,  poutre::IInterface &o_img)
{
  POUTRE_ENTERING("ErodeX");
  AssertSizesCompatible(i_img, o_img, "ErodeX images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "ErodeX images must have compatible types");
  AssertImagesAreDifferent(i_img, o_img, "ErodeX images input output images must be different");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("ErodeX Unsupported number of dims:0");
  }
  case 1: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      ErodeImageLineXDispatch<1, poutre::PType::PType_GrayUINT8>(i_img, size_half_segment, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      ErodeImageLineXDispatch<1, poutre::PType::PType_GrayINT32>(i_img,size_half_segment, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      ErodeImageLineXDispatch<1, poutre::PType::PType_GrayINT64>(i_img,size_half_segment, o_img);
    } break;
    case poutre::PType::PType_F32: {
      ErodeImageLineXDispatch<1, poutre::PType::PType_F32>(i_img,size_half_segment, o_img);
    } break;
    case poutre::PType::PType_D64: {
      ErodeImageLineXDispatch<1, poutre::PType::PType_D64>(i_img, size_half_segment, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ErodeX unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      ErodeImageLineXDispatch<2, poutre::PType::PType_GrayUINT8>(i_img, size_half_segment, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      ErodeImageLineXDispatch<2, poutre::PType::PType_GrayINT32>(i_img,size_half_segment, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      ErodeImageLineXDispatch<2, poutre::PType::PType_GrayINT64>(i_img,size_half_segment, o_img);
    } break;
    case poutre::PType::PType_F32: {
      ErodeImageLineXDispatch<2, poutre::PType::PType_F32>(i_img, size_half_segment,o_img);
    } break;
    case poutre::PType::PType_D64: {
      ErodeImageLineXDispatch<2, poutre::PType::PType_D64>(i_img, size_half_segment,o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ErodeX unsupported PTYPE");
    }
    }
  } break;
    // case 4: {
    //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
    // } break;
  default: {
    POUTRE_RUNTIME_ERROR("ErodeX Unsupported number of dims");
  }
  }
}
}