/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
 
#ifndef INCLUDE_POUTRE_STRUCTURING_ELEMENT_SE_INTERFACE
#define INCLUDE_POUTRE_STRUCTURING_ELEMENT_SE_INTERFACE

#include <memory>
#include <poutre/base/config.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/structuring_element/structuring_element.hpp>

namespace poutre::se {
/**
 * @addtogroup poutre_se_interface_group Structuring Element interface
 * @ingroup poutre_se_group
 *@{
 */

class SE_API IStructuringElement
{

public:
  virtual ~IStructuringElement() = default;

  IStructuringElement() = default;

  //! Returns a new structuring element that is a transposed copy
  [[nodiscard]] virtual std::unique_ptr< IStructuringElement > Transpose() const = 0;

  //! Get @c se_type belong to @c IStructuringElement
  [[nodiscard]] virtual se_type GetType() const = 0;

  //! Removes the center point from the structuring element
  [[nodiscard]] virtual std::unique_ptr< IStructuringElement > RemoveCenter() const = 0;

  //! Returns the size of the structuring element in number of neighbors (the center, if defined, is included)
  [[nodiscard]] virtual size_t GetSize() const = 0;

  //! Returns a deep copy of this structuring element
  [[nodiscard]] virtual std::unique_ptr< IStructuringElement > Clone() const = 0;

  //! Checks the equality this structuring element with the provided one
  [[nodiscard]] virtual bool is_equal(const IStructuringElement &strel) const noexcept = 0;

  //! Checks the equality of the structuring element with the argument, but without considering any ordering
  [[nodiscard]] virtual bool is_equal_unordered(const IStructuringElement &strel) const noexcept = 0;

  class IStructuringElement &operator=(const class IStructuringElement &) = default;
  IStructuringElement(const class IStructuringElement &) = default;

  class IStructuringElement &operator=(class IStructuringElement&&) = default;
  IStructuringElement(class IStructuringElement &&) = default;
};

// TODO add factory

//! @} doxygroup: poutre_se_interface_group
}// namespace poutre::se


#endif /* INCLUDE_POUTRE_STRUCTURING_ELEMENT_SE_INTERFACE */
