
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   neighbor_list_static_se_t.hpp
 * @author Thomas Retornaz
 * @brief  Neighbor list STATIC SE
 *
 *
 */
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <array>

namespace poutre::se::details {
/**
 * @addtogroup poutre_se_nl_static_group Some predefined Static Neighbor list Structuring Element
 * * @ingroup se_group
 *@{
 */

template<Common_NL_SE>
struct static_se_traits
{
 static_assert(false, "Traits must be defined for each Common_NL_SE element");
};

template<>
struct static_se_traits<Common_NL_SE::SESegmentX1D>
{
  using neighbor_element = poutre::details::av::index<1>;
  using se_tag = static_neighborhood_tag;
  static POUTRE_CONSTEXPR const std::array<neighbor_element, 3> coordinates =
    {{
            neighbor_element{+0},
            neighbor_element{-1},
            neighbor_element{+1}
    }};
  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank        = 1;
  POUTRE_STATIC_CONSTEXPR ptrdiff_t size        = 3;

  POUTRE_STATIC_CONSTEXPR ptrdiff_t maximum_extension() { return 1; }

  POUTRE_STATIC_CONSTEXPR std::pair<std::array<neighbor_element, 1>,std::array<neighbor_element, 1>> split_coordinates_upper_lower()
  {
    POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_upper = {neighbor_element{ -1 }};
    POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_lower = {neighbor_element{ +1 }};
    return std::make_pair(coordinates_upper, coordinates_lower);
  }
};


template<>
struct static_se_traits<Common_NL_SE::SESegmentX2D>
{
  using neighbor_element = poutre::details::av::index<2>;
  using se_tag = static_neighborhood_tag;
  static POUTRE_CONSTEXPR const std::array<neighbor_element, 3> coordinates =
    {{
      neighbor_element{ 0,0 },
      neighbor_element{ 0,-1 },
      neighbor_element{ 0,+1 },
    }};

  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank        = 2;
  POUTRE_STATIC_CONSTEXPR ptrdiff_t size        = 3;

  POUTRE_STATIC_CONSTEXPR ptrdiff_t maximum_extension() { return 1; }

  POUTRE_STATIC_CONSTEXPR std::pair<std::array<neighbor_element, 1>,std::array<neighbor_element, 1>> split_coordinates_upper_lower()
  {
      POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_upper = {neighbor_element{ 0,-1 }};
      POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_lower = {neighbor_element{ 0,+1 }};
    return std::make_pair(coordinates_upper, coordinates_lower);
  }
};

template<>
struct static_se_traits<Common_NL_SE::SESegmentY2D>
{
  using neighbor_element = poutre::details::av::index<2>;
  using se_tag = static_neighborhood_tag;
  static POUTRE_CONSTEXPR const std::array<neighbor_element, 3> coordinates =
    {{
      neighbor_element{ 0,0 },
      neighbor_element{ -1,+0 },
      neighbor_element{ +1,+0 },
    }};

  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank        = 2;
  POUTRE_STATIC_CONSTEXPR ptrdiff_t size        = 3;

  POUTRE_STATIC_CONSTEXPR ptrdiff_t maximum_extension() { return 1; }

  POUTRE_STATIC_CONSTEXPR std::pair<std::array<neighbor_element, 1>,std::array<neighbor_element, 1>> split_coordinates_upper_lower()
  {
    POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_upper = {neighbor_element{ -1,+0 }};
    POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_lower = {neighbor_element{ +1,+0 }};
    return std::make_pair(coordinates_upper, coordinates_lower);
  }
};

template<>
struct static_se_traits<Common_NL_SE::SESegmentY3D>
{
  using neighbor_element = poutre::details::av::index<3>;
  using se_tag = static_neighborhood_tag;
  static POUTRE_CONSTEXPR const std::array<neighbor_element, 3> coordinates =
    {{
      neighbor_element{ 0, 0,0 },
      neighbor_element{ 0, -1,+0 },
      neighbor_element{ 0, +1,+0 },
    }};

  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank        = 3;
  POUTRE_STATIC_CONSTEXPR ptrdiff_t size        = 3;

  POUTRE_STATIC_CONSTEXPR ptrdiff_t maximum_extension() { return 1; }

  POUTRE_STATIC_CONSTEXPR std::pair<std::array<neighbor_element, 1>,std::array<neighbor_element, 1>> split_coordinates_upper_lower()
  {
    POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_upper = {neighbor_element{ 0, -1,+0 }};
    POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_lower = {neighbor_element{ 0, +1,+0 }};
    return std::make_pair(coordinates_upper, coordinates_lower);
  }
};

template<>
struct static_se_traits<Common_NL_SE::SESegmentZ3D>
{
  using neighbor_element = poutre::details::av::index<3>;
  using se_tag = static_neighborhood_tag;
  static POUTRE_CONSTEXPR const std::array<neighbor_element, 3> coordinates =
    {{
      neighbor_element{ 0, 0,0 },
      neighbor_element{ -1, 0,+0 },
      neighbor_element{ +1, 0,+0 },
    }};

  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank        = 3;
  POUTRE_STATIC_CONSTEXPR ptrdiff_t size        = 3;

