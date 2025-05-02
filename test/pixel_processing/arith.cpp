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
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>
// #include <poutre/base/types_traits.hpp>
#include <cstddef>
#include <poutre/pixel_processing/details/arith_op_t.hpp>
#include <string>
#include <vector>

TEST_CASE("negate same ptr type", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT64> img1(shape);
  const poutre::pINT32 value = 1;
  img1.fill(value);
  poutre::details::image_t<poutre::pINT64> img2(shape);
  // call simd path
  poutre::details::t_ArithInvert(img1, img2);

  const std::string expected =
    "Scalar GINT64 2 5 6 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("negate diff ptr type", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT32> img1(shape);
  const poutre::pINT32 value = 1;
  img1.fill(value);
  poutre::details::image_t<poutre::pINT64> img2(shape);
  // call UnaryOpDispatcher array view template specialization,fall back ptr
  poutre::details::t_ArithInvert(img1, img2);
  const std::string expected =
    "Scalar GINT64 2 5 6 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("negate strided view one idx", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT32> img1(shape);
  const poutre::pINT32 value = 1;
  img1.fill(value);
  poutre::details::image_t<poutre::pINT32> img2(shape);
  const poutre::pINT32 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  // Extract section
  auto s_img1 = v_img1.section(/*poutre::idx2d*/ { 1, 1 });
  auto s_img2 = v_img2.section(/*poutre::idx2d*/ { 1, 1 });
  REQUIRE(s_img1.size() == 20);
  REQUIRE(s_img1.bound() == (poutre::details::av::bd2d({ 4, 5 })));//-V112
  REQUIRE(s_img2.size() == 20);
  REQUIRE(s_img2.bound() == (poutre::details::av::bd2d({ 4, 5 })));//-V112
  // call UnaryOpDispatcher one idx
  poutre::details::t_ArithInvert(s_img1, s_img2);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("negate strided view one idx diff ptr type", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT32> img1(shape);
  const poutre::pINT32 value1 = 1;
  img1.fill(value1);
  poutre::details::image_t<poutre::pINT64> img2(shape);
  const poutre::pINT64 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 });
  auto s_img2 = v_img2.section({ 1, 1 });
  REQUIRE(s_img1.size() == 20);
  REQUIRE(s_img1.bound() == (poutre::details::av::bd2d({ 4, 5 })));//-V112
  REQUIRE(s_img2.size() == 20);
  REQUIRE(s_img2.bound() == (poutre::details::av::bd2d({ 4, 5 })));//-V112

  poutre::details::t_ArithInvert(s_img1, s_img2);
  const std::string expected =
    "Scalar GINT64 2 5 6 \
3 3 3 3 3 3 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("negate strided view one idx same ptr type", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT32> img1(shape);
  const poutre::pINT32 value1 = 1;
  img1.fill(value1);
  poutre::details::image_t<poutre::pINT32> img2(shape);
  const poutre::pINT32 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);

  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 }, { 2, 2 });
  auto s_img2 = v_img2.section({ 3, 3 }, { 2, 2 });
  // call UnaryOpDispatcher one idx
  poutre::details::t_ArithInvert(s_img1, s_img2);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 -1 -1 3 \
