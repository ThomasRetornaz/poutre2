
// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/base/types.hpp>
#include <poutre/io/details/oiio.hpp>
#include <poutre/io/io.hpp>

#include <OpenImageIO/imageio.h>

#include <cstddef>
#include <filesystem>
#include <format>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace poutre::io {

void LoadFromOIIOScalarDispatch(OIIO::ImageInput &in_oiio, poutre::IInterface &img, PType ptype)
{
  switch (ptype) {
    // todo think about bool/binary here
  case PType::PType_GrayUINT8: {
    using ImageType_t = poutre::details::image_t<pUINT8, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOScalar(in_oiio, *img_t);
  } break;
  case PType::PType_GrayINT32: {
    using ImageType_t = poutre::details::image_t<pINT32, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOScalar(in_oiio, *img_t);
  } break;
  case PType::PType_F32: {
    using ImageType_t = poutre::details::image_t<pFLOAT, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOScalar(in_oiio, *img_t);
  } break;
  // case PType::PType_GrayINT64: {
  //   using ImageType_t  = poutre::details::image_t<pINT64, 2>;
  //   auto *img_t = dynamic_cast<ImageType_t *>(&img);
  //   if( img_t == nullptr )
  //   {
  //     POUTRE_RUNTIME_ERROR("Dynamic cast fail");
  //   }
  //   details::FillImageFromOIIOScalar(in_oiio, *img_t);
  // }
  //   break;
  case PType::PType_D64: {
    using ImageType_t = poutre::details::image_t<pDOUBLE, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOScalar(in_oiio, *img_t);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("LoadFromOIIOScalarDispatch:: Unsupported scalar type:{}", ptype));
  }
  }
}
void LoadFromOIIO3ChannelsDispatch(OIIO::ImageInput &in_oiio, poutre::IInterface &img, PType ptype)
{
  switch (ptype) {
    // todo think about bool/binary here
  case PType::PType_GrayUINT8: {
    using ImageType_t = poutre::details::image_t<compound_type<pUINT8, 3>, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOCompound3(in_oiio, *img_t);
  } break;
  case PType::PType_GrayINT32: {
    using ImageType_t = poutre::details::image_t<compound_type<pINT32, 3>, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOCompound3(in_oiio, *img_t);
  } break;
  case PType::PType_F32: {
    using ImageType_t = poutre::details::image_t<compound_type<pFLOAT, 3>, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOCompound3(in_oiio, *img_t);
  } break;
  // case PType::PType_GrayINT64: {
  //   using ImageType_t  = poutre::details::image_t<compound_type<pINT64,3>, 2>;
  //   auto *img_t = dynamic_cast<ImageType_t *>(&img);
  //   if( img_t == nullptr )
  //   {
  //     POUTRE_RUNTIME_ERROR("Dynamic cast fail");
  //   }
  //   details::FillImageFromOIIOCompound3(in_oiio, *img_t);
  // }
  //   break;
  case PType::PType_D64: {
    using ImageType_t = poutre::details::image_t<compound_type<pDOUBLE, 3>, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOCompound3(in_oiio, *img_t);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("LoadFromOIIO3ChannelsDispatch:: Unsupported scalar type:{}", ptype));
  }
  }
}
void LoadFromOIIO4ChannelsDispatch(OIIO::ImageInput &in_oiio, poutre::IInterface &img, PType ptype)
{
  switch (ptype) {
    // todo think about bool/binary here
  case PType::PType_GrayUINT8: {
    using ImageType_t = poutre::details::image_t<compound_type<pUINT8, 4>, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOCompound4(in_oiio, *img_t);
  } break;
  case PType::PType_GrayINT32: {
    using ImageType_t = poutre::details::image_t<compound_type<pINT32, 4>, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOCompound4(in_oiio, *img_t);
  } break;
  case PType::PType_F32: {
    using ImageType_t = poutre::details::image_t<compound_type<pFLOAT, 4>, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOCompound4(in_oiio, *img_t);
  } break;
  // case PType::PType_GrayINT64: {
  //   using ImageType_t  = poutre::details::image_t<compound_type<pINT64,4>, 2>;
  //   auto *img_t = dynamic_cast<ImageType_t *>(&img);
  //   if( img_t == nullptr )
  //   {
  //     POUTRE_RUNTIME_ERROR("Dynamic cast fail");
  //   }
  //   details::FillImageFromOIIOCompound4(in_oiio, *img_t);
  // }
  //   break;
  case PType::PType_D64: {
    using ImageType_t = poutre::details::image_t<compound_type<pDOUBLE, 4>, 2>;
    auto *img_t = dynamic_cast<ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::FillImageFromOIIOCompound4(in_oiio, *img_t);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("LoadFromOIIO4ChannelsDispatch:: Unsupported scalar type:{}", ptype));
  }
  }
}

