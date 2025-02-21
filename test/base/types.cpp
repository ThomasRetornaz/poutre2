// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutre/base/types.hpp>

#include <catch2/catch_test_macros.hpp>

// NOLINTBEGIN

TEST_CASE("types", "[compound_pixel_container]")
{
  using c5G32 = poutre::compound_type<poutre::pUINT32, 5>;
  REQUIRE(poutre::get_dim<c5G32>::dim == 5);
  c5G32 p00;
  p00.fill(10);
  c5G32 p0 = { 10, 10, 10, 10, 10 };
  REQUIRE(p00 == p0);
  p00[4] = 11;
  REQUIRE(p00 != p0);
}

TEST_CASE("types", "[compound_pixel_3]")
{
  using c3G8 = poutre::compound_type<poutre::pUINT8, 3>;
  REQUIRE(poutre::get_dim<c3G8>::dim == 3);
  c3G8 p00(0, 1, 2);
  REQUIRE(p00[0] == 0);
  REQUIRE(p00[1] == 1);
  REQUIRE(p00[2] == 2);

  c3G8 p0; //default uninitialized
  p0.fill(12);
  REQUIRE(p0[0] == 12);
  REQUIRE(p0[1] == 12);
  REQUIRE(p0[2] == 12);

  c3G8 p(1, 2, 3);
  REQUIRE(p0 != p);
  REQUIRE(p[0] == 1);
  REQUIRE(p[1] == 2);
  REQUIRE(p[2] == 3);

  c3G8 pc = p;
  REQUIRE(&pc != &p);
  REQUIRE(pc[0] == 1);
  REQUIRE(pc[1] == 2);
  REQUIRE(pc[2] == 3);
  pc[0] = 3;
  pc[1] = 4; //-V112
  pc[2] = 5;
  REQUIRE(pc[0] == 3);
  REQUIRE(pc[1] == 4); //-V112

  //p0 = std::move(pc);
  //EXPECT_TRUE(p0[0] == 3);
  //EXPECT_TRUE(p0[1] == 4);
  //EXPECT_TRUE(p0[2] == 5);
}

TEST_CASE("types", "[compound_pixel_4]")
{
  using c4G32 = poutre::compound_type<poutre::pUINT32, 4>;
  REQUIRE(poutre::get_dim<c4G32>::dim == 4); //-V112
  // c4G32 p00(0, 1, 2, 3);
  // REQUIRE(p00[0] == 0);
  // REQUIRE(p00[1] == 1);
  // REQUIRE(p00[2] == 2);
  // REQUIRE(p00[3] == 3);

  c4G32 p0; //default uninitialized
  p0.fill(static_cast<poutre::pINT32>(12));
  REQUIRE(p0[0] == 12);
  REQUIRE(p0[1] == 12);
  REQUIRE(p0[2] == 12);
  REQUIRE(p0[3] == 12);

  c4G32 p = {1, 2, 3, 4}; //-V112
  REQUIRE(p0 != p);
  REQUIRE(p[0] == 1);
  REQUIRE(p[1] == 2);
  REQUIRE(p[2] == 3);
  REQUIRE(p[3] == 4); //-V112

  c4G32 pc = p;
  REQUIRE(&pc != &p);
  REQUIRE(pc[0] == 1);
  REQUIRE(pc[1] == 2);
  REQUIRE(pc[2] == 3);
  REQUIRE(pc[3] == 4); //-V112
  pc[0] = 3;
  pc[1] = 4; //-V112
  pc[2] = 5;
  pc[3] = 6;
  REQUIRE(pc[0] == 3);
  REQUIRE(pc[1] == 4); //-V112
  REQUIRE(pc[2] == 5);
  
  //p0 = std::move(pc);
  //EXPECT_TRUE(p0[0] == 3);
  //EXPECT_TRUE(p0[1] == 4);
  //EXPECT_TRUE(p0[2] == 5);
  //EXPECT_TRUE(p0[3] == 6);
}

// NOLINTEND