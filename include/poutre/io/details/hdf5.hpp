//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   hdf5.hpp
 * @author Thomas Retornaz
 * @brief  hdf5 load/save integrations
 *
 *
 */


#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/trace.hpp>

// NOLINTBEGIN
#include <H5pubconf.h>
#include <H5Cpp.h>
#include <H5public.h>
#include <cstdint>
#include <hdf5_hl.h>
// NOLINTEND

#include <memory>
#include <string>

#include <filesystem>
#include <format>

namespace poutre::io::details {
/**
 * @addtogroup poutre_io_group
 *@{
 */

namespace fs = std::filesystem;

    const H5std_string DATASET_NAME("Images");
    const H5std_string CHANNEL1("channel_1");
    const H5std_string CHANNEL2("channel_2");
    const H5std_string CHANNEL3("channel_3");
    const H5std_string CHANNEL4("channel_4");

    // compound_types
    const H5std_string IMAGEScalar("Scalar");
    const H5std_string IMAGE3Planes("3Planes");
    const H5std_string IMAGE4Planes("4Planes");

    // ptype
    const H5std_string PUINT8("pUINT8");
    const H5std_string PINT32("pINT32");
    const H5std_string PINT64("pINT64");
    const H5std_string PFLOAT("pF32");
    const H5std_string PDOUBLE("pD64");

    void CreateAttribute(H5::DataSet &dataSet, const std::string &key, const std::string &value) // NOLINT(misc-unused-parameters)
    {
      H5::StrType   const strType(H5::PredType::C_S1, value.size());
      H5::Attribute attr {dataSet.createAttribute(key, strType, H5::DataSpace())};
      attr.write(strType, value.c_str());
      attr.close();
    }

    void CreateAttribute(H5::DataSet &       dataSet,
                         const std::string & key,
                         int                 value,
                         const H5::PredType &type) // NOLINT(misc-unused-parameters)
    {
      H5::Attribute attr {dataSet.createAttribute(key, type, H5::DataSpace())};
      attr.write(type, &value);
      attr.close();
    }

    template<typename native_type> struct nativeTypeToAttrStr
    {
    };
    template<> struct nativeTypeToAttrStr<uint8_t>
    {
      H5std_string str {PUINT8};
    };

    template<> struct nativeTypeToAttrStr<int32_t>
    {
      H5std_string str {PINT32};
    };
    template<> struct nativeTypeToAttrStr<int64_t>
    {
      H5std_string str {PINT64};
    };
    template<> struct nativeTypeToAttrStr<float>
    {
      H5std_string str {PFLOAT};
    };
    template<> struct nativeTypeToAttrStr<double>
    {
      H5std_string str {PDOUBLE};
    };

    template<typename native_type> struct nativeTypeToH5DataType
    {
    };
    template<> struct nativeTypeToH5DataType<uint8_t>
    {
      H5::IntType type {H5::PredType::NATIVE_UINT8};
    };
    template<> struct nativeTypeToH5DataType<int32_t>
    {
      H5::IntType type {H5::PredType::NATIVE_INT32};
    };
    template<> struct nativeTypeToH5DataType<int64_t>
    {
      H5::IntType type {H5::PredType::NATIVE_INT64};
    };
    template<> struct nativeTypeToH5DataType<float>
    {
      H5::FloatType type {H5::PredType::NATIVE_FLOAT};
    };
    template<> struct nativeTypeToH5DataType<double>
    {
      H5::FloatType type {H5::PredType::NATIVE_DOUBLE};
    };

    std::vector<hsize_t> ImageCoordToHDF5Dim(const std::vector<std::size_t> &coords) // NOLINT(misc-unused-parameters)
    {
      auto res = std::vector<hsize_t>(coords.size());
      std::ranges::copy(coords, res.begin());
      return res;
    }

