
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
#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/base/types.hpp>
#include <poutre/io/io.hpp>
#include <poutre/io/details/hdf5.hpp>

#include <H5Exception.h>
#include <H5File.h>
#include <H5DataSet.h>
#include <H5Attribute.h>
#include <H5DataSpace.h>
#include <H5public.h>
#include <H5Fpublic.h>
#include <H5StrType.h>

#include <algorithm>
#include <format>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace poutre::io {

// NOLINTBEGIN
template <ptrdiff_t dim>
void StoreWithHDF5ScalarDispatch(const std::string &path,
                     const IInterface & iimage,
                     const std::string &image_name,
                     PType ptype)
{
switch( ptype ) {
  case PType::PType_GrayUINT8: details::StoreWithHDF5_helper<pUINT8, dim>(iimage, path, image_name); break;
  case PType::PType_GrayINT32: details::StoreWithHDF5_helper<pINT32, dim>(iimage, path, image_name); break;
  case PType::PType_GrayINT64: details::StoreWithHDF5_helper<pINT64, dim>(iimage, path, image_name); break;
  case PType::PType_F32: details::StoreWithHDF5_helper<pFLOAT, dim>(iimage, path, image_name); break;
  case PType::PType_D64: details::StoreWithHDF5_helper<pDOUBLE, dim>(iimage, path, image_name); break;
  default: POUTRE_RUNTIME_ERROR((std::format("StoreWithHDF5: unsupported pTYpe {}", ptype)));
  }
}

template <ptrdiff_t dim>
void StoreWithHDF53PlanesDispatch(const std::string &path,
                     const IInterface & iimage,
                     const std::string &image_name,
                     PType ptype)
{
switch( ptype ) {
case PType::PType_GrayUINT8: details::StoreWithHDF53Planes_helper<pUINT8, dim>(iimage, path, image_name); break;
case PType::PType_GrayINT32: details::StoreWithHDF53Planes_helper<pINT32, dim>(iimage, path, image_name); break;
case PType::PType_GrayINT64: details::StoreWithHDF53Planes_helper<pINT64, dim>(iimage, path, image_name); break;
case PType::PType_F32: details::StoreWithHDF53Planes_helper<pFLOAT, dim>(iimage, path, image_name); break;
case PType::PType_D64: details::StoreWithHDF53Planes_helper<pDOUBLE, dim>(iimage, path, image_name); break;
default: POUTRE_RUNTIME_ERROR((std::format("StoreWithHDF5: unsupported pTYpe {}", ptype)));
}
}

template <ptrdiff_t dim>
void StoreWithHDF54PlanesDispatch(const std::string &path,
                     const IInterface & iimage,
                     const std::string &image_name,
                     PType ptype)
{
  switch( ptype ) {
  case PType::PType_GrayUINT8: details::StoreWithHDF54Planes_helper<pUINT8, dim>(iimage, path, image_name); break;
  case PType::PType_GrayINT32: details::StoreWithHDF54Planes_helper<pINT32, dim>(iimage, path, image_name); break;
  case PType::PType_GrayINT64: details::StoreWithHDF54Planes_helper<pINT64, dim>(iimage, path, image_name); break;
  case PType::PType_F32: details::StoreWithHDF54Planes_helper<pFLOAT, dim>(iimage, path, image_name); break;
  case PType::PType_D64: details::StoreWithHDF54Planes_helper<pDOUBLE, dim>(iimage, path, image_name); break;
  default: POUTRE_RUNTIME_ERROR((std::format("StoreWithHDF5: unsupported pTYpe {}", ptype)));
  }
}
template <ptrdiff_t dim>
void LoadFromHDF5Dispatch(IInterface &iimage, const H5::DataSet &data_set, PType ptype)
{
  switch( ptype ) {
  case PType::PType_GrayUINT8: details::LoadFromHDF5_helper<pUINT8, dim>(iimage, data_set); break;
  case PType::PType_GrayINT32: details::LoadFromHDF5_helper<pINT32, dim>(iimage, data_set); break;
  case PType::PType_GrayINT64: details::LoadFromHDF5_helper<pINT64, dim>(iimage, data_set); break;
  case PType::PType_F32: details::LoadFromHDF5_helper<pFLOAT, dim>(iimage, data_set); break;
  case PType::PType_D64: details::LoadFromHDF5_helper<pDOUBLE, dim>(iimage, data_set); break;
  default: POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF5: unsupported pTYpe {}", ptype)));
  }
}

