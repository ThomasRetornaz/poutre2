
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   se_interface.hpp
 * @author Thomas Retornaz
 * @brief  Base interface for all SE
 *
 *
 */

#include <memory>
#include <poutre/base/config.hpp>
#include <poutre/structuring_element/structuring_element.hpp>

namespace poutre::se {
/**
 * @addtogroup poutre_se_interface_group Structuring Element interface
 * @ingroup poutre_se_group
 *@{
 */

enum class se_type {
  runtime,                            //! SE computed at runtime (think amoeba)
  neighbor_list,                      //!< Neighbor list structuring element
  image,                              //!< Image structuring element
  internal,                           //!< Template for internal use, not available through interface
  chain,                              //!< Chain of structuring element
};


class SE_API IStructuringElement {

    public:
      virtual ~IStructuringElement(){}

      //! Returns a new structuring element that is a transposed copy
      [[nodiscard]] virtual std::unique_ptr<IStructuringElement> Transpose() const    = 0;

      //! Get @c se_type belong to @c IStructuringElement
      [[nodiscard]] virtual se_type GetType() const  = 0;

      //! Removes the center point from the structuring element
      //! (usually useful for propagating algorithms, hit-or-miss etc.)
      [[nodiscard]] virtual std::unique_ptr<IStructuringElement> RemoveCenter() const = 0;

      //! Returns the size of the structuring element in number of neighbors (the center, if defined, is included)
      [[nodiscard]] virtual size_t GetSize() const              = 0;

      //! Returns a deep copy of this structuring element
      [[nodiscard]] virtual std::unique_ptr<IStructuringElement> Clone() const        = 0;

      //! Checks the equality this structuring element with the provided one
      [[nodiscard]] virtual bool is_equal(const IStructuringElement& ) const noexcept = 0;

      //! Checks the equality of the structuring element with the argument, but without considering any ordering
      [[nodiscard]] virtual bool is_equal_unordered(const IStructuringElement& se) const noexcept = 0;
    };

// TODO add factory

//! @} doxygroup: poutre_se_interface_group
}// namespace poutre