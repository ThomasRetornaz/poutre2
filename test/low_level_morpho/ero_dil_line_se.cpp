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

#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>
#include <cstddef>
#include <poutre/low_level_morpho/details/ero_dil_line_se_t.hpp>
#include <string>
#include <vector>

TEST_CASE("dilateX 1D", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 5 };
  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 1 5
 0 5 6 5 0
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,1>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,1> img_out(shape);
  poutre::llm::details::t_DilateX(*img, 1, img_out);
  const std::string expected =
    "Scalar GINT64 1 5\
 5 6 6 6 5";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("erodeX 1D", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 5 };
  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 1 5
 0 5 6 5 0
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,1>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,1> img_out(shape);
  poutre::llm::details::t_ErodeX(*img, 1, img_out);
  const std::string expected =
    "Scalar GINT64 1 5\
 0 0 5 0 0";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("dilateX 2D", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 5, 5 };

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 2 5 5
 8 0 0 0 0
 0 5 5 5 0
 0 5 6 5 0
 0 5 5 5 0
 0 0 0 0 0
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64> img_out(shape);
  poutre::llm::details::t_DilateX(*img, 1, img_out);

  const std::string expected =
    "Scalar GINT64 2 5 5\
 8 8 0 0 0\
 5 5 5 5 5\
 5 6 6 6 5\
 5 5 5 5 5\
 0 0 0 0 0";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
TEST_CASE("erodeX 2D", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 5, 5 };

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 2 5 5
 8 0 0 0 0
 0 5 5 5 0
 0 5 6 5 0
 0 5 5 5 0
 0 0 0 0 0
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64> img_out(shape);
  poutre::llm::details::t_ErodeX(*img, 1, img_out);

  const std::string expected =
    "Scalar GINT64 2 5 5\
 0 0 0 0 0\
 0 0 5 0 0\
 0 0 5 0 0\
 0 0 5 0 0\
 0 0 0 0 0";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("DilateY 2D", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 5, 5 };

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 2 5 5
 8 0 0 0 0
 0 5 5 5 0
 0 5 6 5 0
 0 5 5 5 0
 0 0 0 0 0
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64> img_out(shape);
  poutre::llm::details::t_DilateY(*img, 1, img_out);

  const std::string expected =
    "Scalar GINT64 2 5 5\
 8 5 5 5 0\
 8 5 6 5 0\
 0 5 6 5 0\
 0 5 6 5 0\
 0 5 5 5 0";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("erodeY 2D", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 5, 5 };

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 2 5 5
 8 0 0 0 0
 0 5 5 5 0
 0 5 6 5 0
 0 5 5 5 0
 0 0 0 0 0
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64> img_out(shape);
  poutre::llm::details::t_ErodeY(*img, 1, img_out);

  const std::string expected =
    "Scalar GINT64 2 5 5\
 0 0 0 0 0\
 0 0 0 0 0\
 0 5 5 5 0\
 0 0 0 0 0\
 0 0 0 0 0";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}