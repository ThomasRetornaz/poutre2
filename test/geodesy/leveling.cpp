// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "poutre/geodesy/leveling.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <poutre/base/image_interface.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

#include <memory>
#include <string>

TEST_CASE("low_leveling 2D SESquare2D", "[geodesy]")
{
  const auto img_ref = poutre::ImageFromString(
    "Scalar GINT64 2 3 3 \
2 2 2 \
5 5 5 \
1 1 1");
  const auto img_marker = poutre::ImageFromString(
    "Scalar GINT64 2 3 3 \
1 1 1 \
3 3 3 \
4 4 4");

  auto img_out = poutre::CloneGeometry(*img_marker); // NOLINT

  REQUIRE(img_out.get() != nullptr);

  poutre::geo::low_leveling(*img_ref, *img_marker ,poutre::se::Common_NL_SE::SESquare2D,*img_out);

  const std::string expected =
    "Scalar GINT64 2 3 3 \
2 2 2 \
4 4 4 \
1 1 1";
  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("high_leveling 2D SESquare2D", "[geodesy]")
{
  const auto img_ref = poutre::ImageFromString(
    "Scalar GINT64 2 3 3 \
2 2 2 \
5 5 5 \
1 1 1");
  const auto img_marker = poutre::ImageFromString(
    "Scalar GINT64 2 3 3 \
1 1 1 \
3 3 3 \
4 4 4");

  auto img_out = poutre::CloneGeometry(*img_marker); // NOLINT

  REQUIRE(img_out.get() != nullptr);

  poutre::geo::high_leveling(*img_ref, *img_marker ,poutre::se::Common_NL_SE::SESquare2D,*img_out);

  const std::string expected =
    "Scalar GINT64 2 3 3 \
2 2 2 \
5 5 5 \
3 3 3";
  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("leveling 1D", "[geodesy]")
{
  {
    const auto img_ref = poutre::ImageFromString(
      "Scalar GUINT8 1 14 1 2 1 1 4 4 4 4 4 0 0 0 0 0");
    const auto img_marker = poutre::ImageFromString(
      "Scalar GUINT8 1 14 1 0 0 0 0 0 0 6 6 6 6 6 0 0 2 0");

    auto img_out = poutre::CloneGeometry(*img_marker); // NOLINT

    REQUIRE(img_out.get() != nullptr);

    poutre::geo::leveling(*img_ref, *img_marker ,poutre::se::Common_NL_SE::SESegmentX1D,*img_out);

    const std::string expected = "Scalar GUINT8 1 14 1 1 1 1 4 4 4 4 4 0 0 0 0 0";
    const auto img_str = poutre::ImageToString(*img_out);
    REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
  }
  {
    const auto img_ref = poutre::ImageFromString(
      "Scalar GUINT8 1 8 1 1 1 1 5 5 5 5");
    const auto img_marker = poutre::ImageFromString(
      "Scalar GUINT8 1 8 4 4 4 4 2 2 2 2");

    auto img_out = poutre::CloneGeometry(*img_marker); // NOLINT

    REQUIRE(img_out.get() != nullptr);

    poutre::geo::leveling(*img_ref, *img_marker ,poutre::se::Common_NL_SE::SESegmentX1D,*img_out);

    const std::string expected = "Scalar GUINT8 1 8 2 2 2 2 4 4 4 4";
    const auto img_str = poutre::ImageToString(*img_out);
    REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
  }
}