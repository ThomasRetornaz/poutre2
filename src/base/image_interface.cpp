// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <algorithm>
#include <cstddef>
#include <format>
#include <memory>
#include <ostream>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/base/types.hpp>
#include <poutre/base/types_traits.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace poutre {
std::ostream &operator<<(std::ostream &stream, const IInterface &img)// NOLINT(misc-unused-parameters)
{
  stream << img.str();
  return stream;
}

bool AsSizesCompatible(const IInterface &i_img1, const IInterface &i_img2)// NOLINT(misc-unused-parameters)
{
  POUTRE_ENTERING("AsSizesCompatible");
  if (i_img1.GetRank() != i_img2.GetRank()) { return false; }
  const auto &coord1 = i_img1.GetShape();
  const auto &coord2 = i_img2.GetShape();
  return std::equal(coord1.begin(), coord1.end(), coord2.begin());
}

bool AsTypesCompatible(const IInterface &i_img1, const IInterface &i_img2)// NOLINT(misc-unused-parameters)
{
  POUTRE_ENTERING("AsTypesCompatible");
  if (i_img1.GetCType() != i_img2.GetCType()) { return false; }
  if (i_img1.GetPType() != i_img2.GetPType()) { return false; }
  return true;
}

bool AreImagesDifferent(const IInterface &i_img1, const IInterface &i_img2)
{
  POUTRE_ENTERING("AreImagesDifferent");
  return (&i_img1 != &i_img2);
}

void AssertSizesCompatible(const IInterface &i_img1,
  const IInterface &i_img2,
  const std::string &i_msg)// NOLINT(misc-unused-parameters)
{
  POUTRE_ENTERING("AssertSizesCompatible");
  if (!AsSizesCompatible(i_img1, i_img2)) { POUTRE_RUNTIME_ERROR(i_msg); }
}

void AssertAsTypesCompatible(const IInterface &i_img1,
  const IInterface &i_img2,
  const std::string &i_msg)// NOLINT(misc-unused-parameters)
{
  POUTRE_ENTERING("AssertAsTypesCompatible");
  if (!AsTypesCompatible(i_img1, i_img2)) { POUTRE_RUNTIME_ERROR(i_msg); }
}

void AssertImagesAreDifferent(const IInterface &i_img1, const IInterface &i_img2, const std::string &i_msg)
{
  POUTRE_ENTERING("AssertImagesAreDifferent");
  if (!AreImagesDifferent(i_img1, i_img2)) { POUTRE_RUNTIME_ERROR(i_msg); }
}

// TODO FACTORIZE DISPATCH

// NDIMS
template<std::ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateDenseDispatchPTypeScalar(const std::vector<std::size_t> &dims, PType ptype)
{
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<details::image_t<pUINT8, numDims>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<details::image_t<pINT32, numDims>>(dims);
  case PType::PType_F32:
    return std::make_unique<details::image_t<pFLOAT, numDims>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<details::image_t<pINT64, numDims>>(dims);
  case PType::PType_D64:
    return std::make_unique<details::image_t<pDOUBLE, numDims>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(std::format("CreateDenseDispatchPTypeScalar:: Unsupported scalar type:{}", ptype));
  }
  }
}

template<std::ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateDenseDispatchPType3PLanes(const std::vector<std::size_t> &dims, PType ptype)
{
  switch (ptype) {
    // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<details::image_t<compound_type<pUINT8, 3>, numDims>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<details::image_t<compound_type<pINT32, 3>, numDims>>(dims);
  case PType::PType_F32:
    return std::make_unique<details::image_t<compound_type<pFLOAT, 3>, numDims>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<details::image_t<compound_type<pINT64, 3>, numDims>>(dims);
  case PType::PType_D64:
    return std::make_unique<details::image_t<compound_type<pDOUBLE, 3>, numDims>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(std::format("CreateDenseDispatchPType3PLanes:: Unsupported compound type 3 with {}", ptype));
  }
  }
}

