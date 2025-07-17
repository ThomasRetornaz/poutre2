
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
#include <memory>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/base/types.hpp>
#include <poutre/base/types_traits.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/low_level_morpho/ero_dil.hpp>
#include <poutre/low_level_morpho/details/ero_dil_static_se_t.hpp>
#include <poutre/low_level_morpho/details/ero_dil_compound_static_se_t.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>

namespace {

template<ptrdiff_t NumDims, poutre::PType P> void ErodeImageDispatch(const poutre::IInterface &i_img, poutre::se::Common_NL_SE nl_static, poutre::IInterface &o_img)
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ErodeImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ErodeImageDispatch img2_t downcast fail"); }
  poutre::llm::details::t_Erode(*img1_t,nl_static,*img2_t);
}

template<ptrdiff_t NumDims, poutre::PType P> void DilateImageDispatch(const poutre::IInterface &i_img, poutre::se::Common_NL_SE nl_static, poutre::IInterface &o_img)
{
  using ImgType = poutre::details::image_t<typename poutre::enum_to_type<poutre::CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("DilateImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("DilateImageDispatch img2_t downcast fail"); }
  poutre::llm::details::t_Dilate(*img1_t,nl_static,*img2_t);
}

void Erode(const poutre::IInterface &i_img, poutre::se::Common_NL_SE nl_static, poutre::IInterface &o_img)
{
  AssertSizesCompatible(i_img, o_img, "Erode images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "Erode images must have compatible types");
  AssertImagesAreDifferent(i_img, o_img, "Erode images input ouput images must be different");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Erode Unsupported number of dims:0");
  }
  case 1: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      ErodeImageDispatch<1, poutre::PType::PType_GrayUINT8>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      ErodeImageDispatch<1, poutre::PType::PType_GrayINT32>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      ErodeImageDispatch<1, poutre::PType::PType_GrayINT64>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      ErodeImageDispatch<1, poutre::PType::PType_F32>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_D64: {
      ErodeImageDispatch<1, poutre::PType::PType_D64>(i_img, nl_static, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Erode unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      ErodeImageDispatch<2, poutre::PType::PType_GrayUINT8>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      ErodeImageDispatch<2, poutre::PType::PType_GrayINT32>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      ErodeImageDispatch<2, poutre::PType::PType_GrayINT64>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      ErodeImageDispatch<2, poutre::PType::PType_F32>(i_img, nl_static,o_img);
    } break;
    case poutre::PType::PType_D64: {
      ErodeImageDispatch<2, poutre::PType::PType_D64>(i_img, nl_static,o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Erode unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      ErodeImageDispatch<3, poutre::PType::PType_GrayUINT8>(i_img, nl_static,o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      ErodeImageDispatch<3, poutre::PType::PType_GrayINT32>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      ErodeImageDispatch<3, poutre::PType::PType_GrayINT64>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      ErodeImageDispatch<3, poutre::PType::PType_F32>(i_img, nl_static,o_img);
    } break;
    case poutre::PType::PType_D64: {
      ErodeImageDispatch<3, poutre::PType::PType_D64>(i_img,nl_static, o_img);
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

void Dilate(const poutre::IInterface &i_img, poutre::se::Common_NL_SE nl_static, poutre::IInterface &o_img)
{
  AssertSizesCompatible(i_img, o_img, "Dilate images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "Dilate images must have compatible types");
  AssertImagesAreDifferent(i_img, o_img, "Dilate images input ouput images must be different");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Dilate Unsupported number of dims:0");
  }
  case 1: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      DilateImageDispatch<1, poutre::PType::PType_GrayUINT8>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      DilateImageDispatch<1, poutre::PType::PType_GrayINT32>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      DilateImageDispatch<1, poutre::PType::PType_GrayINT64>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      DilateImageDispatch<1, poutre::PType::PType_F32>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_D64: {
      DilateImageDispatch<1, poutre::PType::PType_D64>(i_img, nl_static, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Dilate unsupported PTYPE");
    }
    }
  } break;
  case 2: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      DilateImageDispatch<2, poutre::PType::PType_GrayUINT8>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      DilateImageDispatch<2, poutre::PType::PType_GrayINT32>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      DilateImageDispatch<2, poutre::PType::PType_GrayINT64>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      DilateImageDispatch<2, poutre::PType::PType_F32>(i_img, nl_static,o_img);
    } break;
    case poutre::PType::PType_D64: {
      DilateImageDispatch<2, poutre::PType::PType_D64>(i_img, nl_static,o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("Dilate unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case poutre::PType::PType_GrayUINT8: {
      DilateImageDispatch<3, poutre::PType::PType_GrayUINT8>(i_img, nl_static,o_img);
    } break;
    case poutre::PType::PType_GrayINT32: {
      DilateImageDispatch<3, poutre::PType::PType_GrayINT32>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_GrayINT64: {
      DilateImageDispatch<3, poutre::PType::PType_GrayINT64>(i_img,nl_static, o_img);
    } break;
    case poutre::PType::PType_F32: {
      DilateImageDispatch<3, poutre::PType::PType_F32>(i_img, nl_static,o_img);
    } break;
    case poutre::PType::PType_D64: {
      DilateImageDispatch<3, poutre::PType::PType_D64>(i_img,nl_static, o_img);
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

namespace poutre {

void Erode(const IInterface &i_img, se::Common_NL_SE nl_static, const int iter, IInterface &o_img)
{
  POUTRE_ENTERING("Erode iter");
  AssertSizesCompatible(i_img, o_img, "Erode iter images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "Erode iter images must have compatible types");
  AssertImagesAreDifferent(i_img, o_img, "Erode iter images input ouput images must be different");
  POUTRE_CHECK(iter>=0, "Erode iter must be >= 0");

  if ( iter == 0 )
  {
    CopyInto(i_img, o_img);
    return;
  }

  if ( iter == 1 )
  {
    ::Erode(i_img, nl_static, o_img);
    return;
  }

  // Temporary image starts as a copy of the input image
  std::unique_ptr<IInterface> tmpImg(Clone(i_img));  // NOLINT
  IInterface* tmpImg1 = tmpImg.get();
  IInterface* tmpImg2 = &o_img;

  const int nooddsize = iter % 2; // equals 0 or 1
  const int oddsize = iter - nooddsize;

  for ( int iterodd = 0; iterodd < oddsize; iterodd += 2 )
  {
    ::Erode(*tmpImg1, nl_static, *tmpImg2);
    ::Erode(*tmpImg2, nl_static, *tmpImg1);
  }

  if ( nooddsize == 0 )
  {
    CopyInto(*tmpImg1, *tmpImg2); //costly ?, if yes use swap
    return;
  }
  else
  {
    ::Erode(*tmpImg1, nl_static, *tmpImg2);
  }
}

void Dilate(const IInterface &i_img, se::Common_NL_SE nl_static, const int iter, IInterface &o_img)
{
  POUTRE_ENTERING("Dilate iter");
  AssertSizesCompatible(i_img, o_img, "Dilate iter images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "Dilate iter images must have compatible types");
  AssertImagesAreDifferent(i_img, o_img, "Dilate iter images input ouput images must be different");
  POUTRE_CHECK(iter>=0, "Dilate iter must be >= 0");

  if ( iter == 0 )
  {
    CopyInto(i_img, o_img);
    return;
  }

  if ( iter == 1 )
  {
    ::Dilate(i_img, nl_static, o_img);
    return;
  }

  // Temporary image starts as a copy of the input image
  std::unique_ptr<IInterface> tmpImg(Clone(i_img));  // NOLINT
  IInterface* tmpImg1 = tmpImg.get();
  IInterface* tmpImg2 = &o_img;

  const int nooddsize = iter % 2; // equals 0 or 1
  const int oddsize = iter - nooddsize;

  for ( int iterodd = 0; iterodd < oddsize; iterodd += 2 )
  {
    ::Dilate(*tmpImg1, nl_static, *tmpImg2);
    ::Dilate(*tmpImg2, nl_static, *tmpImg1);
  }

  if ( nooddsize == 0 )
  {
    CopyInto(*tmpImg1, *tmpImg2); //costly ?, if yes use swap
    return;
  }
  else
  {
    ::Dilate(*tmpImg1, nl_static, *tmpImg2);
  }
}
}// namespace poutre