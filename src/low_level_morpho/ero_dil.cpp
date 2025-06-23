
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
#include <poutre/structuring_element/se_interface.hpp>
#include <poutre/structuring_element/details/neighbor_list_se_t.hpp>
#include <poutre/low_level_morpho/ero_dil.hpp>
#include <poutre/low_level_morpho/details/ero_dil_runtime_nl_se_t.hpp>

namespace {
template<ptrdiff_t NumDims, poutre::PType P> void ErodeImageRuntimeNLDispatch(const poutre::IInterface &i_img,const poutre::se::details::neighbor_list_t<NumDims>& nl_runtime, poutre::IInterface &o_img)
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ErodeImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ErodeImageDispatch img2_t downcast fail"); }
  poutre::llm::details::t_Erode(*img1_t,nl_runtime,*img2_t);
}

template<ptrdiff_t NumDims, poutre::PType P> void DilateImageRuntimeNLDispatch(const poutre::IInterface &i_img, const poutre::se::details::neighbor_list_t<NumDims>& nl_runtime, poutre::IInterface &o_img)
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("DilateImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("DilateImageDispatch img2_t downcast fail"); }
  poutre::llm::details::t_Dilate(*img1_t,nl_runtime,*img2_t);
}
}

namespace poutre {
void Dilate(const poutre::IInterface &i_img, const se::IStructuringElement& str_el,  poutre::IInterface &o_img)
{
  POUTRE_ENTERING("Dilate IStructuringElement");
  AssertSizesCompatible(i_img, o_img, "Dilate images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "Dilate images must have compatible types");
  AssertImagesAreDifferent(i_img, o_img, "Dilate images input output images must be different");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Dilate Unsupported number of dims:0");
  }
  case 1: {
    const auto *const strel_ptr_t = dynamic_cast<const se::details::neighbor_list_t< 1 >*>(&str_el);
    if (strel_ptr_t == nullptr) {
      POUTRE_RUNTIME_ERROR("Dilate Unsupported IStructuringElement type");
    }
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      DilateImageRuntimeNLDispatch<1, poutre::PType::PType_GrayUINT8>(i_img, *strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      DilateImageRuntimeNLDispatch<1, poutre::PType::PType_GrayINT32>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      DilateImageRuntimeNLDispatch<1, poutre::PType::PType_GrayINT64>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_F32: {
      DilateImageRuntimeNLDispatch<1, poutre::PType::PType_F32>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_D64: {
      DilateImageRuntimeNLDispatch<1, poutre::PType::PType_D64>(i_img, *strel_ptr_t, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Dilate unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    const auto *const strel_ptr_t = dynamic_cast<const se::details::neighbor_list_t< 2 >*>(&str_el);
    if (strel_ptr_t == nullptr) {
      POUTRE_RUNTIME_ERROR("Dilate Unsupported IStructuringElement type");
    }

    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      DilateImageRuntimeNLDispatch<2, poutre::PType::PType_GrayUINT8>(i_img, *strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      DilateImageRuntimeNLDispatch<2, poutre::PType::PType_GrayINT32>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      DilateImageRuntimeNLDispatch<2, poutre::PType::PType_GrayINT64>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_F32: {
      DilateImageRuntimeNLDispatch<2, poutre::PType::PType_F32>(i_img, *strel_ptr_t,o_img);
    } break;
    case poutre::PType::PType_D64: {
      DilateImageRuntimeNLDispatch<2, poutre::PType::PType_D64>(i_img, *strel_ptr_t,o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Dilate unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    const auto *const strel_ptr_t = dynamic_cast<const se::details::neighbor_list_t< 3 >*>(&str_el);
    if (strel_ptr_t == nullptr) {
      POUTRE_RUNTIME_ERROR("Dilate Unsupported IStructuringElement type");
    }
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      DilateImageRuntimeNLDispatch<3, poutre::PType::PType_GrayUINT8>(i_img, *strel_ptr_t,o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      DilateImageRuntimeNLDispatch<3, poutre::PType::PType_GrayINT32>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      DilateImageRuntimeNLDispatch<3, poutre::PType::PType_GrayINT64>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_F32: {
      DilateImageRuntimeNLDispatch<3, poutre::PType::PType_F32>(i_img, *strel_ptr_t,o_img);
    } break;
    case poutre::PType::PType_D64: {
      DilateImageRuntimeNLDispatch<3, poutre::PType::PType_D64>(i_img,*strel_ptr_t, o_img);
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

void Erode(const poutre::IInterface &i_img, const se::IStructuringElement& str_el,  poutre::IInterface &o_img)
{
  POUTRE_ENTERING("Erode IStructuringElement");
  AssertSizesCompatible(i_img, o_img, "Erode images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "Erode images must have compatible types");
  AssertImagesAreDifferent(i_img, o_img, "Erode images input output images must be different");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Erode Unsupported number of dims:0");
  }
  case 1: {
    const auto *const strel_ptr_t = dynamic_cast<const se::details::neighbor_list_t< 1 >*>(&str_el);
    if (strel_ptr_t == nullptr) {
      POUTRE_RUNTIME_ERROR("Erode Unsupported IStructuringElement type");
    }
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      ErodeImageRuntimeNLDispatch<1, poutre::PType::PType_GrayUINT8>(i_img, *strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      ErodeImageRuntimeNLDispatch<1, poutre::PType::PType_GrayINT32>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      ErodeImageRuntimeNLDispatch<1, poutre::PType::PType_GrayINT64>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_F32: {
      ErodeImageRuntimeNLDispatch<1, poutre::PType::PType_F32>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_D64: {
      ErodeImageRuntimeNLDispatch<1, poutre::PType::PType_D64>(i_img, *strel_ptr_t, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Erode unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    const auto *const strel_ptr_t = dynamic_cast<const se::details::neighbor_list_t< 2 >*>(&str_el);
    if (strel_ptr_t == nullptr) {
      POUTRE_RUNTIME_ERROR("Erode Unsupported IStructuringElement type");
    }

    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      ErodeImageRuntimeNLDispatch<2, poutre::PType::PType_GrayUINT8>(i_img, *strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      ErodeImageRuntimeNLDispatch<2, poutre::PType::PType_GrayINT32>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      ErodeImageRuntimeNLDispatch<2, poutre::PType::PType_GrayINT64>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_F32: {
      ErodeImageRuntimeNLDispatch<2, poutre::PType::PType_F32>(i_img, *strel_ptr_t,o_img);
    } break;
    case poutre::PType::PType_D64: {
      ErodeImageRuntimeNLDispatch<2, poutre::PType::PType_D64>(i_img, *strel_ptr_t,o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Erode unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    const auto *const strel_ptr_t = dynamic_cast<const se::details::neighbor_list_t< 3 >*>(&str_el);
    if (strel_ptr_t == nullptr) {
      POUTRE_RUNTIME_ERROR("Erode Unsupported IStructuringElement type");
    }
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      ErodeImageRuntimeNLDispatch<3, poutre::PType::PType_GrayUINT8>(i_img, *strel_ptr_t,o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      ErodeImageRuntimeNLDispatch<3, poutre::PType::PType_GrayINT32>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      ErodeImageRuntimeNLDispatch<3, poutre::PType::PType_GrayINT64>(i_img,*strel_ptr_t, o_img);
    } break;
    case poutre::PType::PType_F32: {
      ErodeImageRuntimeNLDispatch<3, poutre::PType::PType_F32>(i_img, *strel_ptr_t,o_img);
    } break;
    case poutre::PType::PType_D64: {
      ErodeImageRuntimeNLDispatch<3, poutre::PType::PType_D64>(i_img,*strel_ptr_t, o_img);
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
}