//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   oiio.hpp
 * @author Thomas Retornaz
 * @brief  openimageio load/save integrations
 *
 *
 */


#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/trace.hpp>

#include <filesystem>
#include <format>
#include <memory>

#ifdef __CLANG__
    #pragma clang diagnostic push
    // silencing warnings from OpenEXR 2.2.0 / OpenImageIO
    #pragma clang diagnostic ignored "-Wdeprecated-register"
    #pragma clang load("OpenImageIO")
#endif

#include <OpenImageIO/imageio.h>

#ifdef __CLANG__
    #pragma clang diagnostic pop
#endif

namespace poutre::io::details {
/**
 * @addtogroup poutre_io_group
 *@{
 */

namespace fs = std::filesystem;

 /**
 * @brief  Helper function which copy content of input OIIO object in 2D Image assuming
 * scalar type
 *
 * @tparam T Image pType
 * @param input preconstructed OIIO object
 * @param img Image where to copy data
 * @warning Unsafe method use @c ImageLoader for safety check
 */
template<typename T> void FillImageFromOIIOScalar(OIIO::ImageInput &input, poutre::details::image_t<T, 2> &img)
{
  input.read_image(0,0,0,1,OIIO::BaseTypeFromC<T>::value, img.data());
}


/**
 * @brief Helper function which copy content of input file in Image 2D assuming scalar type
 *
 * @tparam T  T Image pType
 * @param path path of the image to load
 * @param img Image where to copy data
 * @warning Unsafe method use @c ImageLoader for safety check
 */
template<typename T> void FillImageFromOIIOScalar(const std::string &path, poutre::details::image_t<T, 2> &img)
{
  fs::path const local_path(path);
  if( !(fs::exists(local_path)) )
  {
    POUTRE_RUNTIME_ERROR(
        (std::format("FillImageFromOIIOScalar: provided path {} doesn't exists", local_path.string())));
  }
  auto in_oiio(OIIO::ImageInput::open(path));
  if( !in_oiio )
  {
    std::ostringstream error_stream;
    error_stream << " FillImageFromOIIOScalar load_image(): Error reading image '";
    error_stream << path;
    error_stream << "\n" << OIIO::geterror();
    POUTRE_RUNTIME_ERROR(error_stream.str());
  }
  const OIIO::ImageSpec &spec = in_oiio->spec();
  if( spec.nchannels != 1 )
  {
  	in_oiio->close();
    POUTRE_RUNTIME_ERROR(
        (std::format("FillImageFromOIIOScalar: wrong number of channels expected 1 found {}", spec.nchannels)));
  }
  if( spec.depth > 1 )
  {
	in_oiio->close();
    POUTRE_RUNTIME_ERROR(std::format("FillImageFromOIIOScalar: wrong number of dim expected 2 found depth dim"));
  }
  FillImageFromOIIOScalar(*in_oiio, img);

  in_oiio->close();
}

