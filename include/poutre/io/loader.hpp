//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   loader.hpp
 * @author Thomas Retornaz
 * @brief  Loader images
 *
 *
 */

#include <poutre/base/image_interface.hpp>
#include <poutre/io/io.hpp>

#include <memory>
#include <string>

#if defined(POUTRE_IS_MSVC)
  #pragma warning(push)
  #pragma warning(disable : 4251)// needs to have dll-interface to be used by clients of class
#endif
namespace poutre::io {
/**
 * @addtogroup poutre_io_group
 *@{
 */

class ImageProvider;

/**
   * @brief Concrete ImageLoader
   * Powered by OpenImageIO toolset see https://sites.google.com/site/openimageio/home
   * so various formats are supported (and deduced from path)
   * at least the most common ones (PNG,JPEG,TIFF,BMP,...)
   * @note for Image with more than 2 dimensions, only hdf5 format is supported
   * @warning in HDF5 case, only one image is loaded correspond to name "poutre_img_1"
   * for advanced options
   */
class IO_API ImageLoader
{
  private:

  friend class ImageProvider;
  std::string m_imgPath;
  std::string m_i_name;
  bool        m_isready;

public:
  /**
   * @brief Construct a new Image Loader object
   *
   */
  ImageLoader();

  /**
   * @brief Set the Path to the image to load
   *
   * @param i_imgpath  path to the image to load
   * @return self_type& return reference to object to chain call
   */
  ImageLoader &SetPath(const std::string &i_imgpath);

  /**
 * @brief Set the name (id) to the image to load. Only make sense for container like hdf5,npz
 *
 * @param i_name  name to the image to load
 * @return self_type& return reference to object to chain call
 */
  ImageLoader &SetName(const std::string &i_name);

  /**
   * @brief Load the image according to SetPath
   *
   * @return std::unique_ptr<IInterface>
   */
  std::unique_ptr<IInterface> Load() const;
};

//! @} doxygroup: poutre_io_group
}// namespace poutre::io
#if defined(POUTRE_IS_MSVC)
	#pragma warning(pop)
#endif