template<std::ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateDenseDispatchPType4PLanes(const std::vector<std::size_t> &dims, PType ptype)
{
  switch (ptype) {
    // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<details::image_t<compound_type<pUINT8, 4>, numDims>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<details::image_t<compound_type<pINT32, 4>, numDims>>(dims);
  case PType::PType_F32:
    return std::make_unique<details::image_t<compound_type<pFLOAT, 4>, numDims>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<details::image_t<compound_type<pINT64, 4>, numDims>>(dims);
  case PType::PType_D64:
    return std::make_unique<details::image_t<compound_type<pDOUBLE, 4>, numDims>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(std::format("CreateDenseDispatchPType3PLanes:: Unsupported compound type 3 with {}", ptype));
  }
  }
}

template<std::ptrdiff_t numDims>
std::unique_ptr<IInterface>
  CreateDenseDispatchDims(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype)
{
  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    return CreateDenseDispatchPTypeScalar<numDims>(dims, ptype);
  } break;
  case CompoundType::CompoundType_3Planes: {
    return CreateDenseDispatchPType3PLanes<numDims>(dims, ptype);
  } break;
  case CompoundType::CompoundType_4Planes: {
    return CreateDenseDispatchPType4PLanes<numDims>(dims, ptype);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("Unsupported compound type:{}", ctype));
  }
  }
}

// 1D
template<std::ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateImage1DDispatchPTypeScalar(const std::vector<std::size_t> &dims, PType ptype)
{
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<details::image_t<pUINT8, 1>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<details::image_t<pINT32, 1>>(dims);
  case PType::PType_F32:
    return std::make_unique<details::image_t<pFLOAT, 1>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<details::image_t<pINT64, 1>>(dims);
  case PType::PType_D64:
    return std::make_unique<details::image_t<pDOUBLE, 1>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(std::format("CreateImage1DDispatchPTypeScalar:: Unsupported scalar type:{}", ptype));
  }
  }
}

template<ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateImage1DDispatch(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype)
{
  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    return CreateImage1DDispatchPTypeScalar<numDims>(dims, ptype);
  } break;
  // case CompoundType::CompoundType_3Planes:
  //   {
  //   return  CreateDenseDispatchPType3PLanes<numDims>(dims, ptype);
  //   }break;
  // case CompoundType::CompoundType_4Planes:
  //   {
  //   return  CreateDenseDispatchPType4PLanes<numDims>(dims, ptype);
  //   }break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("Unsupported compound type:{}", ctype));
  }
  }
}

// 2D
template<ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateImage2DDispatchPTypeScalar(const std::vector<std::size_t> &dims, PType ptype)
{
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<details::image_t<pUINT8, 2>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<details::image_t<pINT32, 2>>(dims);
  case PType::PType_F32:
    return std::make_unique<details::image_t<pFLOAT, 2>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<details::image_t<pINT64, 2>>(dims);
  case PType::PType_D64:
    return std::make_unique<details::image_t<pDOUBLE, 2>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(std::format("CreateImage2DDispatchPTypeScalar:: Unsupported scalar type:{}", ptype));
  }
  }
}

template<ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateImage2DDispatchPType3PLanes(const std::vector<std::size_t> &dims, PType ptype)
{
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<details::image_t<compound_type<pUINT8, 3>, 2>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<details::image_t<compound_type<pINT32, 3>, 2>>(dims);
  case PType::PType_F32:
    return std::make_unique<details::image_t<compound_type<pFLOAT, 3>, 2>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<details::image_t<compound_type<pINT64, 3>, 2>>(dims);
  case PType::PType_D64:
    return std::make_unique<details::image_t<compound_type<pDOUBLE, 3>, 2>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(std::format("CreateImage2DDispatchPType3PLanes:: Unsupported scalar type:{}", ptype));
  }
  }
}

template<ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateImage2DDispatchPType4PLanes(const std::vector<std::size_t> &dims, PType ptype)
{
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<details::image_t<compound_type<pUINT8, 4>, 2>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<details::image_t<compound_type<pINT32, 4>, 2>>(dims);
  case PType::PType_F32:
    return std::make_unique<details::image_t<compound_type<pFLOAT, 4>, 2>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<details::image_t<compound_type<pINT64, 4>, 2>>(dims);
  case PType::PType_D64:
    return std::make_unique<details::image_t<compound_type<pDOUBLE, 4>, 2>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(std::format("CreateImage2DDispatchPType4PLanes:: Unsupported scalar type:{}", ptype));
  }
  }
}

