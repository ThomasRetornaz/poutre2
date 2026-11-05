//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file max_tree.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief Define common max_tree data structure
 * @version 0.1
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "image_t.hpp"
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <vector>

#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/tufa.hpp>

namespace poutre::details {
/**
 * @addtogroup max_tree_group Max Tree facilities
 * @ingroup poutre_base_group
 *@{
 */

// --- Visitor Interface ---
template< class valuetype = std::uint8_t > class MaxTree;

template< class valuetype = std::uint8_t > class MaxTreeVisitor
{
public:
  virtual void visit(MaxTree< valuetype > &tree) = 0;
  virtual ~MaxTreeVisitor() = default;
};


template< typename Tmarker, typename Tmask, typename Tout, ptrdiff_t Rank >
void t_geodesic_distance(const poutre::details::image_t< Tmarker, Rank > &i_marker,
                         const poutre::details::image_t< Tmask, Rank > &i_mask,
                         poutre::se::Common_NL_SE nl_static,

template <class valuetype = std::uint8_t, ptrdiff_t Rank >
class MaxTree
{
private:
  std::vector< std::size_t > m_parent;
  std::vector< std::size_t > m_sorted_indices;
  const poutre::details::image_t< valuetype, Rank > &m_image;

public:
  MaxTree(const poutre::details::image_t< valuetype, Rank > &m_image) : m_image(m_image) { build(m_image); }
  MaxTree(const MaxTree &) = delete;
  MaxTree &operator=(const MaxTree &) = delete;
  MaxTree(MaxTree && other) = default;
  MaxTree &operator=(MaxTree &&other) = default;

  virtual ~MaxTree() = default;

  void accept(MaxTreeVisitor< valuetype > & visitor) { visitor.visit(*this); }

private:
  void build(const poutre::details::image_t< valuetype, Rank > &image,
             poutre::se::Common_NL_SE nl_static,
             image_t< class valuetype > &o_vout)
  {
    // Initialize parent and sorted_indices
    m_parent.resize(image.size());
    std::iota(m_parent.begin(), m_parent.end(), 0); // Each pixel is its own parent initially

    m_sorted_indices.resize(image.size());
    std::iota(m_sorted_indices.begin(), m_sorted_indices.end(), 0);

    // Sort indices based on pixel values in descending order
    // FIXME provide counting sort for integer types for better performance
    std::sort(m_sorted_indices.begin(), m_sorted_indices.end(), [&image](std::size_t a, std::size_t b) {
      return image[a] > image[b];// Sort by pixel value
    });

    // Build the max-tree structure
    for (std::size_t idx : m_sorted_indices) {
      // For each pixel, find its parent based on the neighborhood
      // This is a simplified version; actual implementation may vary
      for (const auto &neighbor : get_neighbors(idx, image.shape())) {
        if (image[neighbor] > image[idx]) {
          m_parent[idx] = neighbor;// Set parent to the neighbor with a higher value
          break;
        }
      }
    }
  }
};

//! @} doxygroup: max_tree_group
}// namespace poutre::details