  POUTRE_STATIC_CONSTEXPR ptrdiff_t maximum_extension() { return 1; }

  POUTRE_STATIC_CONSTEXPR std::pair<std::array<neighbor_element, 1>,std::array<neighbor_element, 1>> split_coordinates_upper_lower()
  {
    POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_upper = {neighbor_element{ -1,0,+0 }};
    POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_lower = {neighbor_element{ +1,0, +0 }};
    return std::make_pair(coordinates_upper, coordinates_lower);
  }
};

template<>
struct static_se_traits<Common_NL_SE::SESegmentX3D>
{
  using neighbor_element = poutre::details::av::index<3>;
  using se_tag = static_neighborhood_tag;
  static POUTRE_CONSTEXPR const std::array<neighbor_element, 3> coordinates =
    {{
      neighbor_element{ 0, 0, 0 },
      neighbor_element{ 0, 0, -1 },
      neighbor_element{ 0, 0, +1 },
    }};

  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank        = 3;
  POUTRE_STATIC_CONSTEXPR ptrdiff_t size        = 3;

  POUTRE_STATIC_CONSTEXPR ptrdiff_t maximum_extension() { return 1; }

  POUTRE_STATIC_CONSTEXPR std::pair<std::array<neighbor_element, 1>,std::array<neighbor_element, 1>> split_coordinates_upper_lower()
  {
    POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_upper = {neighbor_element{ 0, 0,-1 }};
    POUTRE_CONSTEXPR std::array<neighbor_element, 1> coordinates_lower = {neighbor_element{ 0, 0,+1 }};
    return std::make_pair(coordinates_upper, coordinates_lower);
  }
};
template<>
struct static_se_traits<Common_NL_SE::SESquare2D>
{
  using neighbor_element = poutre::details::av::index<2>;
  using se_tag = static_neighborhood_tag;
  static POUTRE_CONSTEXPR const std::array<neighbor_element, 9> coordinates =
    {{
    neighbor_element{ 0,0 },
    neighbor_element{-1, -1}, neighbor_element{+0, -1}, neighbor_element{+1, -1},
    neighbor_element{-1, +0}, neighbor_element{+1, +0},
    neighbor_element{-1, +1}, neighbor_element{+0, +1}, neighbor_element{+1, +1},
    }};

  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank        = 2;
  POUTRE_STATIC_CONSTEXPR ptrdiff_t size        = 9;

  POUTRE_STATIC_CONSTEXPR ptrdiff_t maximum_extension() { return 1; }

  POUTRE_STATIC_CONSTEXPR std::pair<std::array<neighbor_element, 4>,std::array<neighbor_element, 4>> split_coordinates_upper_lower()
  {
    POUTRE_CONSTEXPR std::array<neighbor_element, 4> coordinates_upper = {
    neighbor_element{-1, -1},
    neighbor_element{-1, +0},
    neighbor_element{-1, +1},
    neighbor_element{+0, -1},
};
    POUTRE_CONSTEXPR std::array<neighbor_element, 4> coordinates_lower = {
      neighbor_element{+1, -1},
      neighbor_element{+1, +0},
      neighbor_element{+1, +1},
      neighbor_element{+0, +1},
};
    return std::make_pair(coordinates_upper, coordinates_lower);
  }
};

template<>
struct static_se_traits<Common_NL_SE::SECross2D>
{
  using neighbor_element = poutre::details::av::index<2>;
  using se_tag = static_neighborhood_tag;
  static POUTRE_CONSTEXPR const std::array<neighbor_element, 5> coordinates =
    {{
      neighbor_element{ 0,0 },
      neighbor_element{+0, -1},
      neighbor_element{-1, +0},
      neighbor_element{+1, +0},
      neighbor_element{+0, +1},
      }};

  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank        = 2;
  POUTRE_STATIC_CONSTEXPR ptrdiff_t size        = 5;

  POUTRE_STATIC_CONSTEXPR ptrdiff_t maximum_extension() { return 1; }