template<ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateImage2DDispatch(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype)
{
  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    return CreateImage2DDispatchPTypeScalar<numDims>(dims, ptype);
  } break;
  case CompoundType::CompoundType_3Planes: {
    return CreateImage2DDispatchPType3PLanes<numDims>(dims, ptype);
  } break;
  case CompoundType::CompoundType_4Planes: {
    return CreateImage2DDispatchPType4PLanes<numDims>(dims, ptype);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("Unsupported compound type:{}", ctype));
  }
  }
}

// 3D
template<ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateImage3DDispatchPTypeScalar(const std::vector<std::size_t> &dims, PType ptype)
{
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<details::image_t<pUINT8, 3>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<details::image_t<pINT32, 3>>(dims);
  case PType::PType_F32:
    return std::make_unique<details::image_t<pFLOAT, 3>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<details::image_t<pINT64, 3>>(dims);
  case PType::PType_D64:
    return std::make_unique<details::image_t<pDOUBLE, 3>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(std::format("CreateImage3DDispatchPTypeScalar:: Unsupported scalar type:{}", ptype));
  }
  }
}

template<ptrdiff_t numDims>
std::unique_ptr<IInterface> CreateImage3DDispatchPType3PLanes(const std::vector<std::size_t> &dims, PType ptype)
{
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<details::image_t<compound_type<pUINT8, 3>>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<details::image_t<compound_type<pINT32, 3>>>(dims);
  case PType::PType_F32:
    return std::make_unique<details::image_t<compound_type<pFLOAT, 3>>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<details::image_t<compound_type<pINT64, 3>>>(dims);
  case PType::PType_D64:
    return std::make_unique<details::image_t<compound_type<pDOUBLE, 3>>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(std::format("CreateImage3DDispatchPType3PLanes:: Unsupported scalar type:{}", ptype));
  }
  }
}

template<size_t numDims>
std::unique_ptr<IInterface> CreateImage3DDispatchPType4PLanes(const std::vector<std::size_t> &dims, PType ptype)
{
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<details::image_t<compound_type<pUINT8, 4>>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<details::image_t<compound_type<pINT32, 4>>>(dims);
  case PType::PType_F32:
    return std::make_unique<details::image_t<compound_type<pFLOAT, 4>>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<details::image_t<compound_type<pINT64, 4>>>(dims);
  case PType::PType_D64:
    return std::make_unique<details::image_t<compound_type<pDOUBLE, 4>>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(std::format("CreateImage3DDispatchPType4PLanes:: Unsupported scalar type:{}", ptype));
  }
  }
}

template<size_t numDims>
std::unique_ptr<IInterface> CreateImage3DDispatch(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype)
{
  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    return CreateImage3DDispatchPTypeScalar<numDims>(dims, ptype);
  } break;
  case CompoundType::CompoundType_3Planes: {
    return CreateImage3DDispatchPType3PLanes<numDims>(dims, ptype);
  } break;
  case CompoundType::CompoundType_4Planes: {
    return CreateImage3DDispatchPType4PLanes<numDims>(dims, ptype);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("Unsupported compound type:{}", ctype));
  }
  }
}

//! Factory to build contiguous dense image
std::unique_ptr<IInterface> Create(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype)
{
  POUTRE_ENTERING("CreateDense");
  const auto &numDims = dims.size();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Unsupported number of dims:0");
  } break;
  case 1: {
    return CreateImage1DDispatch<1>(dims, ctype, ptype);
  } break;
  case 2: {
    return CreateImage2DDispatch<2>(dims, ctype, ptype);
  } break;
  case 3: {
    return CreateImage3DDispatch<3>(dims, ctype, ptype);
  } break;
  case 4: {
    return CreateDenseDispatchDims<4>(dims, ctype, ptype);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("Unsupported number of dims");
  }
  }
}