void StoreWithOIIOScalarDispatch(const IInterface &img,
  const std::string &path,
  details::StoreWithOIIOOptions const &options,
  PType ptype)
{
  switch (ptype) {
    // todo think about bool/binary here
  case PType::PType_GrayUINT8: {
    using ImageType_t = poutre::details::image_t<pUINT8, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOScalar(*img_t, path, options);
  } break;
  case PType::PType_GrayINT32: {
    using ImageType_t = poutre::details::image_t<pINT32, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOScalar(*img_t, path, options);
  } break;
  case PType::PType_F32: {
    using ImageType_t = poutre::details::image_t<pFLOAT, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOScalar(*img_t, path, options);
  } break;
  // case PType::PType_GrayINT64: {
  //   using ImageType_t  = poutre::details::image_t<pINT64, 2>;
  //   const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
  //   if( img_t == nullptr )
  //   {
  //     POUTRE_RUNTIME_ERROR("Dynamic cast fail");
  //   }
  //   details::StoreWithOIIOScalar(*img_t, path, options);
  // }
  //   break;
  case PType::PType_D64: {
    using ImageType_t = poutre::details::image_t<pDOUBLE, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOScalar(*img_t, path, options);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("StoreWithOIIOScalar:: Unsupported scalar type:{}", ptype));
  }
  }
}

void StoreWithOIIO3PlanesDispatch(const IInterface &img,
  const std::string &path,
  details::StoreWithOIIOOptions const &options,
  PType ptype)
{
  switch (ptype) {
    // todo think about bool/binary here
  case PType::PType_GrayUINT8: {
    using ImageType_t = poutre::details::image_t<compound_type<pUINT8, 3>, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOCompound3(*img_t, path, options);
  } break;
  case PType::PType_GrayINT32: {
    using ImageType_t = poutre::details::image_t<compound_type<pINT32, 3>, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOCompound3(*img_t, path, options);
  } break;
  case PType::PType_F32: {
    using ImageType_t = poutre::details::image_t<compound_type<pFLOAT, 3>, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOCompound3(*img_t, path, options);
  } break;
  // case PType::PType_GrayINT64: {
  //   using ImageType_t  = poutre::details::image_t<compound_type<pINT64,3>, 2>;
  //   const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
  //   if( img_t == nullptr )
  //   {
  //     POUTRE_RUNTIME_ERROR("Dynamic cast fail");
  //   }
  //   details::StoreWithOIIOCompound3(*img_t, path, options);
  // }
  //   break;
  case PType::PType_D64: {
    using ImageType_t = poutre::details::image_t<compound_type<pDOUBLE, 3>, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOCompound3(*img_t, path, options);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("StoreWithOIIO3PlanesDispatch:: Unsupported scalar type:{}", ptype));
  }
  }
}

void StoreWithOIIO4PlanesDispatch(const IInterface &img,
  const std::string &path,
  details::StoreWithOIIOOptions const &options,
  PType ptype)
{
  switch (ptype) {
    // todo think about bool/binary here
  case PType::PType_GrayUINT8: {
    using ImageType_t = poutre::details::image_t<compound_type<pUINT8, 4>, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOCompound4(*img_t, path, options);
  } break;
  case PType::PType_GrayINT32: {
    using ImageType_t = poutre::details::image_t<compound_type<pINT32, 4>, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOCompound4(*img_t, path, options);
  } break;
  case PType::PType_F32: {
    using ImageType_t = poutre::details::image_t<compound_type<pFLOAT, 4>, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOCompound4(*img_t, path, options);
  } break;
  // case PType::PType_GrayINT64: {
  //   using ImageType_t  = poutre::details::image_t<compound_type<pINT64,4>, 2>;
  //   const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
  //   if( img_t == nullptr )
  //   {
  //     POUTRE_RUNTIME_ERROR("Dynamic cast fail");
  //   }
  //   details::StoreWithOIIOCompound4(*img_t, path, options);
  // }
  //   break;
  case PType::PType_D64: {
    using ImageType_t = poutre::details::image_t<compound_type<pDOUBLE, 4>, 2>;
    const auto *img_t = dynamic_cast<const ImageType_t *>(&img);
    if (img_t == nullptr) { POUTRE_RUNTIME_ERROR("Dynamic cast fail"); }
    details::StoreWithOIIOCompound4(*img_t, path, options);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("StoreWithOIIO4PlanesDispatch:: Unsupported scalar type:{}", ptype));
  }
  }
}

namespace fs = std::filesystem;