3 3 3 -1 -1 3";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("negate strided view two idx diff ptr type", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT32> img1(shape);
  const poutre::pINT32 value1 = 1;
  img1.fill(value1);
  poutre::details::image_t<poutre::pINT64> img2(shape);
  const poutre::pINT64 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 }, { 3, 2 });
  auto s_img2 = v_img2.section({ 1, 1 }, { 2, 3 });
  // call UnaryOpDispatcher primary template default two idx
  poutre::details::t_ArithInvert(s_img1, s_img2);

  const std::string expected =
    "Scalar GINT64 2 5 6 \
3 3 3 3 3 3 \
3 -1 -1 -1 3 3 \
3 -1 -1 -1 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sadd const same ptr type", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedAddConstant(img1, constant, img2);

  const std::string expected =
    "Scalar GUINT8 2 5 6 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255";

  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sadd const diff ptr type", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  // call UnaryOpDispatcher array view template specialization,fall back ptr
  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedAddConstant(img1, constant, img2);
  const std::string expected =
    "Scalar GUINT8 2 5 6 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255";

  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sadd const section same ptr type", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  // Extract section
  auto s_img1 = v_img1.section(/*poutre::idx2d*/ { 1, 1 });
  auto s_img2 = v_img2.section(/*poutre::idx2d*/ { 1, 1 });
  // call UnaryOpDispatcher one idx
  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedAddConstant(s_img1, constant, s_img2);
  const std::string expected =
    "Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 255 255 255 255 255 \
3 255 255 255 255 255 \
3 255 255 255 255 255 \
3 255 255 255 255 255";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sadd const section diff ptr type", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT32> img1(shape);
  const poutre::pINT32 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 });
  auto s_img2 = v_img2.section({ 1, 1 });

  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedAddConstant(s_img1, constant, s_img2);
  const std::string expected =
    "Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 255 255 255 255 255 \
3 255 255 255 255 255 \
3 255 255 255 255 255 \
3 255 255 255 255 255";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sadd const section diff ptr type compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);

  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 }, { 2, 2 });
  auto s_img2 = v_img2.section({ 3, 3 }, { 2, 2 });
  // call UnaryOpDispatcher one idx
  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedAddConstant(s_img1, constant, s_img2);

  const std::string expected =
    "Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 255 255 3 \
3 3 3 255 255 3";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sadd const section diff ptr type not compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT64> img1(shape);
  const poutre::pINT64 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 }, { 3, 2 });
  auto s_img2 = v_img2.section({ 1, 1 }, { 2, 3 });
  // call UnaryOpDispatcher primary template default two idx
  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedAddConstant(s_img1, constant, s_img2);

  const std::string expected =
    "Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 255 255 255 3 3 \
3 255 255 255 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssub const same ptr type compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 5;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedSubConstant(img1, constant, img2);

  const std::string expected =
    "Scalar GUINT8 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssub const diff ptr type", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 5;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  // call UnaryOpDispatcher array view template specialization,fall back ptr
  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedSubConstant(img1, constant, img2);

  const std::string expected =
    "Scalar GUINT8 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssub const same ptr type diff offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 5;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  // Extract section
  auto s_img1 = v_img1.section(/*poutre::idx2d*/ { 1, 1 });
  auto s_img2 = v_img2.section(/*poutre::idx2d*/ { 1, 1 });
  // call UnaryOpDispatcher one idx
  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedSubConstant(s_img1, constant, s_img2);
  const std::string expected =
    "Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 0 0 0 0 0 \
3 0 0 0 0 0 \
3 0 0 0 0 0 \
3 0 0 0 0 0";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssub const diff ptr type n compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT32> img1(shape);
  const poutre::pINT32 value = 5;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 });
  auto s_img2 = v_img2.section({ 1, 1 });

  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedSubConstant(s_img1, constant, s_img2);
  const std::string expected =
    "Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 0 0 0 0 0 \
3 0 0 0 0 0 \
3 0 0 0 0 0 \
3 0 0 0 0 0";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssub const same ptr type not compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 5;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);

  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 }, { 2, 2 });
  auto s_img2 = v_img2.section({ 3, 3 }, { 2, 2 });
  // call UnaryOpDispatcher one idx
  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedSubConstant(s_img1, constant, s_img2);

  const std::string expected =
    "Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 0 0 3 \
3 3 3 0 0 3";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssub const diff ptr type not compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT64> img1(shape);
  const poutre::pINT64 value = 5;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 3;
  img2.fill(value2);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 }, { 3, 2 });
  auto s_img2 = v_img2.section({ 1, 1 }, { 2, 3 });
  // call UnaryOpDispatcher primary template default two idx
  const auto constant = static_cast<poutre::pUINT8>(10);
  poutre::details::t_ArithSaturatedSubConstant(s_img1, constant, s_img2);

  const std::string expected =
    "Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 0 0 0 3 3 \
3 0 0 0 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3";
  const auto img_str = poutre::ImageToString(img2);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sup diff ptr type not compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT64> img1(shape);
  const poutre::pINT64 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 150;
  img2.fill(value2);
  poutre::details::image_t<poutre::pINT32> img3(shape);
  const poutre::pINT32 value3 = 3;
  img3.fill(value3);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  auto v_img3 = poutre::details::view(img3);

  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 }, { 3, 2 });
  auto s_img2 = v_img2.section({ 1, 1 }, { 2, 3 });
  auto s_img3 = v_img3.section({ 2, 2 }, { 2, 3 });

  poutre::details::t_ArithSup(s_img1, s_img2, s_img3);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 250 250 250 3 \