/***********************************************************************************************************************/
/*                                       IMAGE FROM STRING */
/***********************************************************************************************************************/

template<ptrdiff_t dims, typename ptype>
void ImageFromStringDenseScalarDispatchPTypeHelper(poutre::IInterface &img, std::istringstream &istrm)
{
  using ImgType = details::image_t<typename TypeTraits<ptype>::storage_type, dims>;
  auto* img_t = dynamic_cast<ImgType *>(&img);
  typename TypeTraits<ptype>::str_type tmp;
  if (!img_t) {POUTRE_RUNTIME_ERROR("ImageFromStringDenseScalarDispatchPType:Dynamic cast failure");}
  for (auto &iter : (*img_t)) {
    istrm >> tmp;
    // todo check range validity
    if (istrm.fail()) {
      std::string err =
        "ImageFromStringDenseScalarDispatchPType: unable to "
        "read current pixel from "
        + istrm.str();
      POUTRE_RUNTIME_ERROR(err);
    }
    iter = static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
  }
}// namespace poutre

template<ptrdiff_t dims, typename ptype>
void ImageFromStringDense3PlanesDispatchPTypeHelper(poutre::IInterface &img, std::istringstream &istrm)
{
  using ImgType = details::image_t<typename TypeTraits<compound_type<ptype, 3>>::storage_type, dims>;
  auto *img_t = dynamic_cast<ImgType *>(&img);
  typename TypeTraits<ptype>::str_type tmp;
  if (!img_t) { POUTRE_RUNTIME_ERROR("ImageFromStringDenseScalarDispatchPType:Dynamic cast failure"); }

  for (auto &iter : (*img_t)) {
    istrm >> tmp;
    // todo check range validity ?
    if (istrm.fail()) {
      std::string err =
        "ImageFromStringDenseScalarDispatchPType: unable to "
        "read current pixel from "
        + istrm.str();
      POUTRE_RUNTIME_ERROR(err);
    }
    iter[0] = static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
    istrm >> tmp;
    // todo check range validity ?
    if (istrm.fail()) {
      std::string err =
        "ImageFromStringDenseScalarDispatchPType: unable to "
        "read current pixel from "
        + istrm.str();
      POUTRE_RUNTIME_ERROR(err);
    }
    iter[1] = static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
    // todo check range validity ?
    istrm >> tmp;
    if (istrm.fail()) {
      std::string err =
        "ImageFromStringDenseScalarDispatchPType: unable to "
        "read current pixel from "
        + istrm.str();
      POUTRE_RUNTIME_ERROR(err);
    }
    iter[2] = static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
  }
}

template<ptrdiff_t dims, typename ptype>
void ImageFromStringDense4PlanesDispatchPTypeHelper(poutre::IInterface &img, std::istringstream &istrm)
{
  using ImgType = details::image_t<typename TypeTraits<compound_type<ptype, 4>>::storage_type, dims>;
  auto *img_t = dynamic_cast<ImgType *>(&img);
  typename TypeTraits<ptype>::str_type tmp;
  if (!img_t) { POUTRE_RUNTIME_ERROR("ImageFromStringDenseScalarDispatchPType:Dynamic cast failure"); }
  for (auto &iter : (*img_t)) {
    istrm >> tmp;
    // todo check range validity ?
    if (istrm.fail()) {
      std::string err =
        "ImageFromStringDenseScalarDispatchPType: unable to "
        "read current pixel from "
        + istrm.str();
      POUTRE_RUNTIME_ERROR(err);
    }
    iter[0] = static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
    istrm >> tmp;
    // todo check range validity ?
    if (istrm.fail()) {
      std::string err =
        "ImageFromStringDenseScalarDispatchPType: unable to "
        "read current pixel from "
        + istrm.str();
      POUTRE_RUNTIME_ERROR(err);
    }
    iter[1] = static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
    // todo check range validity ?
    istrm >> tmp;
    if (istrm.fail()) {
      std::string err =
        "ImageFromStringDenseScalarDispatchPType: unable to "
        "read current pixel from "
        + istrm.str();
      POUTRE_RUNTIME_ERROR(err);
    }
    iter[2] = static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
    // todo check range validity ?
    istrm >> tmp;
    if (istrm.fail()) {
      std::string err =
        "ImageFromStringDenseScalarDispatchPType: unable to "
        "read current pixel from "
        + istrm.str();
      POUTRE_RUNTIME_ERROR(err);
    }
    iter[3] = static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
  }
}