 /**
     * @brief  Helper function which copy content of input OIIO object in Image 2D assuming RGB
     * like color space
     *
     * @tparam T Image pType
     * @param input preconstructed OIIO object
     * @param img Image where to copy data
     * @warning Unsafe method use @c ImageLoader for safety check
     */
    template<typename T> void FillImageFromOIIOCompound3(OIIO::ImageInput &input, poutre::details::image_t<compound_type<T, 3>, 2> &img)
    {
      const OIIO::ImageSpec &spec = input.spec();
      if( spec.nchannels != 3 )
      {
        POUTRE_RUNTIME_ERROR(
            (std::format("FillImageFromOIIOCompound3: wrong number of channels expected 3 found {}", spec.nchannels)));
      }

      std::vector<T> tmp;
      tmp.resize(static_cast<size_t>(spec.width) * static_cast<size_t>(spec.height) * static_cast<size_t>(spec.nchannels));
      input.read_image(0,0,0,-1,OIIO::BaseTypeFromC<T>::value, tmp.data());
      const auto ptr_tmp = tmp.data();
      auto       ptr_img = img.data();
      for( size_t i = 0; i < tmp.size(); i += 3 )
      {
        (*ptr_img)[0] = ptr_tmp[i];
        (*ptr_img)[1] = ptr_tmp[i + 1];
        (*ptr_img)[2] = ptr_tmp[i + 2];
        ++ptr_img;
      }
    }
    /**
     * @brief Helper function which copy content of input file in Image 2D assuming like RGB
     * color space
     *
     * @tparam T  T Image pType
     * @param path path of the image to load
     * @param img where to copy data
     * @warning Unsafe method use @c ImageLoader for safety check
     */
    template<typename T> void FillImageFromOIIOCompound3(const std::string &path, poutre::details::image_t<compound_type<T, 3>, 2> &img)
    {
      fs::path const localPath(path);
      if( !(fs::exists(localPath)) )
      {
        POUTRE_RUNTIME_ERROR(
            (std::format("FillImageFromOIIOCompound3: provided path {} doesn't exists", localPath.string())));
      }
      auto in_oiio(OIIO::ImageInput::open(path));
      if( !in_oiio )
      {
        std::ostringstream error_stream;
        error_stream << " FillImageFromOIIOCompound3 load_image(): Error reading image '";
        error_stream << path;
        error_stream << "\n" << OIIO::geterror();
        POUTRE_RUNTIME_ERROR(error_stream.str());
      }
      const OIIO::ImageSpec &spec = in_oiio->spec();
      if( spec.nchannels != 3 )
      {
        in_oiio->close();
        POUTRE_RUNTIME_ERROR(
            (std::format("FillImageFromOIIOCompound3: wrong number of channels expected 3 found {}", spec.nchannels)));
      }
      if( spec.depth > 1 )
      {
        in_oiio->close();
        POUTRE_RUNTIME_ERROR(std::format("FillImageFromOIIOCompound3: wrong number of dim expected 2 found depth dim"));
      }
      FillImageFromOIIOCompound3(*in_oiio, img);

      in_oiio->close();
    }
    /**
     * @brief  Helper function which copy content of input OIIO object in Image 2D assuming
     * RGBA like color space
     *
     * @tparam T Image pType
     * @param input preconstructed OIIO object
     * @param img Image where to copy data
     * @warning Unsafe method use @c ImageLoader for safety check
     */
    template<typename T> void FillImageFromOIIOCompound4(OIIO::ImageInput &input, poutre::details::image_t<compound_type<T, 4>, 2> &img)
    {
      const OIIO::ImageSpec &spec = input.spec();
      if( spec.nchannels != 4 ) //-V112
      {
        POUTRE_RUNTIME_ERROR(
            (std::format("FillImageFromOIIOCompound4: wrong number of channels expected 4 found {}", spec.nchannels)));
      }

      std::vector<T> tmp;
      tmp.resize(
        static_cast<size_t>(spec.width) * static_cast<size_t>(spec.height) * static_cast<size_t>(spec.nchannels));
      input.read_image(0,0,0,-1,OIIO::BaseTypeFromC<T>::value, tmp.data());
      const auto ptr_tmp = tmp.data();
      auto       ptr_img = img.data();
      for( size_t i = 0; i < tmp.size(); i += 4 ) //-V112
      {
        (*ptr_img)[0] = ptr_tmp[i];
        (*ptr_img)[1] = ptr_tmp[i + 1];
        (*ptr_img)[2] = ptr_tmp[i + 2];
        (*ptr_img)[3] = ptr_tmp[i + 3];
        ++ptr_img;
      }
    }

    /**
     * @brief Helper function which copy content of input file in Image 2D assuming like RGBA
     * color space
     *
     * @tparam T  T Image pType
     * @param path path of the image to load
     * @param img where to copy data
     * @warning Unsafe method use @c ImageLoader for safety check
     */
    template<typename T> void FillImageFromOIIOCompound4(const std::string &path, poutre::details::image_t<compound_type<T, 4>, 2> &img)
    {
      fs::path const localPath(path);
      if( !(fs::exists(localPath)) )
      {
        POUTRE_RUNTIME_ERROR(
            (std::format("FillImageFromOIIOCompound4: provided path {} doesn't exists", localPath.string())));
      }
      auto in_oiio(OIIO::ImageInput::open(path));
      if( !in_oiio )
      {
        std::ostringstream error_stream;
        error_stream << " FillImageFromOIIOCompound4 load_image(): Error reading image '";
        error_stream << path;
        error_stream << "\n" << OIIO::geterror();
        POUTRE_RUNTIME_ERROR(error_stream.str());
      }
      const OIIO::ImageSpec &spec = in_oiio->spec();
      if( spec.nchannels != 4 ) //-V112
      {
        in_oiio->close();
        POUTRE_RUNTIME_ERROR(
            (std::format("FillImageFromOIIOCompound4: wrong number of channels expected 4 found {}", spec.nchannels)));
      }
      if( spec.depth > 1 )
      {
        in_oiio->close();
        POUTRE_RUNTIME_ERROR(std::format("FillImageFromOIIOCompound4: wrong number of dim expected 2 found depth dim"));
      }
      FillImageFromOIIOCompound4(*in_oiio, img);

      in_oiio->close();
    }

