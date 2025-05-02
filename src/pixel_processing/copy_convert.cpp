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
#include <poutre/pixel_processing/copy_convert.hpp>
#include <poutre/pixel_processing/details/copy_convert_t.hpp>

namespace poutre {
std::unique_ptr<IInterface> Clone(const IInterface &i_img1)
{
  POUTRE_ENTERING("Clone");
  auto o_img = CloneGeometry(i_img1);
  CopyInto(i_img1, *o_img);
  return o_img;
}

std::unique_ptr<IInterface> CloneGeometry(const IInterface &i_img1)
{
  POUTRE_ENTERING("CloneGeometry");
  return Create(i_img1.GetShape(), i_img1.GetCType(), i_img1.GetPType());
}

std::unique_ptr<IInterface> Convert(const IInterface &i_img1, CompoundType ctype, PType ptype)
{
  POUTRE_ENTERING("Convert");
  auto o_img = ConvertGeometry(i_img1, ctype, ptype);
  ConvertInto(i_img1, *o_img);
  return o_img;
}

std::unique_ptr<IInterface> ConvertGeometry(const IInterface &i_img1, CompoundType ctype, PType ptype)
{
  POUTRE_ENTERING("ConvertGeometry");
  return Create(i_img1.GetShape(), ctype, ptype);
}

std::unique_ptr<IInterface> ConvertGeometry(const IInterface &i_img1, PType ptype)
{
  POUTRE_ENTERING("ConvertGeometry");
  return ConvertGeometry(i_img1, i_img1.GetCType(), ptype);
}

void CopyInto(const IInterface &i_img, IInterface &o_img)
{
  POUTRE_ENTERING("CopyInto");
  AssertSizesCompatible(i_img, o_img, "Copy images have not compatible sizes");
  AssertAsTypesCompatible(
    i_img, o_img, "Copy images have not compatible types may you have to use ConvertInto instead");
  ConvertInto(i_img, o_img);
}

template<size_t NumDims, PType Pin> void ConvertIntoDispatchScalarP(const IInterface &i_img1, IInterface &o_img2)
{
  using ImgType1 = details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, Pin>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType1 *>(&i_img1);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP i_img1 downcast fail"); }
  switch (o_img2.GetPType())// allow type promotion
  {
  case PType::PType_GrayUINT8: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, PType::PType_GrayUINT8>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_GrayINT32: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, PType::PType_GrayINT32>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_GrayINT64: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, PType::PType_GrayINT64>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_F32: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, PType::PType_F32>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_D64: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_Scalar, PType::PType_D64>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 PType not supported");
  }
  }
}

template<size_t NumDims, PType Pin> void ConvertIntoDispatch3PLanesP(const IInterface &i_img1, IInterface &o_img2)
{
  using ImgType1 = details::image_t<typename enum_to_type<CompoundType::CompoundType_3Planes, Pin>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType1 *>(&i_img1);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP i_img1 downcast fail"); }
  switch (o_img2.GetPType())// allow type promotion
  {
  case PType::PType_GrayUINT8: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_3Planes, PType::PType_GrayUINT8>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_GrayINT32: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_3Planes, PType::PType_GrayINT32>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_GrayINT64: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_3Planes, PType::PType_GrayINT64>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_F32: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_3Planes, PType::PType_F32>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_D64: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_3Planes, PType::PType_D64>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 PType not supported");
  }
  }
}

