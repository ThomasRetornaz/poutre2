
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
#include <variant>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/base/types.hpp>
#include <poutre/base/types_traits.hpp>
#include <poutre/pixel_processing/arith.hpp>
#include <poutre/pixel_processing/details/arith_op_t.hpp>

namespace poutre {

template<ptrdiff_t NumDims, PType P>
void ArithSaturatedAddConstantDispatch(const IInterface &i_img, const ScalarTypeVariant &vscalar, IInterface &o_img)
{
  using pType = typename enum_to_type<CompoundType::CompoundType_Scalar, P>::type;
  using ImgType = details::image_t<pType, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ArithSaturatedAddConstantDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ArithSaturatedAddConstantDispatch img2_t downcast fail"); }

  try {
    const pType pix_val = std::get<pType>(vscalar);
    details::t_ArithSaturatedAddConstant(*img1_t, pix_val, *img2_t);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("ArithSaturatedAddConstantDispatch unable to extract suistable type from pvalue");
  }
}

template<ptrdiff_t NumDims, PType P>
void ArithSaturatedSubConstantDispatch(const IInterface &i_img, const ScalarTypeVariant &vscalar, IInterface &o_img)
{
  using pType = typename enum_to_type<CompoundType::CompoundType_Scalar, P>::type;
  using ImgType = details::image_t<pType, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ArithSaturatedASubConstantDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ArithSaturatedASubConstantDispatch img2_t downcast fail"); }
  try {
    const pType pix_val = std::get<pType>(vscalar);
    details::t_ArithSaturatedSubConstant(*img1_t, pix_val, *img2_t);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("ArithSaturatedAddConstantDispatch unable to extract suistable type from pvalue");
  }
}

template<ptrdiff_t NumDims, PType P>
void ArithSaturatedAddImageDispatch(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img)
{
  using ImgType = details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img1);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ArithSaddImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<const ImgType *>(&i_img2);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ArithSaddImageDispatch img2_t downcast fail"); }
  auto *img3_t = dynamic_cast<ImgType *>(&o_img);
  if (!img3_t) { POUTRE_RUNTIME_ERROR("ArithSaddImageDispatch img3_t downcast fail"); }
  details::t_ArithSaturatedAdd(*img1_t, *img2_t, *img3_t);
}


template<ptrdiff_t NumDims, PType P>
void ArithSaturatedSubImageDispatch(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img)
{
  using ImgType = details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img1);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ArithSsubImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<const ImgType *>(&i_img2);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ArithSsubImageDispatch img2_t downcast fail"); }
  auto *img3_t = dynamic_cast<ImgType *>(&o_img);
  if (!img3_t) { POUTRE_RUNTIME_ERROR("ArithSsubImageDispatch img3_t downcast fail"); }
  details::t_ArithSaturatedSub(*img1_t, *img2_t, *img3_t);
}

template<ptrdiff_t NumDims, PType P>
void ArithSupImageDispatch(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img)
{
  using ImgType = details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img1);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ArithSupImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<const ImgType *>(&i_img2);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ArithSupImageDispatch img2_t downcast fail"); }
  auto *img3_t = dynamic_cast<ImgType *>(&o_img);
  if (!img3_t) { POUTRE_RUNTIME_ERROR("ArithSupImageDispatch img3_t downcast fail"); }
  details::t_ArithSup(*img1_t, *img2_t, *img3_t);
}

template<ptrdiff_t NumDims, PType P>
void ArithInfImageDispatch(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img)
{
  using ImgType = details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img1);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ArithInfImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<const ImgType *>(&i_img2);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ArithInfImageDispatch img2_t downcast fail"); }
  auto *img3_t = dynamic_cast<ImgType *>(&o_img);
  if (!img3_t) { POUTRE_RUNTIME_ERROR("ArithInfImageDispatch img3_t downcast fail"); }
  details::t_ArithInf(*img1_t, *img2_t, *img3_t);
}

template<ptrdiff_t NumDims, PType P> void ArithInvertImageDispatch(const IInterface &i_img, IInterface &o_img)
{
  using ImgType = details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, P>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType *>(&i_img);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ArithInvertImageDispatch img1_t downcast fail"); }
  auto *img2_t = dynamic_cast<ImgType *>(&o_img);
  if (!img2_t) { POUTRE_RUNTIME_ERROR("ArithInvertImageDispatch img2_t downcast fail"); }
  details::t_ArithInvert(*img1_t, *img2_t);
}

