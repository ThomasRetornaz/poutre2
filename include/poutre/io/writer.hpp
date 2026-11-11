/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/


#ifndef INCLUDE_POUTRE_IO_WRITER
#define INCLUDE_POUTRE_IO_WRITER

#include <poutre/base/image_interface.hpp>
#include <poutre/io/io.hpp>

#include <string>

#ifdef POUTRE_IS_MSVC
#pragma warning(push)
#pragma warning(disable : 4251)// needs to have dll-interface to be used by clients of class
#endif

namespace poutre::io {
/**
 * @addtogroup image_processing_io_group Image Processing IO API
 * @ingroup image_processing_group
 *@{
 */

/**
 * @brief Concrete ImageWritter
 * so various formats are supported (and deduced from path)
 * at least the most common ones (PNG,JPEG,TIFF,BMP,hdf5,...)
 */
class IO_API ImageWriter
{
  std::string m_imgPath;
  std::string m_i_name;
  bool m_isready = false;

public:
  /**
   * @brief Construct a new Image Loader object
   *
   */
  ImageWriter();

  /**
   * @brief Set the Path object
   *
   * @param i_imgpath path where to store the image, format is deduced from extention
   * @return ImageWriter& chain
   * @note For image with more than 2 dimentions only format like hdf5 is supported
   */
  ImageWriter &SetPath(const std::string &i_imgpath);

  /**
   * @brief Set the name (id) to the image to load. Only make sense for container like hdf5,npz
   *
   * @param i_name  name to the image to load
   * @return ImageWriter& return reference to object to chain call
   */
  ImageWriter &SetName(const std::string &i_name);
  void Write(const IInterface &i_img) const;
};

//! @} doxygroup: image_processing_io_group
}// namespace poutre::io
#ifdef POUTRE_IS_MSVC
#pragma warning(pop)
#endif


#endif /* INCLUDE_POUTRE_IO_WRITER */
