/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_IO_LOADER
#define INCLUDE_POUTRE_IO_LOADER

#include <poutre/base/image_interface.hpp>
#include <poutre/io/io.hpp>

#include <memory>
#include <string>

#ifdef POUTRE_IS_MSVC
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
  friend class ImageProvider;
  std::string m_imgPath;
  std::string m_i_name;
  bool m_isready = false;

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
  [[nodiscard]] std::unique_ptr<IInterface> Load() const;
};

//! @} doxygroup: poutre_io_group
}// namespace poutre::io
#ifdef POUTRE_IS_MSVC
#pragma warning(pop)
#endif


#endif /* INCLUDE_POUTRE_IO_LOADER */