    /**
     * @brief Way to pass options to @c StoreWithOIIO
     *
     */
    struct StoreWithOIIOOptions
    {
      /**
       * @brief Construct a new options with default compression quality to 100
       *
       */
      StoreWithOIIOOptions() : spec(0, 0, 0) { spec.attribute("CompressionQuality", 100); }

      /**
       * @brief Forward an attribute to an OpenImageIO ImageSpec.
       *   See the documentation of OIIO::ImageSpec::attribute() for a list
       *   of supported attributes.
       *   Default: "CompressionQuality" = 100
       *
       * @tparam T
       * @param name
       * @param attribute
       * @return StoreWithOIIOOptions&
       */
      template<class T> StoreWithOIIOOptions &attribute(OIIO::string_view name, T const &attribute)
      {
        spec.attribute(name, attribute);
        return *this;
      }

      OIIO::ImageSpec spec;
      bool            autoconvert{true};
    };

    /**
     * @brief  Helper function which copy content of Image into OIIO object assuming scalar
     * type
     *
     * @tparam T Image pType
     * @param img Image to copy into OIIO object
     * @param out preconstructed OIIO object
     * @warning Unsafe method use @c ImageWriter for safety check
     */
    template<typename T> void StoreWithOIIOScalar(const poutre::details::image_t<T, 2> &img, OIIO::ImageOutput &out)
    {
      const OIIO::ImageSpec &spec = out.spec();
      POUTRE_CHECK(spec.nchannels == 1, "Wrong number of channels");

      // stolen from
      // https://github.com/xtensor-stack/xtensor-io/blob/master/include/xtensor-io/ximage.hpp
      if( out.spec().format != OIIO::BaseTypeFromC<T>::value )
      {
        // OpenImageIO changed the target type because the file format doesn't support value_type.
        // It will do automatic conversion, but the data should be in the range 0...1
        // for good results.

        //! TODO! make measurement module
        //! TODO! vectorize
        const auto    iters = std::minmax_element(img.cbegin(), img.cend());
        const pDOUBLE min   = (iters.first != img.cend() ? static_cast<pDOUBLE>(*iters.first) : TypeTraits<pDOUBLE>::sup());
        const pDOUBLE max   = (iters.second != img.cend() ? static_cast<pDOUBLE>(*iters.second) : TypeTraits<pDOUBLE>::inf());

        if( max != min )
        {
          std::vector<pDOUBLE> normalized(img.size());
          for( size_t i = 0; i < img.size(); ++i ) {normalized[i] = (1.0 / (max - min)) * (static_cast<pDOUBLE>(img[i]) - min);}
          out.write_image(OIIO::BaseTypeFromC<pDOUBLE>::value, normalized.data());
        }
        else
        {
          out.write_image(OIIO::BaseTypeFromC<T>::value, img.data());
        }
      }
      else
      {
        out.write_image(OIIO::BaseTypeFromC<T>::value, img.data());
      }
      out.close();
    }