template <ptrdiff_t dim>
void LoadFromHDF53PlanesDispatch(IInterface &iimage, const H5::DataSet &data_set, PType ptype)
{
  switch( ptype ) {
  case PType::PType_GrayUINT8: details::LoadFromHDF53Planes_helper<pUINT8, dim>(iimage, data_set); break;
  case PType::PType_GrayINT32: details::LoadFromHDF53Planes_helper<pINT32, dim>(iimage, data_set); break;
  case PType::PType_GrayINT64: details::LoadFromHDF53Planes_helper<pINT64, dim>(iimage, data_set); break;
  case PType::PType_F32: details::LoadFromHDF53Planes_helper<pFLOAT, dim>(iimage, data_set); break;
  case PType::PType_D64: details::LoadFromHDF53Planes_helper<pDOUBLE, dim>(iimage, data_set); break;
  default: POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF5: unsupported pTYpe {}", ptype)));
  }
}

template <ptrdiff_t dim>
void LoadFromHDF54PlanesDispatch(IInterface &iimage, const H5::DataSet &data_set, PType ptype)
{
  switch( ptype ) {
  case PType::PType_GrayUINT8: details::LoadFromHDF54Planes_helper<pUINT8, dim>(iimage, data_set); break;
  case PType::PType_GrayINT32: details::LoadFromHDF54Planes_helper<pINT32, dim>(iimage, data_set); break;
  case PType::PType_GrayINT64: details::LoadFromHDF54Planes_helper<pINT64, dim>(iimage, data_set); break;
  case PType::PType_F32: details::LoadFromHDF54Planes_helper<pFLOAT, dim>(iimage, data_set); break;
  case PType::PType_D64: details::LoadFromHDF54Planes_helper<pDOUBLE, dim>(iimage, data_set); break;
  default: POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF5: unsupported pTYpe {}", ptype)));
  }
}

// NOLINTEND

void DumpHDF5(const std::string &path,
                     const IInterface & iimage, // NOLINT
                     const std::string &image_name)
{
  POUTRE_ENTERING("StoreWithHDF5");
  /*
   * Turn off the auto-printing when failure occurs so that we can
   * handle the errors appropriately
   */
  H5::Exception::dontPrint();

  auto       ptype  = iimage.GetPType();
  auto       ctype  = iimage.GetCType();
  switch ( const auto rank = iimage.GetRank() )
  {
  case 1: {
    switch( ctype ) {
    case CompoundType::CompoundType_Scalar:
      StoreWithHDF5ScalarDispatch<1>(path,iimage,image_name,ptype);
      break;
    case CompoundType::CompoundType_3Planes:
      StoreWithHDF53PlanesDispatch<1>(path,iimage,image_name,ptype);
      break;
    case CompoundType::CompoundType_4Planes:
      StoreWithHDF54PlanesDispatch<1>(path,iimage,image_name,ptype);
      break;
    default: POUTRE_RUNTIME_ERROR((std::format("StoreWithHDF5: unsupported cTYpe {}", ctype)));
    }
  }break;
  case 2: {
    switch( ctype ) {
    case CompoundType::CompoundType_Scalar:
      StoreWithHDF5ScalarDispatch<2>(path,iimage,image_name,ptype);
      break;
    case CompoundType::CompoundType_3Planes:
      StoreWithHDF53PlanesDispatch<2>(path,iimage,image_name,ptype);
      break;
    case CompoundType::CompoundType_4Planes:
      StoreWithHDF54PlanesDispatch<2>(path,iimage,image_name,ptype);
      break;
    default: POUTRE_RUNTIME_ERROR((std::format("StoreWithHDF5: unsupported cTYpe {}", ctype)));
    }
  }break;
  case 3: {
    switch( ctype ) {
    case CompoundType::CompoundType_Scalar:
      StoreWithHDF5ScalarDispatch<3>(path,iimage,image_name,ptype);
      break;
    case CompoundType::CompoundType_3Planes:
      StoreWithHDF53PlanesDispatch<3>(path,iimage,image_name,ptype);
      break;
    case CompoundType::CompoundType_4Planes:
      StoreWithHDF54PlanesDispatch<3>(path,iimage,image_name,ptype);
      break;
    default: POUTRE_RUNTIME_ERROR((std::format("StoreWithHDF5: unsupported cTYpe {}", ctype)));
    }
  }break;
  default: POUTRE_RUNTIME_ERROR((std::format("StoreWithHDF5: unsupported number of dims {}", rank)));
  } // dims
}


