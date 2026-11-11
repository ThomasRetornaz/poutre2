/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
 
#ifndef INCLUDE_POUTRE_STRUCTURING_ELEMENT_SE_CHAINED
#define INCLUDE_POUTRE_STRUCTURING_ELEMENT_SE_CHAINED

#include <memory>
#include <poutre/base/config.hpp>
#include <poutre/structuring_element/se_interface.hpp>
#include <poutre/structuring_element/structuring_element.hpp>
#include <vector>

namespace poutre::se {
/**
 * @addtogroup poutre_se_chained_group Chain of SE, usefull to chain neighbor list se after a decomposition of a larger one
 * * @ingroup se_group
 *@{
 */

class SE_API ChainedStructuringElement : public IStructuringElement
{

public:
  ~ChainedStructuringElement() override = default;
  
  ChainedStructuringElement() = default;

  // see this later
  ChainedStructuringElement(const ChainedStructuringElement &rhs) = delete;
  ChainedStructuringElement &operator=(const ChainedStructuringElement &rhs) = delete;
  // see this later
  ChainedStructuringElement(ChainedStructuringElement &&other) = delete;
  ChainedStructuringElement &operator=(ChainedStructuringElement &&other) = delete;

  //! Append the se to the list \warning{the ownership is transferred}
  void append(std::unique_ptr< IStructuringElement > strel);

  /*!
   *@name  Virtual methods inherited from IStructuringElement
   *@{
   */

  [[nodiscard]] se_type GetType() const override { return se_type::chain; }

  //! Returns a new structuring element that is a transposed copy
  [[nodiscard]] std::unique_ptr< IStructuringElement > Transpose() const override;

  /*!@brief Removes the center point from the structuring element
   * @warning Not implemented
   * @throw runtime_error
   */
  [[nodiscard]] std::unique_ptr< IStructuringElement > RemoveCenter() const override;

  /*!@brief Returns the size of the structuring element in number of neighbors (the center, if defined, is included)
   * @warning return 0
   */
  [[nodiscard]] std::size_t GetSize() const override;

  //! Returns a deep copy of this structuring element
  [[nodiscard]] std::unique_ptr< IStructuringElement > Clone() const override;

  /*!@brief Checks the equality this structuring element with the provided one
   * @warning tests equality item by item
   */
  [[nodiscard]] bool is_equal(const IStructuringElement &strel) const noexcept override;

  /*! Checks the equality of the structuring element with the argument, but without considering any ordering
   * @warning tests equality item by item
   */
  [[nodiscard]] bool is_equal_unordered(const IStructuringElement &strel) const noexcept override;

  //! @}
private:
  std::vector< std::unique_ptr< IStructuringElement > > m_vect_se;
};

//! @} doxygroup: poutre_se_chained_group
}// namespace poutre::se


#endif /* INCLUDE_POUTRE_STRUCTURING_ELEMENT_SE_CHAINED */
