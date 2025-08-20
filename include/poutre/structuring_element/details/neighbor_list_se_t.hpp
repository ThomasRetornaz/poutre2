
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   neighbor_list_se_t.hpp
 * @author Thomas Retornaz
 * @brief  Neighbor list SE
 *
 *
 */

#include <algorithm>
#include <memory>
#include <numeric>
#include <vector>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/structuring_element/se_interface.hpp>

namespace poutre::se::details {
/**
 * @addtogroup poutre_se_group
 *@{
 */

// Idea of design stolen from Yayi
template <ptrdiff_t Rank>
  struct neighbor_list_t : public IStructuringElement
{
public:
  static const std::ptrdiff_t rank = Rank;
  using neighbor_element = poutre::details::av::index<Rank>;
  using self_type = neighbor_list_t<Rank>;
  using parent_interface = IStructuringElement;
  using storage_type = std::vector<neighbor_element>;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;
  using reverse_iterator = typename storage_type::reverse_iterator;
  using const_reverse_iterator = typename storage_type::const_reverse_iterator;

  using se_tag = runtime_neighborhood_tag;

  // see this later
  neighbor_list_t(const neighbor_list_t &rhs) = default;
  neighbor_list_t &operator=(const neighbor_list_t &rhs) = default;
  // see this later
  neighbor_list_t(neighbor_list_t &&other) = default;
  neighbor_list_t &operator=(neighbor_list_t &&other) = default;
  ~neighbor_list_t() override = default;

  //! Constructor taking as argument a vector for each point of the structuring element
  explicit neighbor_list_t(const storage_type& v) : vector_coordinate(v)
  {}

  const_iterator begin() const {
    return vector_coordinate.begin();
  }

  const_iterator end() const {
    return vector_coordinate.end();
  }

  const storage_type& get_coordinates() const noexcept {
    return vector_coordinate;
  }

  bool is_symmetric() const noexcept
  {
    auto cp_vector_coordinate = vector_coordinate;
    std::sort(cp_vector_coordinate.begin(), cp_vector_coordinate.end());
    for (size_t i = 0; i < cp_vector_coordinate.size()/2;++i) {
      if (cp_vector_coordinate[i] != - cp_vector_coordinate[cp_vector_coordinate.size()-i-1]) {
        return false;
      }
      }
    return true;
  }

  //! Returns the upper,lower part of the neighbor list @throw runtime_error if the SE is not symmetric
  std::pair<self_type,self_type> split_upper_lower() const
  {
    if (!is_symmetric()) {
      POUTRE_RUNTIME_ERROR("split_upper_lower the SE is not symmetric");
    }
    static const neighbor_element center = vector_coordinate[0];
    std::vector<neighbor_element> upper,lower;
    for(typename storage_type::const_iterator it(vector_coordinate.begin()), ite(vector_coordinate.end());
            it != ite;
            ++it)
    {
      if(*it == center) { continue;}
      if(*it < center) {
        upper.push_back(*it);
      }
      else {
        lower.push_back(*it);
      }
    }
    return std::make_pair(self_type(upper), self_type(lower));
  }

  //! Returns a transposed copy of this structuring element
  self_type transpose() const {
    storage_type out(vector_coordinate.size());
    static const neighbor_element center = vector_coordinate[0];
    for (size_t i = 0; i < vector_coordinate.size(); ++i) {
      for(size_t j = 0; j < center.rank; ++j) {
        out[i][j] = 2*center[j] - vector_coordinate[i][j];
      }
    }
    return self_type(out);
  }

  //! Returns a copy of this structuring element without the center, assuming the center is in first pos
  self_type remove_center() const {
    static const neighbor_element center = vector_coordinate[0];
    storage_type out;
    for(typename storage_type::const_iterator it(vector_coordinate.begin()), ite(vector_coordinate.end());
        it != ite;
        ++it)
    {
      if(*it == center) continue;
      out.push_back(*it);
    }
    return self_type(out);
  }

  //! Returns the size of the structuring element
  size_t size() const {
    return vector_coordinate.size();
  }