    template<typename T, ptrdiff_t rank>
    void StoreWithHDF5_helper(const IInterface &iimage,
                              const std::string &file_name, // NOLINT
                              const std::string &data_set_name)
    {
      const auto *im_t = dynamic_cast<const poutre::details::image_t<T, rank> *>(&iimage);
      if( !im_t )
      {
        POUTRE_RUNTIME_ERROR("StoreWithHDF5_helper Dynamic cast fail");
      }
      nativeTypeToH5DataType<T> hdf5_type;
      nativeTypeToAttrStr<T>    str_type;
      auto                      file       = H5::H5File(file_name,
                             H5F_ACC_TRUNC); // H5F_ACC_TRUNC : Overwrite existing files,H5F_ACC_EXCL fail if exist
      const auto &              dimensions = ImageCoordToHDF5Dim((*im_t).GetShape());

      try
      {
        // auto innerDataType = datatype.getSuper();
        // Little endian for x86
        hdf5_type.type.setOrder(H5T_ORDER_LE);

        H5::DataSpace dataspace(static_cast<int>(dimensions.size()), dimensions.data());

        H5::DataSet dataset = file.createDataSet(data_set_name, hdf5_type.type, dataspace);

        CreateAttribute(dataset, "CLASS", "IMAGE");
        CreateAttribute(dataset, "IMAGE_COMP_TYPE", IMAGEScalar);
        CreateAttribute(dataset, "IMAGE_P_TYPE", str_type.str);

        dataset.write(im_t->data(), hdf5_type.type);
      }
      catch( const H5::Exception &e )
      {
        POUTRE_RUNTIME_ERROR(std::format("StoreWithHDF5_helper: HDF5 fail : {}", e.getDetailMsg()));
      }
    }

    template<typename T, ptrdiff_t rank>
    void
    StoreWithHDF53Planes_helper(const IInterface &iimage,
                                const std::string &file_name, // NOLINT
                                const std::string &data_set_name)
    {
      const auto *im_t = dynamic_cast<const poutre::details::image_t<compound_type<T, 3>, rank> *>(&iimage);
      if( !im_t )
      {
        POUTRE_RUNTIME_ERROR("StoreWithHDF53Planes_helper Dynamic cast fail");
      }
      nativeTypeToH5DataType<T> native_hdf5_type;
      nativeTypeToAttrStr<T>    str_type;
      auto              hdf5_type = H5::CompType(sizeof(compound_type<T, 3>));
      hdf5_type.insertMember(CHANNEL1, 0, native_hdf5_type.type);
      hdf5_type.insertMember(CHANNEL2, sizeof(T), native_hdf5_type.type);
      hdf5_type.insertMember(CHANNEL3, 2 * sizeof(T), native_hdf5_type.type);

      auto        file       = H5::H5File(file_name,
                             H5F_ACC_TRUNC); // H5F_ACC_TRUNC : Overwrite existing files,H5F_ACC_EXCL fail if exist
      const auto &dimensions = ImageCoordToHDF5Dim((*im_t).GetShape());

      try
      {
        H5::DataSpace dataspace(static_cast<int>(dimensions.size()), dimensions.data());

        H5::DataSet dataset = file.createDataSet(data_set_name, hdf5_type, dataspace);

        CreateAttribute(dataset, "CLASS", "IMAGE");
        CreateAttribute(dataset, "IMAGE_COMP_TYPE", IMAGE3Planes);
        CreateAttribute(dataset, "IMAGE_P_TYPE", str_type.str);

        dataset.write(im_t->data(), hdf5_type);
      }
      catch( const H5::Exception &e )
      {
        POUTRE_RUNTIME_ERROR((std::format("StoreWithHDF53Planes_helper: HDF5 fail : {}", e.getDetailMsg())));
      }
    }

