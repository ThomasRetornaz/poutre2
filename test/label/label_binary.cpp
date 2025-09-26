// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "poutre/label/label_binary.hpp"


#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <poutre/base/image_interface.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

#include <memory>
#include <string>

TEST_CASE("label_binary 2D SESquare2D", "[label]")
{
  const auto img = poutre::ImageFromString(
    "Scalar GINT64 2 7 11 \
1 1 1 1 1 0 0 0 1 0 0 \
1 0 0 0 1 0 0 0 0 1 0 \
1 1 0 1 1 0 0 1 1 1 0 \
0 0 1 0 0 0 0 0 0 0 0 \
1 1 0 1 1 0 1 0 0 0 1 \
1 0 0 0 1 0 1 0 1 0 1 \
1 1 1 1 1 0 1 1 1 1 1");

  auto img_out = poutre::CloneGeometry(*img); // NOLINT
  REQUIRE(img_out.get() != nullptr);

  poutre::label::label_binary(*img,poutre::se::Common_NL_SE::SESquare2D,*img_out);

  const std::string expected =
    "Scalar GINT64 2 7 11 \
1 1 1 1 1 0 0 0 2 0 0 \
1 0 0 0 1 0 0 0 0 2 0 \
1 1 0 1 1 0 0 2 2 2 0 \
0 0 1 0 0 0 0 0 0 0 0 \
1 1 0 1 1 0 3 0 0 0 3 \
1 0 0 0 1 0 3 0 3 0 3 \
1 1 1 1 1 0 3 3 3 3 3";

  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("label_binary 2D SECross2D", "[label]")
{
  const auto img = poutre::ImageFromString(
    "Scalar GINT64 2 7 11 \
1 1 1 1 1 0 0 0 1 0 0 \
1 0 0 0 1 0 0 0 0 1 0 \
1 1 0 1 1 0 0 1 1 1 0 \
0 0 1 0 0 0 0 0 0 0 0 \
1 1 0 1 1 0 1 0 0 0 1 \
1 0 0 0 1 0 1 0 1 0 1 \
1 1 1 1 1 0 1 1 1 1 1");

  auto img_out = poutre::CloneGeometry(*img); // NOLINT
  REQUIRE(img_out.get() != nullptr);

  poutre::label::label_binary(*img,poutre::se::Common_NL_SE::SECross2D,*img_out);

  const std::string expected =
    "Scalar GINT64 2 7 11 \
1 1 1 1 1 0 0 0 2 0 0 \
1 0 0 0 1 0 0 0 0 3 0 \
1 1 0 1 1 0 0 3 3 3 0 \
0 0 4 0 0 0 0 0 0 0 0 \
5 5 0 5 5 0 6 0 0 0 6 \
5 0 0 0 5 0 6 0 6 0 6 \
5 5 5 5 5 0 6 6 6 6 6";

  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("label_binary 2D SESquare2D empty image", "[label]")
{
  const auto img = poutre::ImageFromString(
    "Scalar GINT64 2 7 11 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0");

  auto img_out = poutre::CloneGeometry(*img); // NOLINT
  REQUIRE(img_out.get() != nullptr);

  poutre::label::label_binary(*img,poutre::se::Common_NL_SE::SESquare2D,*img_out);

  const std::string expected =
    "Scalar GINT64 2 7 11 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0 \
0 0 0 0 0 0 0 0 0 0 0";

  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("label_binary 2D SESquare2D labels blobs", "[label]")
{
  const auto img = poutre::ImageFromString(
    "Scalar GINT64 2 7 11 \
1 1 1 1 1 0 0 0 7 0 0 \
1 0 0 0 1 0 0 0 0 6 0 \
1 1 0 1 1 0 0 4 4 5 0 \
0 0 2 0 0 0 0 0 0 0 0 \
1 1 0 1 1 0 1 0 0 0 1 \
1 0 0 0 1 0 1 0 1 0 1 \
1 1 9 1 1 0 1 3 1 3 1");

  auto img_out = poutre::CloneGeometry(*img); // NOLINT
  REQUIRE(img_out.get() != nullptr);

  poutre::label::label_binary(*img,poutre::se::Common_NL_SE::SESquare2D,*img_out);

  const std::string expected =
    "Scalar GINT64 2 7 11 \
1 1 1 1 1 0 0 0 2 0 0 \
1 0 0 0 1 0 0 0 0 2 0 \
1 1 0 1 1 0 0 2 2 2 0 \
0 0 1 0 0 0 0 0 0 0 0 \
1 1 0 1 1 0 3 0 0 0 3 \
1 0 0 0 1 0 3 0 3 0 3 \
1 1 1 1 1 0 3 3 3 3 3";

  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
