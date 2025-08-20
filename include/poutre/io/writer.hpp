//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   writer.hpp
 * @author Thomas Retornaz
 * @brief  Write images
 *
 *
 */

#include <poutre/base/image_interface.hpp>
#include <poutre/io/io.hpp>

#include <memory>
#include <string>

namespace poutre::io
{
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
private:
  std::string m_imgPath;
  std::string m_i_name;
  bool        m_isready;

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
  void       Write(const IInterface &i_img) const;
};

//! @} doxygroup: image_processing_io_group
}