    /**
     * @brief  Helper function which copy content of Image on disk
     *
     * @tparam T Image pType
     * @param img Image to serialize
     * @param path path where image is serialized
     * @param options OIIO options
     * @warning Unsafe method use @c ImageWriter for safety check
     */
    template<typename T>
    void StoreWithOIIOScalar(const poutre::details::image_t<T, 2> &img, const std::string &path, StoreWithOIIOOptions const &options)
    {
      POUTRE_ENTERING("StoreWithOIIOScalar");
      fs::path const localPath(path);
      fs::path const dir = localPath.parent_path();
      if( !(fs::exists(dir)) )
      {
        POUTRE_RUNTIME_ERROR(std::format("StoreWithOIIOScalar: provided path {} doesn't exists", dir.string()));
      }
      const auto shape = img.shape();

      auto out(OIIO::ImageOutput::create(path));
      if( !out )
      {
        std::ostringstream error_stream;
        error_stream << " StoreWithOIIOScalar(): Error opening file '";
        error_stream << path;
        error_stream << "\nGet last error:\n";
        error_stream << OIIO::geterror();
        POUTRE_RUNTIME_ERROR(error_stream.str());
      }
      OIIO::ImageSpec spec = options.spec;

      spec.width     = static_cast<int>(shape[1]);
      spec.height    = static_cast<int>(shape[0]);
      spec.nchannels = 1;
      spec.format    = OIIO::BaseTypeFromC<T>::value;

      out->open(path, spec);

      StoreWithOIIOScalar(img, *out);
    }

    /**
     * @brief  Helper function which copy content of Image into OIIO object assuming RGB like
     * color space
     *
     * @tparam T Image pType
     * @param img Image to copy into OIIO object
     * @param out preconstructed OIIO object
     * @warning Unsafe method use @c ImageWriter for safety check
     */
    template<typename T> void StoreWithOIIOCompound3(const poutre::details::image_t<compound_type<T, 3>, 2> &img, OIIO::ImageOutput &out)
    {
      const OIIO::ImageSpec &spec = out.spec();
      POUTRE_CHECK(spec.nchannels == 3, "StoreWithOIIOCompound3(): Nb of channels must be 3");
      std::vector<T> buffer(static_cast<std::size_t>(spec.width * spec.height * spec.nchannels));
      const auto     ptr_buffer = buffer.data();
      auto           ptr_img    = img.data();

      auto min_0 = TypeTraits<pDOUBLE>::sup();
      auto min_1 = TypeTraits<pDOUBLE>::sup();
      auto min_2     = TypeTraits<pDOUBLE>::sup();
      auto max_0 = TypeTraits<pDOUBLE>::inf();
      auto max_1 = TypeTraits<pDOUBLE>::inf();
      auto max_2 = TypeTraits<pDOUBLE>::inf();

      for( size_t i = 0; i < buffer.size(); i += 3 )
      {
        ptr_buffer[i] = (*ptr_img)[0];
        min_0         = std::min(min_0, static_cast<pDOUBLE>(ptr_buffer[i]));
        max_0         = std::max(max_0, static_cast<pDOUBLE>(ptr_buffer[i]));

        ptr_buffer[i + 1] = (*ptr_img)[1];
        min_1             = std::min(min_1, static_cast<pDOUBLE>(ptr_buffer[i + 1]));
        max_1             = std::max(max_1, static_cast<pDOUBLE>(ptr_buffer[i + 1]));

        ptr_buffer[i + 2] = (*ptr_img)[2];
        min_2             = std::min(min_2, static_cast<pDOUBLE>(ptr_buffer[i + 2]));
        max_2             = std::max(max_2, static_cast<pDOUBLE>(ptr_buffer[i + 2]));

        ++ptr_img;
      }

      // stolen from
      // https://github.com/xtensor-stack/xtensor-io/blob/master/include/xtensor-io/ximage.hpp
      if( out.spec().format != OIIO::BaseTypeFromC<T>::value )
      {
        // OpenImageIO changed the target type because the file format doesn't support value_type.
        // It will do automatic conversion, but the data should be in the range 0...1
        // for good results.

        //! TODO! make measurement module
        //! TODO! vectorize

        if( max_0 != min_0 && max_1 != min_1 && max_2 != min_2 ) //-V550
        {                                                        //-V550
          std::vector<pDOUBLE> normalized(buffer.size());
          for( size_t i = 0; i < buffer.size(); i += 3 )
          {
            normalized[i]     = (1.0 / (max_0 - min_0)) * (static_cast<pDOUBLE>(buffer[i]) - min_0);
            normalized[i + 1] = (1.0) / (max_1 - min_1) * (static_cast<pDOUBLE>(buffer[i + 1]) - min_1);
            normalized[i + 2] = (1.0) / (max_2 - min_2) * (static_cast<pDOUBLE>(buffer[i + 2]) - min_2);
          }
          out.write_image(OIIO::BaseTypeFromC<pDOUBLE>::value, normalized.data());
        }
        else
        {
          out.write_image(OIIO::BaseTypeFromC<T>::value, buffer.data());
        }
      }
      else
      {
        out.write_image(OIIO::BaseTypeFromC<T>::value, buffer.data());
      }
      out.close();
    }