3 3 250 250 250 3 \
3 3 3 3 3 3";
  const auto img_str = poutre::ImageToString(img3);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sup same ptr type compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 255;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 150;
  img2.fill(value2);
  poutre::details::image_t<poutre::pUINT8> img3(shape);

  poutre::details::t_ArithSup(img1, img2, img3);

  const std::string expected =
    "Scalar GUINT8 2 5 6 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255";
  const auto img_str = poutre::ImageToString(img3);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("inf diff ptr type not compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT64> img1(shape);
  const poutre::pINT64 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 150;
  img2.fill(value2);
  poutre::details::image_t<poutre::pINT32> img3(shape);
  const poutre::pINT32 value3 = 3;
  img3.fill(value3);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  auto v_img3 = poutre::details::view(img3);

  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 }, { 3, 2 });
  auto s_img2 = v_img2.section({ 1, 1 }, { 2, 3 });
  auto s_img3 = v_img3.section({ 2, 2 }, { 2, 3 });

  poutre::details::t_ArithInf(s_img1, s_img2, s_img3);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 150 150 150 3 \
3 3 150 150 150 3 \
3 3 3 3 3 3";
  const auto img_str = poutre::ImageToString(img3);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("inf same ptr type compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT64> img1(shape);
  const poutre::pINT64 value = 255;
  img1.fill(value);
  poutre::details::image_t<poutre::pINT64> img2(shape);
  const poutre::pINT64 value2 = 150;
  img2.fill(value2);
  poutre::details::image_t<poutre::pINT64> img3(shape);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  auto v_img3 = poutre::details::view(img3);
  poutre::details::t_ArithInf(v_img1, v_img2, v_img3);

  const std::string expected =
    "Scalar GINT64 2 5 6 \
150 150 150 150 150 150 \
150 150 150 150 150 150 \
150 150 150 150 150 150 \
150 150 150 150 150 150 \
150 150 150 150 150 150";
  const auto img_str = poutre::ImageToString(img3);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sadd diff ptr type not compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT64> img1(shape);
  const poutre::pINT64 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 150;
  img2.fill(value2);
  poutre::details::image_t<poutre::pINT32> img3(shape);
  const poutre::pUINT8 value3 = 3;
  img3.fill(value3);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  auto v_img3 = poutre::details::view(img3);

  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 }, { 3, 2 });
  auto s_img2 = v_img2.section({ 1, 1 }, { 2, 3 });
  auto s_img3 = v_img3.section({ 2, 2 }, { 2, 3 });

  poutre::details::t_ArithSaturatedAdd(s_img1, s_img2, s_img3);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 400 400 400 3 \
3 3 400 400 400 3 \
3 3 3 3 3 3";
  const auto img_str = poutre::ImageToString(img3);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}


TEST_CASE("add same ptr type compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 150;
  img2.fill(value2);
  poutre::details::image_t<poutre::pUINT8> img3(shape);
  poutre::details::t_ArithSaturatedAdd(img1, img2, img3);

  const std::string expected =
    "Scalar GUINT8 2 5 6 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255";
  const auto img_str = poutre::ImageToString(img3);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssub diff ptr type not compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pINT64> img1(shape);
  const poutre::pINT64 value = 250;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 150;
  img2.fill(value2);
  poutre::details::image_t<poutre::pINT32> img3(shape);
  const poutre::pUINT8 value3 = 3;
  img3.fill(value3);
  auto v_img1 = poutre::details::view(img1);
  auto v_img2 = poutre::details::view(img2);
  auto v_img3 = poutre::details::view(img3);

  // Extract section
  auto s_img1 = v_img1.section({ 1, 1 }, { 3, 2 });
  auto s_img2 = v_img2.section({ 1, 1 }, { 2, 3 });
  auto s_img3 = v_img3.section({ 2, 2 }, { 2, 3 });

  poutre::details::t_ArithSaturatedSub(s_img1, s_img2, s_img3);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 100 100 100 3 \
3 3 100 100 100 3 \
3 3 3 3 3 3";
  const auto img_str = poutre::ImageToString(img3);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssub same ptr type compatible offset", "[arith]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  poutre::details::image_t<poutre::pUINT8> img1(shape);
  const poutre::pUINT8 value = 150;
  img1.fill(value);
  poutre::details::image_t<poutre::pUINT8> img2(shape);
  const poutre::pUINT8 value2 = 250;
  img2.fill(value2);
  poutre::details::image_t<poutre::pUINT8> img3(shape);
  poutre::details::t_ArithSaturatedSub(img1, img2, img3);

  const std::string expected =
    "Scalar GUINT8 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0";
  const auto img_str = poutre::ImageToString(img3);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}