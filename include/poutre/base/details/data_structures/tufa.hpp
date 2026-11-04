//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file tufa.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief Define common tarjan union find data structure
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <cstddef>
#include <numeric>
#include <vector>
#include<ranges>

namespace poutre::details {

/**
 * @addtogroup union_find_group Union Find facilities
 * @ingroup poutre_base_group
 *@{
 */

// NOLINTBEGIN
class UnionFind
{
public:
  //! ctor
  explicit UnionFind(std::size_t size) : parent(size), rank(size, 0)
  {
    // Initialize each element as its own parent (iota fills with 0, 1, ..., size-1)
    std::ranges::iota(parent, 0);
  }

  // Iterative Find with Path Compression
  // Guaranteed O(1) stack space, avoiding stack overflow on big data
  std::size_t find(std::size_t i) noexcept
  {
    std::size_t root = i;

    // Step 1: Find the actual root of the set
    while (root != parent[root]) {
      root = parent[root];
    }

    // Step 2: Iterative Path Compression
    // Update all nodes along the path to point directly to the root
    std::size_t curr = i;
    while (curr != root) {
      std::size_t next = parent[curr];
      parent[curr] = root;
      curr = next;
    }

    return root;
  }

  // Union by Rank
  // Attaches the shallower tree under the deeper tree to keep the structure flat
  bool unite(std::size_t i, std::size_t j) noexcept
  {
    std::size_t root_i = find(i);
    std::size_t root_j = find(j);

    // They are already in the same set
    if (root_i == root_j) {
      return false;
    }

    // Union by Rank optimization
    if (rank[root_i] < rank[root_j]) {
      parent[root_i] = root_j;
    } else if (rank[root_i] > rank[root_j]) {
      parent[root_j] = root_i;
    } else {
      parent[root_j] = root_i;
      rank[root_i]++;
    }
    return true;
  }

private:
  std::vector< std::size_t > parent;
  std::vector< std::size_t > rank;
};
// NOLINTEND
//! @} doxygroup: union_find_group
}// namespace poutre