    /**
     * @brief  Helper function which copy content of Image on disk assuming RGB like color
     * space
     *
     * @tparam T Image pType
     * @param img Image to serialize
     * @param path path where image is serialized
     * @param options OIIO options
     * @warning Unsafe method use @c ImageWriter for safety check
     */
    template<typename T>
    void StoreWithOIIOCompound3(const poutre::details::image_t<compound_type<T, 3>, 2> &img,
                                const std::string &                  path,
                                StoreWithOIIOOptions const &         options)
    {
      POUTRE_ENTERING("StoreWithOIIOCompound3");
      fs::path const localPath(path);
      fs::path const dir = localPath.parent_path();
      if( !(fs::exists(dir)) )
      {
        POUTRE_RUNTIME_ERROR(std::format("StoreWithOIIOCompound3: provided path {} doesn't exists", dir.string()));
      }
      const auto shape = img.shape();

      auto out(OIIO::ImageOutput::create(path));
      if( !out )
      {
        std::ostringstream error_stream;
        error_stream << " StoreWithOIIOCompound3(): Error opening file '";
        error_stream << path;
        error_stream << "\nGet last error:\n";
        error_stream << OIIO::geterror();
        POUTRE_RUNTIME_ERROR(error_stream.str());
      }
      OIIO::ImageSpec spec = options.spec;

      spec.width     = static_cast<int>(shape[1]);
      spec.height    = static_cast<int>(shape[0]);
      spec.nchannels = 3;
      spec.format    = OIIO::BaseTypeFromC<T>::value;

      out->open(path, spec);

      StoreWithOIIOCompound3(img, *out);
    }