std::unique_ptr<IInterface>
  LoadHDF5(const std::string &path,                        // NOLINT
               const std::string &image_name /* = poutre_img_1 */) // NOLINT
{
  POUTRE_ENTERING("LoadFromHDF5");
  /*
   * Turn off the auto-printing when failure occurs so that we can
   * handle the errors appropriately
   */
  H5::Exception::dontPrint();

  PType        ptype = PType::PType_Undef;
  CompoundType ctype = CompoundType::CompoundType_Undef;
  auto         res   = std::unique_ptr<IInterface>();

  H5::H5File  const file(path.c_str(), H5F_ACC_RDONLY);
  H5::DataSet const dataset = file.openDataSet(image_name.c_str());

  // Handle sizes
  H5::DataSpace        const dataspace = dataset.getSpace();
  int                  const rank      = dataspace.getSimpleExtentNdims();
  std::vector<hsize_t> dims_out(static_cast<std::size_t>(rank));
  int                  const ndims = dataspace.getSimpleExtentDims(dims_out.data(), nullptr);

  // by hand attribute
  // get compound_type
  H5std_string  attrTypeCompound;
  H5::Attribute resAttr = dataset.openAttribute("IMAGE_COMP_TYPE");
  H5::StrType   stype   = resAttr.getStrType();
  resAttr.read(stype, attrTypeCompound);
  if( attrTypeCompound == details::IMAGEScalar ) {
    ctype = CompoundType::CompoundType_Scalar;
  } else if( attrTypeCompound == details::IMAGE3Planes ) {
    ctype = CompoundType::CompoundType_3Planes;
  } else if( attrTypeCompound == details::IMAGE4Planes ) {
    ctype = CompoundType::CompoundType_4Planes;
  } else
  {
    POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF5: unsupported compound type {}", attrTypeCompound)));
  }
  // get ptype
  H5std_string attrpType;
  resAttr = dataset.openAttribute("IMAGE_P_TYPE");
  stype   = resAttr.getStrType();
  resAttr.read(stype, attrpType);
  if( attrpType == details::PUINT8 ) {
    ptype = PType::PType_GrayUINT8;
  } else if( attrpType == details::PINT32 ) {
    ptype = PType::PType_GrayINT32;
  } else if( attrpType == details::PINT64 ) {
    ptype = PType::PType_GrayINT64;
  } else if( attrpType == details::PFLOAT ) {
    ptype = PType::PType_F32;
  } else if( attrpType == details::PDOUBLE ) {
    ptype = PType::PType_D64;
  } else
  {
    POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF5: unsupported type {}", attrpType)));
  }

  std::vector<size_t> coords(dims_out.size());
  std::ranges::copy(dims_out, coords.begin());
  res = Create(coords, ctype, ptype);

  switch ( ndims )
  {
  case 1: {
    switch( ctype ) {
    case CompoundType::CompoundType_Scalar:
      LoadFromHDF5Dispatch<1>(*res, dataset, ptype);
      break;
    case CompoundType::CompoundType_3Planes:
      LoadFromHDF53PlanesDispatch<1>(*res, dataset, ptype);
      break;
    case CompoundType::CompoundType_4Planes:
      LoadFromHDF54PlanesDispatch<1>(*res, dataset, ptype);
      break;
    default: POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF5: unsupported cTYpe {}", ctype)));
    }
  }break;
  case 2: {
    switch( ctype ) {
    case CompoundType::CompoundType_Scalar:
      LoadFromHDF5Dispatch<2>(*res, dataset, ptype);
      break;
    case CompoundType::CompoundType_3Planes:
      LoadFromHDF53PlanesDispatch<2>(*res, dataset, ptype);
      break;
    case CompoundType::CompoundType_4Planes:
      LoadFromHDF54PlanesDispatch<2>(*res, dataset, ptype);
      break;
    default: POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF5: unsupported cTYpe {}", ctype)));
    }
  }break;
  case 3: {
    switch( ctype ) {
    case CompoundType::CompoundType_Scalar:
      LoadFromHDF5Dispatch<3>(*res, dataset, ptype);
      break;
    case CompoundType::CompoundType_3Planes:
      LoadFromHDF53PlanesDispatch<3>(*res, dataset, ptype);
      break;
    case CompoundType::CompoundType_4Planes:
      LoadFromHDF54PlanesDispatch<3>(*res, dataset, ptype);
      break;
    default: POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF5: unsupported cTYpe {}", ctype)));
    }
  }break;
  default: POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF5: unsupported number of dims {}", rank)));
  }
  return res;
}
}