std::unique_ptr<IInterface> LoadOIIO(const std::string &image_path)
{
  POUTRE_ENTERING("LoadFromOIIO");
  fs::path const localPath(image_path);
  if (!fs::exists(localPath)) {
    POUTRE_RUNTIME_ERROR(std::format("LoadFromOIIO: unable to locate provided path {}", localPath.string()));
  }

  auto in_oiio(OIIO::ImageInput::open(image_path));
  if (in_oiio == nullptr) {
    std::ostringstream error_stream;
    error_stream << " load_image(): Error reading image '";
    error_stream << image_path;
    POUTRE_RUNTIME_ERROR(error_stream.str());
  }

  const OIIO::ImageSpec &spec = in_oiio->spec();
  std::vector<std::size_t> dims;
  if (spec.height > 1) { dims.push_back(static_cast<std::size_t>(spec.height)); }
  if (spec.width > 1) { dims.push_back(static_cast<std::size_t>(spec.width)); }
  if (spec.depth > 1) {
    POUTRE_RUNTIME_ERROR(std::format("LoadFromOIIO: nD with n >2  format require hdf5 ext, use LoadFromHDF5"));
  }
  if (!spec.channelformats.empty()) {
    POUTRE_RUNTIME_ERROR(std::format("LoadFromOIIO: different format over channels is not supported"));
  }
  CompoundType ctype = CompoundType::CompoundType_Undef;
  switch (spec.nchannels) {
  case 1:
    ctype = CompoundType::CompoundType_Scalar;
    break;
  case 3:
    ctype = CompoundType::CompoundType_3Planes;
    break;
  case 4:
    ctype = CompoundType::CompoundType_4Planes;
    break;
  default:
    std::ostringstream error_stream;
    error_stream << " load_image(): Error reading image '";
    error_stream << image_path;
    error_stream << " unsupported number of channels ";
    error_stream << spec.nchannels;
    error_stream << " see desc \n" << spec.to_xml();
    POUTRE_RUNTIME_ERROR(error_stream.str());
  };
  auto ptype = PType::PType_Undef;
  switch (spec.format.basetype) {
  case OpenImageIO_v2_5::TypeDesc::BASETYPE::UINT8:
    ptype = PType::PType_GrayUINT8;
    break;
  case OpenImageIO_v2_5::TypeDesc::BASETYPE::INT32:
    ptype = PType::PType_GrayINT32;
    break;
  case OpenImageIO_v2_5::TypeDesc::BASETYPE::FLOAT:
    ptype = PType::PType_F32;
    break;
  case OpenImageIO_v2_5::TypeDesc::BASETYPE::INT64:
    ptype = PType::PType_GrayINT64;
    break;
  case OpenImageIO_v2_5::TypeDesc::BASETYPE::DOUBLE:
    ptype = PType::PType_D64;
    break;
  default:
    std::ostringstream error_stream;
    error_stream << " load_image(): Error reading image '";
    error_stream << image_path;
    error_stream << " unsupported type ";
    error_stream << static_cast<unsigned int>(spec.format.basetype);
    error_stream << " see desc \n" << spec.to_xml();
    POUTRE_RUNTIME_ERROR(error_stream.str());
  };
  auto i_img = poutre::Create(dims, ctype, ptype);
  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    LoadFromOIIOScalarDispatch(*in_oiio, *i_img, ptype);
  } break;
  case CompoundType::CompoundType_3Planes: {
    LoadFromOIIO3ChannelsDispatch(*in_oiio, *i_img, ptype);
  } break;
  case CompoundType::CompoundType_4Planes: {
    LoadFromOIIO4ChannelsDispatch(*in_oiio, *i_img, ptype);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("Unsupported compound type:{}", ctype));
  }
  }

  // now fill it
  in_oiio->close();

  return i_img;
}

void DumpOIIO(const std::string &path, const IInterface &i_img)// NOLINT(misc-unused-parameters)
{
  POUTRE_ENTERING("StoreWithOIIO");
  const details::StoreWithOIIOOptions options;
  fs::path const localPath(path);
  fs::path const dir = localPath.parent_path();
  if (!(fs::exists(dir))) {
    POUTRE_RUNTIME_ERROR(std::format("StoreFromOIIO: provided path {} doesn't exists", dir.string()));
  }
  if (i_img.GetRank() > 2) { POUTRE_RUNTIME_ERROR(std::format("For nd images with n>2 use StoreWithHDF5")); }

  auto ptype = i_img.GetPType();
  auto ctype = i_img.GetCType();

  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    StoreWithOIIOScalarDispatch(i_img, path, options, ptype);
  } break;
  case CompoundType::CompoundType_3Planes: {
    StoreWithOIIO3PlanesDispatch(i_img, path, options, ptype);
  } break;
  case CompoundType::CompoundType_4Planes: {
    StoreWithOIIO4PlanesDispatch(i_img, path, options, ptype);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(std::format("Unsupported compound type:{}", ctype));
  }
  }
}
}// namespace poutre::io