template<size_t dims>
void ImageFromStringDense3PlanesDispatchPType(poutre::IInterface &img, poutre::PType ptype, std::istringstream &istrm)
{
  switch (ptype) {
  case PType::PType_GrayUINT8: {
    ImageFromStringDense3PlanesDispatchPTypeHelper<dims, pUINT8>(img, istrm);
  } break;
  case PType::PType_GrayINT32: {
    ImageFromStringDense3PlanesDispatchPTypeHelper<dims, pINT32>(img, istrm);
  } break;
  case PType::PType_GrayINT64: {
    ImageFromStringDense3PlanesDispatchPTypeHelper<dims, pINT64>(img, istrm);
  } break;
  case PType::PType_F32: {
    ImageFromStringDense3PlanesDispatchPTypeHelper<dims, pFLOAT>(img, istrm);
  } break;
  case PType::PType_D64: {
    ImageFromStringDense3PlanesDispatchPTypeHelper<dims, pDOUBLE>(img, istrm);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("ImageFromStringDenseScalarDispatchPType::Unsupported ptype:{}", ptype));
  }
  }// switch (ptype)
}

template<size_t dims>
void ImageFromStringDense4PlanesDispatchPType(poutre::IInterface &img, poutre::PType ptype, std::istringstream &istrm)
{
  switch (ptype) {
  case PType::PType_GrayUINT8: {
    ImageFromStringDense4PlanesDispatchPTypeHelper<dims, pUINT8>(img, istrm);
  } break;
  case PType::PType_GrayINT32: {
    ImageFromStringDense4PlanesDispatchPTypeHelper<dims, pINT32>(img, istrm);
  } break;
  case PType::PType_GrayINT64: {
    ImageFromStringDense4PlanesDispatchPTypeHelper<dims, pINT64>(img, istrm);
  } break;
  case PType::PType_F32: {
    ImageFromStringDense4PlanesDispatchPTypeHelper<dims, pFLOAT>(img, istrm);
  } break;
  case PType::PType_D64: {
    ImageFromStringDense4PlanesDispatchPTypeHelper<dims, pDOUBLE>(img, istrm);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("ImageFromStringDenseScalarDispatchPType::Unsupported ptype:{}", ptype));
  }
  }// switch (ptype)
}

template<size_t dims>
void ImageFromStringDenseScalarDispatchPType(poutre::IInterface &img, PType ptype, std::istringstream &istrm)
{
  switch (ptype) {
  case PType::PType_GrayUINT8: {
    ImageFromStringDenseScalarDispatchPTypeHelper<dims, pUINT8>(img, istrm);
  } break;
  case PType::PType_GrayINT32: {
    ImageFromStringDenseScalarDispatchPTypeHelper<dims, pINT32>(img, istrm);
  } break;
  case PType::PType_GrayINT64: {
    ImageFromStringDenseScalarDispatchPTypeHelper<dims, pINT64>(img, istrm);
  } break;
  case PType::PType_F32: {
    ImageFromStringDenseScalarDispatchPTypeHelper<dims, pFLOAT>(img, istrm);
  } break;
  case PType::PType_D64: {
    ImageFromStringDenseScalarDispatchPTypeHelper<dims, pDOUBLE>(img, istrm);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("ImageFromStringDenseScalarDispatchPType::Unsupported ptype:{}", ptype));
  }
  }// switch (ptype)
}