void ArithSaturatedAddImage(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img)
{
  POUTRE_ENTERING("ArithSaturatedAddImage");
  AssertSizesCompatible(i_img1, o_img, "ArithSaturatedAddImage images have not compatible sizes");
  AssertSizesCompatible(i_img2, o_img, "ArithSaturatedAddImage images have not compatible sizes");// NOLINT
  AssertAsTypesCompatible(i_img1, o_img, "ArithSaturatedAddImage images must have compatible types");
  AssertAsTypesCompatible(i_img2, o_img, "ArithSaturatedAddImage images must have compatible types");// NOLINT

  POUTRE_CHECK(i_img1.GetCType() == CompoundType::CompoundType_Scalar, "ArithSaturatedAddImage must be scalar");

  switch (i_img1.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("ArithSaturatedAddImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img1.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithSaturatedAddImageDispatch<2, PType::PType_GrayUINT8>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithSaturatedAddImageDispatch<2, PType::PType_GrayINT32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithSaturatedAddImageDispatch<2, PType::PType_GrayINT64>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_F32: {
      ArithSaturatedAddImageDispatch<2, PType::PType_F32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_D64: {
      ArithSaturatedAddImageDispatch<2, PType::PType_D64>(i_img1, i_img2, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithSaturatedAddImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img1.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithSaturatedAddImageDispatch<3, PType::PType_GrayUINT8>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithSaturatedAddImageDispatch<3, PType::PType_GrayINT32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithSaturatedAddImageDispatch<3, PType::PType_GrayINT64>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_F32: {
      ArithSaturatedAddImageDispatch<3, PType::PType_F32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_D64: {
      ArithSaturatedAddImageDispatch<3, PType::PType_D64>(i_img1, i_img2, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithSaturatedAddImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("ArithSaturatedAddImage Unsupported number of dims");
  }
  }
}

void ArithSaturatedSubImage(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img)
{
  POUTRE_ENTERING("ArithSaturatedSubImage");
  AssertSizesCompatible(i_img1, o_img, "ArithSaturatedSubImage images have not compatible sizes");
  AssertSizesCompatible(i_img2, o_img, "ArithSaturatedSubImage images have not compatible sizes");// NOLINT
  AssertAsTypesCompatible(i_img1, o_img, "ArithSaturatedSubImage images must have compatible types");
  AssertAsTypesCompatible(i_img2, o_img, "ArithSaturatedSubImage images must have compatible types");// NOLINT

  POUTRE_CHECK(i_img1.GetCType() == CompoundType::CompoundType_Scalar, "ArithSaturatedSubImage must be scalar");

  switch (i_img1.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("ArithSaturatedSubImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img1.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithSaturatedSubImageDispatch<2, PType::PType_GrayUINT8>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithSaturatedSubImageDispatch<2, PType::PType_GrayINT32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithSaturatedSubImageDispatch<2, PType::PType_GrayINT64>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_F32: {
      ArithSaturatedSubImageDispatch<2, PType::PType_F32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_D64: {
      ArithSaturatedSubImageDispatch<2, PType::PType_D64>(i_img1, i_img2, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithSaturatedSubImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img1.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithSaturatedSubImageDispatch<3, PType::PType_GrayUINT8>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithSaturatedSubImageDispatch<3, PType::PType_GrayINT32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithSaturatedSubImageDispatch<3, PType::PType_GrayINT64>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_F32: {
      ArithSaturatedSubImageDispatch<3, PType::PType_F32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_D64: {
      ArithSaturatedSubImageDispatch<3, PType::PType_D64>(i_img1, i_img2, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithSaturatedSubImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("ArithSaturatedSubImage Unsupported number of dims");
  }
  }
}
void ArithSupImage(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img)
{
  POUTRE_ENTERING("ArithSupImage");
  AssertSizesCompatible(i_img1, o_img, "ArithInfImage images have not compatible sizes");
  AssertSizesCompatible(i_img2, o_img, "ArithInfImage images have not compatible sizes");// NOLINT
  AssertAsTypesCompatible(i_img1, o_img, "ArithInfImage images must have compatible types");
  AssertAsTypesCompatible(i_img2, o_img, "ArithInfImage images must have compatible types");// NOLINT

  POUTRE_CHECK(i_img1.GetCType() == CompoundType::CompoundType_Scalar, "ArithSupImage must be scalar");

  switch (i_img1.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("ArithSupImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img1.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithSupImageDispatch<2, PType::PType_GrayUINT8>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithSupImageDispatch<2, PType::PType_GrayINT32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithSupImageDispatch<2, PType::PType_GrayINT64>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_F32: {
      ArithSupImageDispatch<2, PType::PType_F32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_D64: {
      ArithSupImageDispatch<2, PType::PType_D64>(i_img1, i_img2, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithSupImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img1.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithSupImageDispatch<3, PType::PType_GrayUINT8>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithSupImageDispatch<3, PType::PType_GrayINT32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithSupImageDispatch<3, PType::PType_GrayINT64>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_F32: {
      ArithSupImageDispatch<3, PType::PType_F32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_D64: {
      ArithSupImageDispatch<3, PType::PType_D64>(i_img1, i_img2, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithSupImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("ArithSupImage Unsupported number of dims");
  }
  }
}

void ArithInfImage(const IInterface &i_img1, const IInterface &i_img2, IInterface &o_img)
{
  POUTRE_ENTERING("ArithInfImage");
  AssertSizesCompatible(i_img1, o_img, "ArithInfImage images have not compatible sizes");
  AssertSizesCompatible(i_img2, o_img, "ArithInfImage images have not compatible sizes");// NOLINT
  AssertAsTypesCompatible(i_img1, o_img, "ArithInfImage images must have compatible types");
  AssertAsTypesCompatible(i_img2, o_img, "ArithInfImage images must have compatible types");// NOLINT

  POUTRE_CHECK(i_img1.GetCType() == CompoundType::CompoundType_Scalar, "ArithInfImage must be scalar");

  switch (i_img1.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("ArithInfImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img1.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithInfImageDispatch<2, PType::PType_GrayUINT8>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithInfImageDispatch<2, PType::PType_GrayINT32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithInfImageDispatch<2, PType::PType_GrayINT64>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_F32: {
      ArithInfImageDispatch<2, PType::PType_F32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_D64: {
      ArithInfImageDispatch<2, PType::PType_D64>(i_img1, i_img2, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithInfImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img1.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithInfImageDispatch<3, PType::PType_GrayUINT8>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithInfImageDispatch<3, PType::PType_GrayINT32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithInfImageDispatch<3, PType::PType_GrayINT64>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_F32: {
      ArithInfImageDispatch<3, PType::PType_F32>(i_img1, i_img2, o_img);
    } break;
    case PType::PType_D64: {
      ArithInfImageDispatch<3, PType::PType_D64>(i_img1, i_img2, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithInfImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("ArithInfImage Unsupported number of dims");
  }
  }
}

void ArithSaturatedAddConstant(const IInterface &i_img,const ScalarTypeVariant &pvalue, IInterface &o_img)
{
  POUTRE_ENTERING("ArithSaturatedAddConstant");
  AssertSizesCompatible(i_img, o_img, "ArithSaturatedAddConstant images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "ArithSaturatedAddConstant images must have compatible types");
  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "ArithSaturatedAddConstant must be scalar");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("ArithSaturatedAddConstant Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithSaturatedAddConstantDispatch<2, PType::PType_GrayUINT8>(i_img, pvalue, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithSaturatedAddConstantDispatch<2, PType::PType_GrayINT32>(i_img, pvalue, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithSaturatedAddConstantDispatch<2, PType::PType_GrayINT64>(i_img, pvalue, o_img);
    } break;
    case PType::PType_F32: {
      ArithSaturatedAddConstantDispatch<2, PType::PType_F32>(i_img, pvalue, o_img);
    } break;
    case PType::PType_D64: {
      ArithSaturatedAddConstantDispatch<2, PType::PType_D64>(i_img, pvalue, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithSaturatedAddConstant unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: { 
      ArithSaturatedAddConstantDispatch<3, PType::PType_GrayUINT8>(i_img, pvalue, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithSaturatedAddConstantDispatch<3, PType::PType_GrayINT32>(i_img, pvalue, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithSaturatedAddConstantDispatch<3, PType::PType_GrayINT64>(i_img, pvalue, o_img);
    } break;
    case PType::PType_F32: {
      ArithSaturatedAddConstantDispatch<3, PType::PType_F32>(i_img, pvalue, o_img);
    } break;
    case PType::PType_D64: {
      ArithSaturatedAddConstantDispatch<3, PType::PType_D64>(i_img, pvalue, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithSaturatedAddConstant unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("ArithSaturatedAddConstant Unsupported number of dims");
  }
  }
}

void ArithSaturatedSubConstant(const IInterface &i_img,const ScalarTypeVariant &pvalue, IInterface &o_img)
{
  POUTRE_ENTERING("ArithSaturatedSubConstant");
  AssertSizesCompatible(i_img, o_img, "ArithSaturatedSubConstant images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "ArithSaturatedSubConstant images must have compatible types");
  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "ArithSaturatedSubConstant must be scalar");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("ArithSaturatedSubConstant Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithSaturatedSubConstantDispatch<2, PType::PType_GrayUINT8>(i_img, pvalue, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithSaturatedSubConstantDispatch<2, PType::PType_GrayINT32>(i_img, pvalue, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithSaturatedSubConstantDispatch<2, PType::PType_GrayINT64>(i_img, pvalue, o_img);
    } break;
    case PType::PType_F32: {
      ArithSaturatedSubConstantDispatch<2, PType::PType_F32>(i_img, pvalue, o_img);
    } break;
    case PType::PType_D64: {
      ArithSaturatedSubConstantDispatch<2, PType::PType_D64>(i_img, pvalue, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithSaturatedSubConstant unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: { 
      ArithSaturatedSubConstantDispatch<3, PType::PType_GrayUINT8>(i_img, pvalue, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithSaturatedSubConstantDispatch<3, PType::PType_GrayINT32>(i_img, pvalue, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithSaturatedSubConstantDispatch<3, PType::PType_GrayINT64>(i_img, pvalue, o_img);
    } break;
    case PType::PType_F32: {
      ArithSaturatedSubConstantDispatch<3, PType::PType_F32>(i_img, pvalue, o_img);
    } break;
    case PType::PType_D64: {
      ArithSaturatedSubConstantDispatch<3, PType::PType_D64>(i_img, pvalue, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithSaturatedSubConstant unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("ArithSaturatedSubConstant Unsupported number of dims");
  }
  }
}

void ArithInvertImage(const IInterface &i_img, IInterface &o_img)
{
  POUTRE_ENTERING("ArithInvertImage");
  AssertSizesCompatible(i_img, o_img, "ArithInvertImage images have not compatible sizes");
  AssertAsTypesCompatible(i_img, o_img, "ArithInvertImage images must have compatible types");
  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "ArithInvertImage must be scalar");

  switch (i_img.GetRank()) {
  case 0: {
    POUTRE_RUNTIME_ERROR("ArithInvertImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithInvertImageDispatch<2, PType::PType_GrayUINT8>(i_img, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithInvertImageDispatch<2, PType::PType_GrayINT32>(i_img, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithInvertImageDispatch<2, PType::PType_GrayINT64>(i_img, o_img);
    } break;
    case PType::PType_F32: {
      ArithInvertImageDispatch<2, PType::PType_F32>(i_img, o_img);
    } break;
    case PType::PType_D64: {
      ArithInvertImageDispatch<2, PType::PType_D64>(i_img, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithInvertImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      ArithInvertImageDispatch<3, PType::PType_GrayUINT8>(i_img, o_img);
    } break;
    case PType::PType_GrayINT32: {
      ArithInvertImageDispatch<3, PType::PType_GrayINT32>(i_img, o_img);
    } break;
    case PType::PType_GrayINT64: {
      ArithInvertImageDispatch<3, PType::PType_GrayINT64>(i_img, o_img);
    } break;
    case PType::PType_F32: {
      ArithInvertImageDispatch<3, PType::PType_F32>(i_img, o_img);
    } break;
    case PType::PType_D64: {
      ArithInvertImageDispatch<3, PType::PType_D64>(i_img, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ArithInvertImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("ArithInvertImage Unsupported number of dims");
  }
  }
}
}// namespace poutre