template<size_t NumDims, PType Pin> void ConvertIntoDispatch4PLanesP(const IInterface &i_img1, IInterface &o_img2)
{
  using ImgType1 = details::image_t<typename enum_to_type<CompoundType::CompoundType_4Planes, Pin>::type, NumDims>;
  const auto *img1_t = dynamic_cast<const ImgType1 *>(&i_img1);
  if (!img1_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP i_img1 downcast fail"); }
  switch (o_img2.GetPType())// allow type promotion
  {
  case PType::PType_GrayUINT8: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_4Planes, PType::PType_GrayUINT8>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_GrayINT32: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_4Planes, PType::PType_GrayINT32>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_GrayINT64: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_4Planes, PType::PType_GrayINT64>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_F32: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_4Planes, PType::PType_F32>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  case PType::PType_D64: {
    using ImgType2 =
      details::image_t<typename enum_to_type<CompoundType::CompoundType_4Planes, PType::PType_D64>::type, NumDims>;
    auto *img2_t = dynamic_cast<ImgType2 *>(&o_img2);
    if (!img2_t) { POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 downcast fail"); }
    details::t_Copy(*img1_t, *img2_t);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("ConvertIntoDispatchP o_img2 PType not supported");
  }
  }
}

template<size_t numDims> void ConvertIntoDispatch(const IInterface &i_img1, IInterface &o_img2)
{
  POUTRE_CHECK(i_img1.GetCType() == o_img2.GetCType(), "ConvertInto must have same CType");
  switch (i_img1.GetCType()) {
  case CompoundType::CompoundType_Scalar: {
    switch (i_img1.GetPType()) {
    case PType::PType_GrayUINT8: {
      ConvertIntoDispatchScalarP<numDims,PType::PType_GrayUINT8>(i_img1,o_img2);
    } break;
    case PType::PType_GrayINT32: {
      ConvertIntoDispatchScalarP<numDims,PType::PType_GrayINT32>(i_img1,o_img2);
    } break;
    case PType::PType_GrayINT64: {
      ConvertIntoDispatchScalarP<numDims,PType::PType_GrayINT64>(i_img1,o_img2);
    } break;
    case PType::PType_F32: {
      ConvertIntoDispatchScalarP<numDims,PType::PType_F32>(i_img1,o_img2);
    } break;
    case PType::PType_D64: {
      ConvertIntoDispatchScalarP<numDims,PType::PType_D64>(i_img1,o_img2);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("ConvertIntoDispatch unsupported PTYPE");
    }
    }
    break;
  } break;
  case CompoundType::CompoundType_3Planes:
   {
    switch (i_img1.GetPType()) {
      case PType::PType_GrayUINT8: {
        ConvertIntoDispatch3PLanesP<numDims,PType::PType_GrayUINT8>(i_img1,o_img2);
      } break;
      case PType::PType_GrayINT32: {
        ConvertIntoDispatch3PLanesP<numDims,PType::PType_GrayINT32>(i_img1,o_img2);
      } break;
      case PType::PType_GrayINT64: {
        ConvertIntoDispatch3PLanesP<numDims,PType::PType_GrayINT64>(i_img1,o_img2);
      } break;
      case PType::PType_F32: {
        ConvertIntoDispatch3PLanesP<numDims,PType::PType_F32>(i_img1,o_img2);
      } break;
      case PType::PType_D64: {
        ConvertIntoDispatch3PLanesP<numDims,PType::PType_D64>(i_img1,o_img2);
      } break;
      default: {
        POUTRE_RUNTIME_ERROR("ConvertIntoDispatch unsupported PTYPE");
      }
      }
      break;
  } break;
  case CompoundType::CompoundType_4Planes:
  {
    switch (i_img1.GetPType()) {
      case PType::PType_GrayUINT8: {
        ConvertIntoDispatch4PLanesP<numDims,PType::PType_GrayUINT8>(i_img1,o_img2);
      } break;
      case PType::PType_GrayINT32: {
        ConvertIntoDispatch4PLanesP<numDims,PType::PType_GrayINT32>(i_img1,o_img2);
      } break;
      case PType::PType_GrayINT64: {
        ConvertIntoDispatch4PLanesP<numDims,PType::PType_GrayINT64>(i_img1,o_img2);
      } break;
      case PType::PType_F32: {
        ConvertIntoDispatch4PLanesP<numDims,PType::PType_F32>(i_img1,o_img2);
      } break;
      case PType::PType_D64: {
        ConvertIntoDispatch4PLanesP<numDims,PType::PType_D64>(i_img1,o_img2);
      } break;
      default: {
        POUTRE_RUNTIME_ERROR("ConvertIntoDispatch unsupported PTYPE");
      }
      }
      break;
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("ConvertIntoDispatch unsupported CTYPE");
  }
  }
}


void ConvertInto(const IInterface &i_img, IInterface &o_img)
{
  POUTRE_ENTERING("ConvertInto");
  AssertSizesCompatible(i_img, o_img, "ConvertInto images have not compatible sizes");
  AssertImagesAreDifferent(i_img, o_img, "ConvertInto images must be differents");

  const auto &numDims = i_img.GetRank();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    ConvertIntoDispatch<2>(i_img, o_img);
  } break;
  case 3: {
    POUTRE_CHECK(i_img.GetCType() == o_img.GetCType(), "ConvertInto must have same CType");
    POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "ConvertInto must be scalar");
    switch (i_img.GetPType()) {
      case PType::PType_GrayUINT8: {
        ConvertIntoDispatchScalarP<3,PType::PType_GrayUINT8>(i_img,o_img);
      } break;
      case PType::PType_GrayINT32: {
        ConvertIntoDispatchScalarP<3,PType::PType_GrayINT32>(i_img,o_img);
      } break;
      case PType::PType_GrayINT64: {
        ConvertIntoDispatchScalarP<3,PType::PType_GrayINT64>(i_img,o_img);
      } break;
      case PType::PType_F32: {
        ConvertIntoDispatchScalarP<3,PType::PType_F32>(i_img,o_img);
      } break;
      case PType::PType_D64: {
        ConvertIntoDispatchScalarP<3,PType::PType_D64>(i_img,o_img);
      } break;
      default: {
        POUTRE_RUNTIME_ERROR("ConvertIntoDispatch unsupported PTYPE");
      }
      }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("Unsupported number of dims");
  }
  }
}
}// namespace poutre
