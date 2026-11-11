/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>
#include <cstddef>
#include <poutre/pixel_processing/details/transpose_op_t.hpp>
#include <string>
#include <vector>

TEST_CASE("2D along x", "[isometry]")
{
  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 2 2 5
 1 6
 2 7
 3 8
 4 9
 5 10
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());

  const std::vector<std::size_t> shape = { 5, 2 };
  poutre::details::image_t<poutre::pINT64> img2(shape);

  poutre::details::t_transpose(*img, img2);

  const std::string expected =
    "Scalar GINT64 2 5 2 \
1 2 3 4 5 \
6 7 8 9 10";

  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}