template<size_t dims>
void ImageFromStringDispatchDense(poutre::IInterface &img, CompoundType cptype, PType ptype, std::istringstream &istrm)
{
  switch (cptype) {
  case CompoundType::CompoundType_Scalar: {
    ImageFromStringDenseScalarDispatchPType<dims>(img, ptype, istrm);
  } break;

  case CompoundType::CompoundType_3Planes: {
    ImageFromStringDense3PlanesDispatchPType<dims>(img, ptype, istrm);
  } break;

  case CompoundType::CompoundType_4Planes: {
    ImageFromStringDense4PlanesDispatchPType<dims>(img, ptype, istrm);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("ImageFromStringDispatch::Unsupported compound type:{}", cptype));
  }
  }// switch (cptype)
}

std::unique_ptr<IInterface> ImageFromString(const std::string &i_str)
{
  POUTRE_ENTERING("ImageFromString");
  std::istringstream istrm(i_str);
  if (!istrm.good()) { POUTRE_RUNTIME_ERROR("ImageFromString: unable to read input string"); }

  CompoundType cptype{ CompoundType::CompoundType_Undef };
  PType ptype{ PType::PType_Undef };
  std::ptrdiff_t NumDims = -1;
  std::vector<std::size_t> dims;

  istrm >> cptype;
  if (istrm.fail()) {
    std::string err = "ImageFromString: unable to read CompoundType from " + i_str;
    POUTRE_RUNTIME_ERROR(err);
  }
  istrm >> ptype;
  if (istrm.fail()) {
    std::string err = "ImageFromString: unable to read PType from " + i_str;
    POUTRE_RUNTIME_ERROR(err);
  }
  istrm >> NumDims;

  if (istrm.fail()) {
    std::string err = "ImageFromString: unable to read NumDims from " + i_str;
    POUTRE_RUNTIME_ERROR(err);
  }

  if (!(NumDims > 0)) {
    std::string err = "ImageFromString: wrong value from NumDims " + i_str;
    POUTRE_RUNTIME_ERROR(err);
  }

  // from NumDims we know howto fill Dims
  dims.reserve(static_cast<std::size_t>(NumDims));
  ptrdiff_t current_dim = 0;
  for (ptrdiff_t i = 0; i < NumDims; i++) {
    istrm >> current_dim;
    if (istrm.fail()) {
      std::string err = "ImageFromString: unable to read dim from " + i_str;
      POUTRE_RUNTIME_ERROR(err);
    }
    dims.push_back(static_cast<std::size_t>(current_dim));
  }

  // create image
  std::unique_ptr<poutre::IInterface> img = Create(dims, cptype, ptype);

  // Dispatch
  switch (NumDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Unsupported number of dims:0");
  } break;
  case 1: {
    ImageFromStringDispatchDense<1>(*img, cptype, ptype, istrm);
  } break;
  case 2: {
    ImageFromStringDispatchDense<2>(*img, cptype, ptype, istrm);
  } break;
  case 3: {
    ImageFromStringDispatchDense<3>(*img, cptype, ptype, istrm);
  } break;
  case 4: {
    ImageFromStringDispatchDense<4>(*img, cptype, ptype, istrm);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("Unsupported number of dims");
  }
  }

  return img;
}


/***********************************************************************************************************************/
/*                                       IMAGE TO STRING */
/***********************************************************************************************************************/

// IMAGE TO STRING DISPATCH PTYPE
template<ptrdiff_t dims, typename ptype>
void ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper(const poutre::IInterface &img, std::ostringstream &ostrm)
{
  using ImgType = details::image_t<typename TypeTraits<compound_type<ptype, 4>>::storage_type, dims>;
  const auto *img_t = dynamic_cast<const ImgType *>(&img);
  if (!img_t) {
    POUTRE_RUNTIME_ERROR(
      "ImageToStringDenseCompoundType_"
      "4PlanesDispatchPType:Dynamic cast failure");
  }
  for (auto it = (*img_t).cbegin(); it != (*img_t).cend(); ++it) {
    for (ptrdiff_t i = 0; i < 4; ++i) {
      ostrm << static_cast<typename poutre::TypeTraits<ptype>::str_type>((*it)[i]) << " ";
    }
  }
  // ostrm.seekp(-1, ostrm.cur);
  std::string tmp = ostrm.str();
  tmp.pop_back();
  ostrm.str(tmp);
  ostrm.seekp(0, ostrm.end);
}

