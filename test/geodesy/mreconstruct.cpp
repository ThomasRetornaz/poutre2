// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <poutre/base/image_interface.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/geodesy/mreconstruct.hpp>

#include <memory>
#include <string>

TEST_CASE("reconstruct dilate 2D SESquare2D", "[geodesy]")
{
  using PtrInterface = std::unique_ptr<poutre::IInterface>;
  const auto img_marker = poutre::ImageFromString(
  "Scalar GINT64 2 6 5 \
0 0 0 0 0 \
0 9 0 0 0 \
0 0 0 0 0 \
0 0 0 3 0 \
0 0 0 0 0 \
0 9 0 0 0");

  const auto img_mask = poutre::ImageFromString(
  "Scalar GINT64 2 6 5 \
0 0 0 0 0 \
0 8 7 1 0 \
0 9 0 4 0 \
0 0 0 4 0 \
0 0 6 5 6 \
0 0 9 8 9");

  PtrInterface img_out = poutre::CloneGeometry(*img_marker);
  REQUIRE(img_out.get() != nullptr);

  poutre::geo::Reconstruction(poutre::geo::reconstruction_type::dilate,*img_marker,*img_mask, poutre::se::Common_NL_SE::SESquare2D, *img_out);
  const std::string expected =
    "Scalar GINT64 2 6 5 \
0 0 0 0 0 \
0 8 7 1 0 \
0 8 0 4 0 \
0 0 0 4 0 \
0 0 4 4 4 \
0 0 4 4 4";
  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}


TEST_CASE("reconstruct dilate 2D SECross2D", "[geodesy]")
{
  using PtrInterface = std::unique_ptr<poutre::IInterface>;
  const auto img_marker = poutre::ImageFromString(
    R"(Scalar GUINT8 2 6 5
 0 0 0 0 0
 0 9 0 0 0
 0 0 0 0 0
 0 0 0 3 0
 0 0 0 0 0
 0 9 0 0 0
)");

  const auto img_mask = poutre::ImageFromString(
    R"(Scalar GUINT8 2 6 5
 0 0 0 0 0
 0 8 7 1 0
 0 9 0 4 0
 0 0 0 4 0
 0 0 6 5 6
 0 0 9 8 9
)");
  PtrInterface img_out = poutre::Clone(*img_marker);
  REQUIRE(img_out.get() != nullptr);

  poutre::geo::Reconstruction(poutre::geo::reconstruction_type::dilate,*img_marker,*img_mask, poutre::se::Common_NL_SE::SECross2D, *img_out);
  const std::string expected =
    "Scalar GUINT8 2 6 5 \
0 0 0 0 0 \
0 8 7 1 0 \
0 8 0 3 0 \
0 0 0 3 0 \
0 0 3 3 3 \
0 0 3 3 3";
  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("reconstruct erode 2D SESquare2D", "[geodesy]")
{
  using PtrInterface = std::unique_ptr<poutre::IInterface>;
  const auto img_marker = poutre::ImageFromString(
  "Scalar GINT64 2 6 5 \
10 10 10 10 10 \
10 9 10 10 10 \
10 10 10 10 10 \
10 10 10 3 10 \
10 10 10 10 10 \
10 9 10 10 10");

  const auto img_mask = poutre::ImageFromString(
  "Scalar GINT64 2 6 5 \
10 10 10 10 10 \
10 2 3 9 10 \
10 1 10 6 10 \
10 10 10 6 10 \
10 10 4 5 4 \
10 10 1 2 1");

  PtrInterface img_out = poutre::CloneGeometry(*img_marker);
  REQUIRE(img_out.get() != nullptr);

  poutre::geo::Reconstruction(poutre::geo::reconstruction_type::erode,*img_marker,*img_mask, poutre::se::Common_NL_SE::SESquare2D, *img_out);
  const std::string expected =
    "Scalar GINT64 2 6 5 \
10 10 10 10 10 \
10 6 6 9 10 \
10 6 10 6 10 \
10 10 10 6 10 \
10 10 6 6 6 \
10 10 6 6 6";
  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("reconstruct erode 2D SECross2D", "[geodesy]")
{
  using PtrInterface = std::unique_ptr<poutre::IInterface>;
  const auto img_marker = poutre::ImageFromString(
    "Scalar GINT64 2 6 5 \
10 10 10 10 10 \
10 9 10 10 10 \
10 10 10 10 10 \
10 10 10 3 10 \
10 10 10 10 10 \
10 9 10 10 10");

  const auto img_mask = poutre::ImageFromString(
  "Scalar GINT64 2 6 5 \
10 10 10 10 10 \
10 2 3 9 10 \
10 1 10 6 10 \
10 10 10 6 10 \
10 10 4 5 4 \
10 10 1 2 1");

  PtrInterface img_out = poutre::Clone(*img_marker);
  REQUIRE(img_out.get() != nullptr);

  poutre::geo::Reconstruction(poutre::geo::reconstruction_type::erode,*img_marker,*img_mask, poutre::se::Common_NL_SE::SECross2D, *img_out);
  const std::string expected =
    "Scalar GINT64 2 6 5 \
10 10 10 10 10 \
10 9 9 9 10 \
10 9 10 6 10 \
10 10 10 6 10 \
10 10 6 6 6 \
10 10 6 6 6";
  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
