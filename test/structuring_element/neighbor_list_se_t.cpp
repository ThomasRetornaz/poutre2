// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

// #include <poutre/structuring_element/details/neighbor_list_se_t.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/structuring_element/predefined_nl_se.hpp>
#include <catch2/catch_test_macros.hpp>
//#include <catch2/matchers/catch_matchers.hpp>
//#include <catch2/matchers/catch_matchers_vector.hpp>

TEST_CASE("remove center", "[se_nl]")
{
  const auto se_square = poutre::se::SESquare2D;
  const auto se_square_no_center = se_square.remove_center();
  REQUIRE(se_square_no_center.is_symmetric());
  REQUIRE(se_square_no_center.get_coordinates().size()==8);
}

TEST_CASE("transpose", "[se_nl]")
{
 const auto se_square = poutre::se::SESquare2D;
 const auto se_square_transposed = se_square.transpose();
 REQUIRE(se_square.is_equal_unordered(se_square_transposed));
}

TEST_CASE("symmetric", "[se_nl]")
{
  const auto se_square = poutre::se::SESquare2D;
  REQUIRE(se_square.is_symmetric());
}

TEST_CASE("split", "[se_nl]")
{
  const auto se_square = poutre::se::SESquare2D;
  const auto [upper,lower] = se_square.split_upper_lower();
  REQUIRE(!upper.is_symmetric());
  REQUIRE(!lower.is_symmetric());
}

TEST_CASE("extension", "se_nl")
{
  const auto se_square = poutre::se::SESquare2D;
  REQUIRE(se_square.maximum_extension(0,true)==1);
  REQUIRE(se_square.maximum_extension(0,false)==1);
  REQUIRE(se_square.maximum_extension(1,true)==1);
  REQUIRE(se_square.maximum_extension(1,false)==1);
}

TEST_CASE("extension pair", "se_nl")
{
  const auto se_square = poutre::se::SESquare2D;
  const auto [ext_upper,ext_lower] = se_square.maximum_extension();
  REQUIRE(ext_lower==poutre::details::av::idx2d{1,1});
  REQUIRE(ext_upper==poutre::details::av::idx2d{-1,-1});
}