  POUTRE_STATIC_CONSTEXPR std::pair<std::array<neighbor_element, 2>,std::array<neighbor_element, 2>> split_coordinates_upper_lower()
  {
    POUTRE_CONSTEXPR std::array<neighbor_element, 2> coordinates_upper = {
      neighbor_element{-1, +0},
      neighbor_element{+0, -1},
  };
    POUTRE_CONSTEXPR std::array<neighbor_element, 2> coordinates_lower = {
      neighbor_element{+1, +0},
      neighbor_element{+0, +1},
};
    return std::make_pair(coordinates_upper, coordinates_lower);
  }
};

template<>
struct static_se_traits<Common_NL_SE::SECross3D>
{
  using neighbor_element = poutre::details::av::index<3>;
  using se_tag = static_neighborhood_tag;
  static POUTRE_CONSTEXPR const std::array<neighbor_element, 7> coordinates =
    {{
      neighbor_element{ 0,0,0 },
      neighbor_element{+0, +0, -1},
      neighbor_element{+0, -1, +0},
      neighbor_element{-1, +0, +0},
      neighbor_element{+1, +0, +0},
      neighbor_element{+0, +1, +0},
      neighbor_element{+0, +0, +1}
      }};

  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank        = 3;
  POUTRE_STATIC_CONSTEXPR ptrdiff_t size        = 7;

  POUTRE_STATIC_CONSTEXPR ptrdiff_t maximum_extension() { return 1; }

  POUTRE_STATIC_CONSTEXPR std::pair<std::array<neighbor_element, 3>,std::array<neighbor_element, 3>> split_coordinates_upper_lower()
  {
    POUTRE_CONSTEXPR std::array<neighbor_element, 3> coordinates_upper = {
      neighbor_element{+0, +0, -1},
      neighbor_element{+0, -1, +0},
      neighbor_element{-1, +0, +0},
  };
    POUTRE_CONSTEXPR std::array<neighbor_element, 3> coordinates_lower = {
      neighbor_element{+1, +0, +0},
      neighbor_element{+0, +1, +0},
      neighbor_element{+0, +0, +1}
};
    return std::make_pair(coordinates_upper, coordinates_lower);
  }
};

template<>
struct static_se_traits<Common_NL_SE::SESquare3D>
{
  using neighbor_element = poutre::details::av::index<3>;
  using se_tag = static_neighborhood_tag;
  static POUTRE_CONSTEXPR const std::array<neighbor_element, 27> coordinates =
    {{
      { 0,0,0 },
    {-1, -1, -1}, {+0, -1, -1}, {+1, -1, -1},
    {-1, +0, -1}, {+0, +0, -1}, {+1, +0, -1},
    {-1, +1, -1}, {+0, +1, -1}, {+1, +1, -1},
    // ----
    {-1, -1, +0}, {+0, -1, +0}, {+1, -1, +0},
    {-1, +0, +0},               {+1, +0, +0},
    {-1, +1, +0}, {+0, +1, +0}, {+1, +1, +0},
    // ----
    {-1, -1, +1}, {+0, -1, +1}, {+1, -1, +1},
    {-1, +0, +1}, {+0, +0, +1}, {+1, +0, +1},
    {-1, +1, +1}, {+0, +1, +1}, {+1, +1, +1},
    }};

  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank        = 3;
  POUTRE_STATIC_CONSTEXPR ptrdiff_t size        = 27;

  POUTRE_STATIC_CONSTEXPR ptrdiff_t maximum_extension() { return 1; }

  POUTRE_STATIC_CONSTEXPR std::pair<std::array<neighbor_element, 13>,std::array<neighbor_element, 13>> split_coordinates_upper_lower()
  {
    POUTRE_CONSTEXPR std::array<neighbor_element, 13> coordinates_upper = {
      neighbor_element{ +1, +0, +0 },
      neighbor_element{ -1, +1, +0 },
      neighbor_element{ +0, +1, +0 },
      neighbor_element{ +1, +1, +0 },
      neighbor_element{ -1, -1, +1 },
      neighbor_element{ 0, -1, +1 },
      neighbor_element{ +1, -1, +1 },
      neighbor_element{ -1, 0, +1 },
      neighbor_element{ 0, 0, +1 },
      neighbor_element{ +1, 0, +1 },
      neighbor_element{ -1, +1, +1 },
      neighbor_element{ 0, +1, +1 },
      neighbor_element{ +1, +1, +1 },
    };
    POUTRE_CONSTEXPR std::array<neighbor_element, 13> coordinates_lower = {
      neighbor_element{-1, -1, -1},
      neighbor_element{0, -1, -1},
      neighbor_element{1, -1, -1},
      neighbor_element{-1, 0, -1},
      neighbor_element{0, 0, -1},
      neighbor_element{+1, 0, -1},
      neighbor_element{-1, +1, -1},
      neighbor_element{0, +1, -1},
      neighbor_element{+1, +1, -1},
      neighbor_element{-1, -1, 0},
      neighbor_element{0, -1, 0},
      neighbor_element{+1, -1, 0},
      neighbor_element{-1, 0, 0},
    };
    return std::make_pair(coordinates_upper, coordinates_lower);
  }
};
//! @} doxygroup: poutre_se_nl_static_group
}