template<ptrdiff_t dims, typename ptype>
void ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper(const poutre::IInterface &img, std::ostringstream &ostrm)
{
  using ImgType = details::image_t<typename TypeTraits<compound_type<ptype, 3>>::storage_type, dims>;
  const auto *img_t = dynamic_cast<const ImgType *>(&img);
  if (!img_t) {
    POUTRE_RUNTIME_ERROR(
      "ImageToStringDenseCompoundType_"
      "3PlanesDispatchPType:Dynamic cast failure");
  }
  for (auto it = (*img_t).cbegin(); it != (*img_t).cend(); ++it) {
    for (ptrdiff_t i = 0; i < 3; ++i) {
      ostrm << static_cast<typename poutre::TypeTraits<ptype>::str_type>((*it)[i]) << " ";
    }
  }
  // ostrm.seekp(-1, ostrm.cur);
  std::string tmp = ostrm.str();
  tmp.pop_back();
  ostrm.str(tmp);
  ostrm.seekp(0, ostrm.end);
}

template<ptrdiff_t dims, typename ptype>
void ImageToStringDenseScalarDispatchPTypeHelper(const poutre::IInterface &img, std::ostringstream &ostrm)
{
  using ImgType = details::image_t<typename TypeTraits<ptype>::storage_type, dims>;
  const auto *img_t = dynamic_cast<const ImgType *>(&img);
  if (!img_t) { POUTRE_RUNTIME_ERROR("ImageToStringDenseScalarDispatchPTypeHelper:Dynamic cast failure"); }
  //   const ptrdiff_t len = img_t->size();
  //   for (ptrdiff_t i = 0; i < len - 1; ++i) {
  //     ostrm << (typename poutre::TypeTraits<ptype>::str_type)(*img_t)[i] << "
  //     ";
  //   }
  //   if (len > 0)
  //     ostrm << (typename poutre::TypeTraits<ptype>::str_type)(*img_t)[len -
  //     1];
  for (auto it = (*img_t).cbegin(); it != (*img_t).cend(); ++it) {
    ostrm << static_cast<typename poutre::TypeTraits<ptype>::str_type>(*it) << " ";
  }
  // ostrm.seekp(-1, ostrm.cur);
  // ostrm.seekp(-1, std::ios_base::end);
  // ostrm.seekp(-1, std::ios_base::end);
  // ostrm << '\0';
  std::string tmp = ostrm.str();
  tmp.pop_back();
  ostrm.str(tmp);
  ostrm.seekp(0, ostrm.end);
}

// IMAGE TO STRING DISPATCH CTYPE
template<size_t dims>
void ImageToStringDenseScalarDispatchPType(const poutre::IInterface &img, PType ptype, std::ostringstream &ostrm)
{
  switch (ptype) {
  case PType::PType_GrayUINT8: {
    ImageToStringDenseScalarDispatchPTypeHelper<dims, poutre::pUINT8>(img, ostrm);
  } break;

  case PType::PType_GrayINT32: {
    ImageToStringDenseScalarDispatchPTypeHelper<dims, poutre::pINT32>(img, ostrm);
  } break;
  case PType::PType_GrayINT64: {
    ImageToStringDenseScalarDispatchPTypeHelper<dims, poutre::pINT64>(img, ostrm);
  } break;
  case PType::PType_F32: {
    ImageToStringDenseScalarDispatchPTypeHelper<dims, poutre::pFLOAT>(img, ostrm);
  } break;
  case PType::PType_D64: {
    ImageToStringDenseScalarDispatchPTypeHelper<dims, poutre::pDOUBLE>(img, ostrm);
  } break;

  default: {
    POUTRE_RUNTIME_ERROR(std::format("ImageToStringDenseScalarDispatchPType::Unsupported pixel type:{}", ptype));
  }
  }// switch (ptype)
}