    template<typename T, ptrdiff_t rank>
    void
    StoreWithHDF54Planes_helper(const IInterface &iimage,
                                const std::string &file_name, //NOLINT
                                const std::string &data_set_name)
    {
      const auto *im_t = dynamic_cast<const poutre::details::image_t<compound_type<T, 4>, rank> *>(&iimage);
      if( !im_t )
      {
        POUTRE_RUNTIME_ERROR("StoreWithHDF54Planes_helper Dynamic cast fail");
      }
      nativeTypeToH5DataType<T> native_hdf5_type;
      nativeTypeToAttrStr<T>    str_type;
      auto              hdf5_type = H5::CompType(sizeof(compound_type<T, 4>));
      hdf5_type.insertMember(CHANNEL1, 0, native_hdf5_type.type);
      hdf5_type.insertMember(CHANNEL2, sizeof(T), native_hdf5_type.type);
      hdf5_type.insertMember(CHANNEL3, 2 * sizeof(T), native_hdf5_type.type);
      hdf5_type.insertMember(CHANNEL4, 3 * sizeof(T), native_hdf5_type.type);

      auto file = H5::H5File(file_name,
                             H5F_ACC_TRUNC); // H5F_ACC_TRUNC : Overwrite existing files,H5F_ACC_EXCL fail if exist

      const auto &dimensions = ImageCoordToHDF5Dim((*im_t).GetShape());
      try
      {
        H5::DataSpace dataspace(static_cast<int>(dimensions.size()), dimensions.data());

        H5::DataSet dataset = file.createDataSet(data_set_name, hdf5_type, dataspace);

        CreateAttribute(dataset, "CLASS", "IMAGE");
        CreateAttribute(dataset, "IMAGE_COMP_TYPE", IMAGE4Planes);
        CreateAttribute(dataset, "IMAGE_P_TYPE", str_type.str);

        dataset.write(im_t->data(), hdf5_type);
      }
      catch( const H5::Exception &e )
      {
        POUTRE_RUNTIME_ERROR((std::format("StoreWithHDF54Planes_helper: HDF5 fail : {}", e.getDetailMsg())));
      }
    }

    template<typename T, ptrdiff_t rank> void LoadFromHDF5_helper(IInterface &iimage, const H5::DataSet &data_set)
    {
      auto *im_t = dynamic_cast<poutre::details::image_t<T, rank> *>(&iimage);
      if( !im_t )
      {
        POUTRE_RUNTIME_ERROR("LoadFromHDF5_helper Dynamic cast fail");
      }
      nativeTypeToH5DataType<T> hdf5_type;

      // const auto &dimensions = ImageCoordToHDF5Dim((*im_t).GetShape());

      try
      {
        data_set.read(im_t->data(), hdf5_type.type);
      }
      catch( const H5::Exception &e )
      {
        POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF5_helper: HDF5 fail : {}", e.getDetailMsg())));
      }
    }

    template<typename T, ptrdiff_t rank> void LoadFromHDF53Planes_helper(IInterface &iimage, const H5::DataSet &data_set)
    {
      auto *im_t = dynamic_cast<poutre::details::image_t<compound_type<T, 3>, rank> *>(&iimage);
      if( !im_t )
      {
        POUTRE_RUNTIME_ERROR("LoadFromHDF53Planes_helper Dynamic cast fail");
      }
      nativeTypeToH5DataType<T> native_hdf5_type;
      auto              hdf5_type = H5::CompType(sizeof(compound_type<T, 3>));
      hdf5_type.insertMember(CHANNEL1, 0, native_hdf5_type.type);
      hdf5_type.insertMember(CHANNEL2, sizeof(T), native_hdf5_type.type);
      hdf5_type.insertMember(CHANNEL3, 2 * sizeof(T), native_hdf5_type.type);

      try
      {
        data_set.read(im_t->data(), hdf5_type);
      }
      catch( const H5::Exception &e )
      {
        POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF53Planes_helper: HDF5 fail : {}", e.getDetailMsg())));
      }
    }

    template<typename T, ptrdiff_t rank> void LoadFromHDF54Planes_helper(IInterface &iimage, const H5::DataSet &data_set)
    {
      auto *im_t = dynamic_cast<poutre::details::image_t<compound_type<T, 4>, rank> *>(&iimage);
      if( !im_t )
      {
        POUTRE_RUNTIME_ERROR("LoadFromHDF53Planes_helper Dynamic cast fail");
      }
      nativeTypeToH5DataType<T> native_hdf5_type;
      auto              hdf5_type = H5::CompType(sizeof(compound_type<T, 4>));
      hdf5_type.insertMember(CHANNEL1, 0, native_hdf5_type.type);
      hdf5_type.insertMember(CHANNEL2, sizeof(T), native_hdf5_type.type);
      hdf5_type.insertMember(CHANNEL3, 2 * sizeof(T), native_hdf5_type.type);
      hdf5_type.insertMember(CHANNEL4, 3 * sizeof(T), native_hdf5_type.type);

      try
      {
        data_set.read(im_t->data(), hdf5_type);
      }
      catch( const H5::Exception &e )
      {
        POUTRE_RUNTIME_ERROR((std::format("LoadFromHDF53Planes_helper: HDF5 fail : {}", e.getDetailMsg())));
      }
    }

//! @} doxygroup: poutre_io_group
}//poutre::io::details