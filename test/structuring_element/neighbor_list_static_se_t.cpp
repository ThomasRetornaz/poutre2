// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutre/structuring_element/details/neighbor_list_static_se_t.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
//#include <poutre/base/details/data_structures/array_view.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("static traits", "[se_nl]")
{
  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SESegmentX1D>::size == 3);
  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SESegmentX1D>::rank == 1);

  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SESegmentX2D>::size == 3);
  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SESegmentX2D>::rank == 2);

  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SESegmentY2D>::size == 3);
  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SESegmentY2D>::rank == 2);

  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SESquare2D>::size == 9);
  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SESquare2D>::rank == 2);

  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SECross2D>::size == 5);
  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SECross2D>::rank == 2);

  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SECross3D>::size == 7);
  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SECross3D>::rank == 3);

  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SESquare3D>::size == 27);
  STATIC_REQUIRE(poutre::se::details::static_se_traits<poutre::se::Common_NL_SE::SESquare3D>::rank == 3);
}