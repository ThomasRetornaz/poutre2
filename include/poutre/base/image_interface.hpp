//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   image_interface.hpp
 * @author Thomas Retornaz
 * @brief  Define main pure interface for "Image"
 *
 *
 */

#include <poutre/base/base.hpp>
#include <poutre/base/config.hpp>
#include <poutre/base/types.hpp>

#include <memory>
#include <vector>

namespace poutre {

/**
 * @addtogroup image_processing_interface_group Image Processing Interface API
 * @ingroup image_processing_group
 *@{
 */

/**
 * @brief Pure interface class for all "Image" object
 */

// TODO add colorspace ?
class BASE_API IInterface
{
public:
  //! Get @c PType belong to @c IInterface
  [[nodiscard]]virtual PType GetPType() const = 0;
  //! Get @c CompoundType belong to @c IInterface
  [[nodiscard]] virtual CompoundType GetCType() const = 0;
  //! Get size over dimensions
  [[nodiscard]] virtual std::vector<std::size_t> GetShape() const = 0;//@TODO
                                                       // may
                                                       // change this
  //! Get num of dimensions
  [[nodiscard]] virtual std::size_t GetRank() const = 0;
  //! Dtor
  virtual ~IInterface() = default;
  //! Stringification
  virtual std::string str() const = 0;
};

//! operator<< for IInterface
//@warning Serialize a description of image. Doesn't serialize data. Use @c
// ImageFromString,@c
// ImageToString
BASE_API std::ostream &operator<<(std::ostream &, const IInterface &img);

/**
 * @brief Check if images are compatible regarding sizes
 *
 * @param i_img1 [in] input image
 * @param i_img2 [in] input image
 *
 * @return true if sizes of provided images are compatible
 */
BASE_API bool AsSizesCompatible(const IInterface &i_img1, const IInterface &i_img2);

/**
 * @brief Check if images are compatible regarding type ( @c CompoundType, @c PType)
 *
 * @param i_img1 [in] input image
 * @param i_img2 [in] input image
 *
 * @return true if sizes of provided images are compatible
 */
BASE_API bool AsTypesCompatible(const IInterface &i_img1, const IInterface &i_img2);

/**
 * @brief Check if images are different regarding adress
 *
 * @param i_img1 [in] input image
 * @param i_img2 [in] input image
 *
 * @return true if ref not belong to the same image
 */
 BASE_API bool AreImagesDifferent(const IInterface &i_img1, const IInterface &i_img2);


//! @throw runtime_error if input images  are not compatible @see @c AsSizesCompatible
BASE_API void AssertSizesCompatible(const IInterface &i_img1, const IInterface &i_img2, const std::string &i_msg);

//! @throw runtime_error if input images  are not compatible @see @c AsTypesCompatible
BASE_API void AssertAsTypesCompatible(const IInterface &i_img1, const IInterface &i_img2, const std::string &i_msg);

//! @throw runtime_error if input images are the same object @see @c AreImagesDifferent
BASE_API void AssertImagesAreDifferent(const IInterface &i_img1, const IInterface &i_img2, const std::string &i_msg);


//! Factory to build contiguous dense image
BASE_API std::unique_ptr<IInterface> Create(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype);

/**
* @brief Convert IInterface to human readable string
*
* @param i_image
* @return std::string
*/
BASE_API std::string ImageToString(const IInterface &i_image);

/**
* @brief From human readable string create an IInterface see @c ImageToString
*
* @param i_str
* @return Image
*/
BASE_API std::unique_ptr<IInterface> ImageFromString(const std::string &i_str);

//! @} doxygroup: image_processing_interface_group

}// namespace poutre