  /*!Returns the maximal extension of this structuring element
   * The parameter @c dimension controls the dimension on which the extension is queried, while @c direction
   * specifies the direction (true for forward, false for backward).
   */
  ptrdiff_t maximum_extension(const size_t dimension, const bool direction) const {
    if(direction) {
      typename neighbor_element::value_type extension = 0;
      // for(size_t i = 0; i < vector_coordinate.size(); i++) {
      //   extension = std::max(extension, vector_coordinate[i][dimension]);
      // }
      extension = std::accumulate(
          vector_coordinate.begin(), vector_coordinate.end(), extension,
          [dimension](auto ext, const auto& coord) {
          return std::max(ext, coord[dimension]);
      });
      return extension;
    }
    else {
      typename neighbor_element::value_type extension = 0;
      // for(size_t i = 0; i < vector_coordinate.size(); i++) {
      //   extension = std::max(extension, -vector_coordinate[i][dimension]);
      // }
      extension = std::accumulate(
          vector_coordinate.begin(), vector_coordinate.end(), extension,
          [dimension](auto ext, const auto& coord) {
          return std::max(ext, -coord[dimension]);
      });
      return extension;
    }
  }



  /*!Returns the maximal extension of this structuring element on every direction*/
  std::pair<neighbor_element, neighbor_element> maximum_extension() const {
    neighbor_element min_(std::numeric_limits<typename neighbor_element::value_type>::max());
    neighbor_element max_(std::numeric_limits<typename neighbor_element::value_type>::min());

    for(size_t i = 0, j = vector_coordinate.size(); i < j; i++) {
      for(size_t dimension = 0, k = vector_coordinate[i].rank; dimension < k; dimension++) {
        min_[dimension] = std::min(min_[dimension], vector_coordinate[i][dimension]);
        max_[dimension] = std::max(max_[dimension], vector_coordinate[i][dimension]);
      }
    }

    return std::make_pair(min_, max_);
  }


  //! Strict equality between two SE
  bool operator==(const self_type& rhs) const {
    return (this == &rhs) || (vector_coordinate == rhs.vector_coordinate);
  }
  bool operator!=(const self_type& rhs) const {
    return (this != &rhs) && (vector_coordinate != rhs.vector_coordinate);
  }


  /*! Test the equality against another structuring element, without taking into account the order of neighbors
   */
  bool is_equal_unordered(const self_type& rhs) const {
      // make a copy to don't change order in place
      auto cp_vector_coordinate = vector_coordinate;
      auto cp_rhs_vector_coordinate = rhs.vector_coordinate;
      std::sort(cp_vector_coordinate.begin(), cp_vector_coordinate.end());
      std::sort(cp_rhs_vector_coordinate.begin(), cp_rhs_vector_coordinate.end());
      auto it1 = std::unique( cp_vector_coordinate.begin(), cp_vector_coordinate.end() );
      auto it2 = std::unique( cp_rhs_vector_coordinate.begin(), cp_rhs_vector_coordinate.end() );

      if(std::distance(cp_vector_coordinate.begin(), it1) != std::distance(cp_rhs_vector_coordinate.begin(), it2))
        return false;

      auto pair_mismatch = std::mismatch(cp_vector_coordinate.begin(), it1, cp_rhs_vector_coordinate.begin());
      return pair_mismatch.first == it1 && pair_mismatch.second == it2;
  }

  /*!
   *@name  Virtual methods inherited from IStructuringElement
   *@{
   */
  [[nodiscard]] se_type GetType() const override {
    return se_type::neighbor_list;
  }

protected:
   [[nodiscard]] size_t GetSize() const noexcept override  {
    return size();
  }

  [[nodiscard]] std::unique_ptr<IStructuringElement> Transpose() const override
   {
    return std::make_unique<self_type>(this->transpose());
  }

  [[nodiscard]] std::unique_ptr<IStructuringElement> RemoveCenter() const override
  {
    return std::make_unique<self_type>(this->remove_center());
  }

  [[nodiscard]] std::unique_ptr<IStructuringElement> Clone() const override {
    return std::make_unique<self_type>(*this);
  }

  [[nodiscard]] bool is_equal(const IStructuringElement& se) const noexcept override
  {
    const self_type *se_t = dynamic_cast<const self_type *>(&se);
    return (se_t != nullptr) && (*this == *se_t);
  }

  [[nodiscard]] bool is_equal_unordered(const IStructuringElement &se) const noexcept override
  {
    const self_type *se_t = dynamic_cast<const self_type *>(&se);
    return (se_t != nullptr) && this->is_equal_unordered(*se_t);
  }

  //! @}

protected:
  storage_type vector_coordinate;
};
//! @} doxygroup: poutre_se_group
}//poutre::se