    /**
     * @brief  Helper function which copy content of Image into OIIO object assuming RGBA like
     * color space
     *
     * @tparam T Image pType
     * @param img Image to copy into OIIO object
     * @param out preconstructed OIIO object
     * @warning Unsafe method use @c ImageWriter for safety check
     */
    template<typename T> void StoreWithOIIOCompound4(const poutre::details::image_t<compound_type<T, 4>, 2> &img, OIIO::ImageOutput &out)
    {
      const OIIO::ImageSpec &spec = out.spec();
      POUTRE_CHECK(spec.nchannels == 4, //-V112
                   "StoreWithOIIOCompound4(): Nb of channels must be 4");
      std::vector<T> buffer(static_cast<std::size_t>(spec.width * spec.height * spec.nchannels));
      const auto     ptr_buffer = buffer.data();
      auto           ptr_img    = img.data();

      auto min_0 = TypeTraits<pDOUBLE>::sup();
      auto min_1 = TypeTraits<pDOUBLE>::sup();
      auto min_2 = TypeTraits<pDOUBLE>::sup();
      auto min_3 = TypeTraits<pDOUBLE>::sup();
      auto max_0 = TypeTraits<pDOUBLE>::inf();
      auto max_1 = TypeTraits<pDOUBLE>::inf();
      auto max_2 = TypeTraits<pDOUBLE>::inf();
      auto max_3 = TypeTraits<pDOUBLE>::inf();

      for( size_t i = 0; i < buffer.size(); i += 4 ) //-V112
      {
        ptr_buffer[i] = (*ptr_img)[0];
        min_0         = std::min(min_0, static_cast<pDOUBLE>(ptr_buffer[i]));
        max_0         = std::max(max_0, static_cast<pDOUBLE>(ptr_buffer[i]));

        ptr_buffer[i + 1] = (*ptr_img)[1];
        min_1             = std::min(min_1, static_cast<pDOUBLE>(ptr_buffer[i + 1]));
        max_1             = std::max(max_1, static_cast<pDOUBLE>(ptr_buffer[i + 1]));

        ptr_buffer[i + 2] = (*ptr_img)[2];
        min_2             = std::min(min_2, static_cast<pDOUBLE>(ptr_buffer[i + 2]));
        max_2             = std::max(max_2, static_cast<pDOUBLE>(ptr_buffer[i + 2]));

        ptr_buffer[i + 3] = (*ptr_img)[3];
        min_3             = std::min(min_3, static_cast<pDOUBLE>(ptr_buffer[i + 2]));
        max_3             = std::max(max_3, static_cast<pDOUBLE>(ptr_buffer[i + 2]));

        ++ptr_img;
      }

      // stolen from
      // https://github.com/xtensor-stack/xtensor-io/blob/master/include/xtensor-io/ximage.hpp
      if( out.spec().format != OIIO::BaseTypeFromC<T>::value )
      {
        // OpenImageIO changed the target type because the file format doesn't support value_type.
        // It will do automatic conversion, but the data should be in the range 0...1
        // for good results.

        //! TODO! make measurement module
        //! TODO! vectorize

        if( max_0 != min_0 && max_1 != min_1 && max_2 != min_2 && max_3 != min_3 ) //-V550
        {                                                                          //-V550
          std::vector<pDOUBLE> normalized(buffer.size());
          for( size_t i = 0; i < buffer.size(); i += 3 )
          {
            normalized[i]     = (1.0 / (max_0 - min_0)) * (static_cast<pDOUBLE>(buffer[i]) - min_0);
            normalized[i + 1] = (1.0 / (max_1 - min_1)) * (static_cast<pDOUBLE>(buffer[i + 1]) - min_1);
            normalized[i + 2] = (1.0 / (max_2 - min_2)) * (static_cast<pDOUBLE>(buffer[i + 2]) - min_2);
            normalized[i + 3] = (1.0 / (max_3 - min_3)) * (static_cast<pDOUBLE>(buffer[i + 3]) - min_3);
          }
          out.write_image(OIIO::BaseTypeFromC<pDOUBLE>::value, normalized.data());
        }
        else
        {
          out.write_image(OIIO::BaseTypeFromC<T>::value, buffer.data());
        }
      }
      else
      {
        out.write_image(OIIO::BaseTypeFromC<T>::value, buffer.data());
      }
      out.close();
    }

    /**
     * @brief  Helper function which copy content of Image on disk assuming RGBA like color
     * space
     *
     * @tparam T Image pType
     * @param img Image to serialize
     * @param path path where image is serialized
     * @param options OIIO options
     * @warning Unsafe method use @c ImageWriter for safety check
     */
    template<typename T>
    void StoreWithOIIOCompound4(const poutre::details::image_t<compound_type<T, 4>, 2> &img,
                                const std::string &                  path,
                                StoreWithOIIOOptions const &         options)
    {
      POUTRE_ENTERING("StoreWithOIIOCompound4");
      fs::path const localPath(path);
      fs::path const dir = localPath.parent_path();
      if( !(fs::exists(dir)) )
      {
        std::ostringstream error_stream;
        error_stream << " StoreWithOIIOCompound4(): Error opening file '";
        error_stream << path;
        POUTRE_RUNTIME_ERROR(error_stream.str());
      }
      const auto shape = img.shape();

      auto out(OIIO::ImageOutput::create(path));
      if( !out )
      {
        std::ostringstream error_stream;
        error_stream << " StoreWithOIIOCompound3(): Error opening file '";
        error_stream << path;
        error_stream << "\nGet last error:\n";
        error_stream << OIIO::geterror();
        POUTRE_RUNTIME_ERROR(error_stream.str());
      }
      OIIO::ImageSpec spec = options.spec;

      spec.width     = static_cast<int>(shape[1]);
      spec.height    = static_cast<int>(shape[0]);
      spec.nchannels = 4; //-V112
      spec.format    = OIIO::BaseTypeFromC<T>::value;

      out->open(path, spec);

      StoreWithOIIOCompound4(img, *out);
    }
//! @} doxygroup: poutre_io_group
}//poutre::io::details