template<size_t dims>
void ImageToStringDenseCompoundType_3PlanesDispatchPType(const poutre::IInterface &img,
  PType ptype,
  std::ostringstream &ostrm)
{
  switch (ptype) {
  case PType::PType_GrayUINT8: {
    ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper<dims, poutre::pUINT8>(img, ostrm);
  } break;

  case PType::PType_GrayINT32: {
    ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper<dims, poutre::pINT32>(img, ostrm);
  } break;
  case PType::PType_GrayINT64: {
    ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper<dims, poutre::pINT64>(img, ostrm);
  } break;
  case PType::PType_F32: {
    ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper<dims, poutre::pFLOAT>(img, ostrm);
  } break;
  case PType::PType_D64: {
    ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper<dims, poutre::pDOUBLE>(img, ostrm);
  } break;

  default: {
    POUTRE_RUNTIME_ERROR(std::format("ImageToStringDenseScalarDispatchPType::Unsupported pixel type:{}", ptype));
  }
  }// switch (ptype)
}

template<size_t dims>
void ImageToStringDenseCompoundType_4PlanesDispatchPType(const poutre::IInterface &img,
  PType ptype,
  std::ostringstream &ostrm)
{
  switch (ptype) {
  case PType::PType_GrayUINT8: {
    ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper<dims, poutre::pUINT8>(img, ostrm);
  } break;

  case PType::PType_GrayINT32: {
    ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper<dims, poutre::pINT32>(img, ostrm);
  } break;
  case PType::PType_GrayINT64: {
    ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper<dims, poutre::pINT64>(img, ostrm);
  } break;
  case PType::PType_F32: {
    ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper<dims, poutre::pFLOAT>(img, ostrm);
  } break;
  case PType::PType_D64: {
    ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper<dims, poutre::pDOUBLE>(img, ostrm);
  } break;

  default: {
    POUTRE_RUNTIME_ERROR(std::format("ImageToStringDenseScalarDispatchPType::Unsupported pixel type:{}", ptype));
  }
  }// switch (ptype)
}


template<size_t dims>
void ImageToStringDispatchDense(const poutre::IInterface &img,
  CompoundType cptype,
  PType ptype,
  std::ostringstream &ostrm)
{
  switch (cptype) {
  case CompoundType::CompoundType_Scalar: {
    ImageToStringDenseScalarDispatchPType<dims>(img, ptype, ostrm);
  } break;

  case CompoundType::CompoundType_3Planes: {
    ImageToStringDenseCompoundType_3PlanesDispatchPType<dims>(img, ptype, ostrm);
  } break;

  case CompoundType::CompoundType_4Planes: {
    ImageToStringDenseCompoundType_4PlanesDispatchPType<dims>(img, ptype, ostrm);
  } break;

  default: {
    POUTRE_RUNTIME_ERROR(std::format("ImageToStringDispatch::Unsupported compound type:{}", cptype));
  }
  }// switch (cptype)
}

// IMAGE TO STRING
std::string ImageToString(const poutre::IInterface &i_image)// NOLINT(misc-unused-parameters)
{
  POUTRE_ENTERING("ImageToString");
  std::ostringstream ostrm;
  if (!ostrm.good()) { POUTRE_RUNTIME_ERROR("ImageToString: unable to create ostrm"); }
  const auto cptype = i_image.GetCType();
  const auto ptype = i_image.GetPType();
  const auto numdim = i_image.GetRank();

  ostrm << cptype << " ";
  ostrm << ptype << " ";
  ostrm << numdim << " ";
  for (auto dim : i_image.GetShape()) { ostrm << dim << " "; }

  // Dispatch
  switch (numdim) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Unsupported number of dims:0");
  } break;
  case 1: {
    ImageToStringDispatchDense<1>(i_image, cptype, ptype, ostrm);
  } break;
  case 2: {
    ImageToStringDispatchDense<2>(i_image, cptype, ptype, ostrm);
  } break;
  case 3: {
    ImageToStringDispatchDense<3>(i_image, cptype, ptype, ostrm);
  } break;
  case 4: {
    ImageToStringDispatchDense<4>(i_image, cptype, ptype, ostrm);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("Unsupported number of dims");
  }
  }
  return ostrm.str();
}

}// namespace poutre