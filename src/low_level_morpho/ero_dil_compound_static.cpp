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
#include <poutre/base/types.hpp>
#include <poutre/base/types_traits.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/low_level_morpho/ero_dil.hpp>
#include <poutre/low_level_morpho/details/ero_dil_compound_static_se_t.hpp>

namespace {
template<ptrdiff_t NumDims, poutre::PType P> void ErodeImageDispatch(const poutre::IInterface &i_img, const poutre::se::Compound_NL_SE compound_nl, const int size, poutre::IInterface &o_img)
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ErodeImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ErodeImageDispatch img2_t downcast fail"); }
  poutre::llm::details::t_Erode(*img1_t,compound_nl,size,*img2_t);
}

template<ptrdiff_t NumDims, poutre::PType P> void DilateImageDispatch(const poutre::IInterface &i_img,  const poutre::se::Compound_NL_SE compound_nl, const int size, poutre::IInterface &o_img)
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("DilateImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("DilateImageDispatch img2_t downcast fail"); }
  poutre::llm::details::t_Dilate(*img1_t,compound_nl,size,*img2_t);
}
}
namespace poutre {

void Erode(const IInterface &i_img, se::Compound_NL_SE nl_compound, const int size, IInterface &o_img)
{
  AssertSizesCompatible(i_img, o_img, "Erode images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "Erode images must have compatible types");
  AssertImagesAreDifferent(i_img, o_img, "Erode images input output images must be different");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Erode Unsupported number of dims:0");
  }
  case 1: {
    POUTRE_RUNTIME_ERROR("Erode Unsupported number of dims:1");
  };
  case 2: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      ErodeImageDispatch<2, poutre::PType::PType_GrayUINT8>(i_img, nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      ErodeImageDispatch<2, poutre::PType::PType_GrayINT32>(i_img,nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      ErodeImageDispatch<2, poutre::PType::PType_GrayINT64>(i_img,nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_F32: {
      ErodeImageDispatch<2, poutre::PType::PType_F32>(i_img, nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_D64: {
      ErodeImageDispatch<2, poutre::PType::PType_D64>(i_img,nl_compound, size,o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Erode unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      ErodeImageDispatch<3, poutre::PType::PType_GrayUINT8>(i_img, nl_compound, size,o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      ErodeImageDispatch<3, poutre::PType::PType_GrayINT32>(i_img,nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      ErodeImageDispatch<3, poutre::PType::PType_GrayINT64>(i_img,nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_F32: {
      ErodeImageDispatch<3, poutre::PType::PType_F32>(i_img, nl_compound, size,o_img);
    } break;
    case poutre::PType::PType_D64: {
      ErodeImageDispatch<3, poutre::PType::PType_D64>(i_img,nl_compound, size, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Erode unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("Erode Unsupported number of dims");
  }
  }
}

void Dilate(const IInterface &i_img, se::Compound_NL_SE nl_compound, const int size, IInterface &o_img)
{
  AssertSizesCompatible(i_img, o_img, "Dilate images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "Dilate images must have compatible types");
  AssertImagesAreDifferent(i_img, o_img, "Dilate images input output images must be different");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Dilate Unsupported number of dims:0");
  }
  case 1: {
    POUTRE_RUNTIME_ERROR("Dilate Unsupported number of dims:1");
  };
  case 2: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      DilateImageDispatch<2, poutre::PType::PType_GrayUINT8>(i_img, nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      DilateImageDispatch<2, poutre::PType::PType_GrayINT32>(i_img,nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      DilateImageDispatch<2, poutre::PType::PType_GrayINT64>(i_img,nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_F32: {
      DilateImageDispatch<2, poutre::PType::PType_F32>(i_img, nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_D64: {
      DilateImageDispatch<2, poutre::PType::PType_D64>(i_img,nl_compound, size,o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Dilate unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      DilateImageDispatch<3, poutre::PType::PType_GrayUINT8>(i_img, nl_compound, size,o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      DilateImageDispatch<3, poutre::PType::PType_GrayINT32>(i_img,nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      DilateImageDispatch<3, poutre::PType::PType_GrayINT64>(i_img,nl_compound, size, o_img);
    } break;
    case poutre::PType::PType_F32: {
      DilateImageDispatch<3, poutre::PType::PType_F32>(i_img, nl_compound, size,o_img);
    } break;
    case poutre::PType::PType_D64: {
      DilateImageDispatch<3, poutre::PType::PType_D64>(i_img,nl_compound, size, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Dilate unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("Dilate Unsupported number of dims");
  }
  }
}

}