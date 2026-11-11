/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "poutre/label/label_flat_zones.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <poutre/base/image_interface.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

#include <memory>
#include <string>

TEST_CASE("label_flat_zones 2D SESquare2D", "[label]")
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

  poutre::label::label_flat_zones(*img,poutre::se::Common_NL_SE::SESquare2D,*img_out);

  const std::string expected =
    "Scalar GINT64 2 7 11 \
1 1 1 1 1 2 2 2 3 2 2 \
1 2 2 2 1 2 2 2 2 3 2 \
1 1 2 1 1 2 2 3 3 3 2 \
2 2 1 2 2 2 2 2 2 2 2 \
1 1 2 1 1 2 4 2 2 2 4 \
1 2 2 2 1 2 4 2 4 2 4 \
1 1 1 1 1 2 4 4 4 4 4";

  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("label_flat_zones 2D SESquare2D empty image", "[label]")
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

  poutre::label::label_flat_zones(*img,poutre::se::Common_NL_SE::SESquare2D,*img_out);

  const std::string expected =
    "Scalar GINT64 2 7 11 \
1 1 1 1 1 1 1 1 1 1 1 \
1 1 1 1 1 1 1 1 1 1 1 \
1 1 1 1 1 1 1 1 1 1 1 \
1 1 1 1 1 1 1 1 1 1 1 \
1 1 1 1 1 1 1 1 1 1 1 \
1 1 1 1 1 1 1 1 1 1 1 \
1 1 1 1 1 1 1 1 1 1 1";

  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
