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
#include <poutre/io/loader.hpp>
#include <poutre/io/writer.hpp>
//#include <cstddef>
#include <filesystem>
#include <string>
// #include <vector>
namespace fs = std::filesystem;

TEST_CASE("load/save 2D gray 8 png", "[io]")
{
  fs::path const tempDir = "POUTRE_NRT_IO_TMP_DIR";
  if (!fs::is_directory(tempDir))
  {
    fs::create_directory(tempDir);
  }

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GUINT8 2 5 5
 8 0 0 0 0
 0 255 255 255 0
 0 255 127 255 0
 0 255 255 255 0
 0 0 0 0 0
)");

  fs::path const image_path = tempDir / "write_test_GUINT8.png";
  auto writer = poutre::io::ImageWriter().SetPath(image_path.string());
  writer.Write(*img_in);

  //load again and check
  auto loader = poutre::io::ImageLoader().SetPath(image_path.string());
  auto loaded_img = loader.Load();

  const std::string expected =
      "Scalar GUINT8 2 5 5\
 8 0 0 0 0\
 0 255 255 255 0\
 0 255 127 255 0\
 0 255 255 255 0\
 0 0 0 0 0";

  const auto img_str = poutre::ImageToString(*loaded_img);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("load/save 2D RGB 8 png", "[io]")
{
  fs::path const tempDir = "POUTRE_NRT_IO_TMP_DIR";
  if (!fs::is_directory(tempDir))
  {
    fs::create_directory(tempDir);
  }

  const auto img_in = poutre::ImageFromString(
  R"(3Planes GUINT8 2 2 3
0 0 0 255 0 0 0 255 0
0 0 255 228 135 255 255 255 255
)");

  fs::path const image_path = tempDir / "write_test_GUINT8_RGB.png";
  auto writer = poutre::io::ImageWriter().SetPath(image_path.string());
  writer.Write(*img_in);

  //load again and check
  auto loader = poutre::io::ImageLoader().SetPath(image_path.string());
  auto loaded_img = loader.Load();

  const std::string expected =
    "3Planes GUINT8 2 2 3\
 0 0 0 255 0 0 0 255 0\
 0 0 255 228 135 255 255 255 255";

  const auto img_str = poutre::ImageToString(*loaded_img);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("load/save 2D RGBA 8", "[io]")
{
  fs::path const tempDir = "POUTRE_NRT_IO_TMP_DIR";
  if (!fs::is_directory(tempDir))
  {
    fs::create_directory(tempDir);
  }

  const auto img_in = poutre::ImageFromString(
  R"(4Planes GUINT8 2 2 3
0 0 0 255 255 0 0 255 0 255 0 255
0 0 255 255 228 135 255 255 255 255 255 255
)");

  fs::path const image_path = tempDir / "write_test_GUINT8_RGBA.png";
  auto writer = poutre::io::ImageWriter().SetPath(image_path.string());
  writer.Write(*img_in);

  //load again and check
  auto loader = poutre::io::ImageLoader().SetPath(image_path.string());
  auto loaded_img = loader.Load();

  const std::string expected =
    "4Planes GUINT8 2 2 3\
 0 0 0 255 255 0 0 255 0 255 0 255\
 0 0 255 255 228 135 255 255 255 255 255 255";

  const auto img_str = poutre::ImageToString(*loaded_img);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}