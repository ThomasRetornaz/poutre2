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
// #include <catch2/matchers/catch_matchers_container_properties.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <cstddef>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>
#include <stdexcept>
#include <utility>
#include <vector>

TEST_CASE("ctor", "[image]")
{
  const poutre::details::image_t<poutre::pUINT8> img({ 3, 4 });//-V112
  //   REQUIRE(img.GetPType() == poutre::PType::PType_GrayUINT8);
  //   REQUIRE(img.GetCType() == poutre::CompoundType::CompoundType_Scalar);
  REQUIRE(img.GetRank() == 2);
  auto coords = img.GetShape();
  const std::vector<std::size_t> expectedcoords = { 3, 4 };//-V112
  REQUIRE_THAT(coords, Catch::Matchers::Equals(expectedcoords));

  /*std::cout << "**************************************************" <<
  std::endl; std::cout << img << std::endl; std::cout <<
  "**************************************************" << std::endl;*/
}

TEST_CASE("factory", "[image]")
{
  {
    const auto img = poutre::Create({ 3, 4 },
      poutre::CompoundType::CompoundType_Scalar,//-V112
      poutre::PType::PType_GrayUINT8);//-V112
    REQUIRE(img != nullptr);
    REQUIRE((*img).GetCType() == poutre::CompoundType::CompoundType_Scalar);
    REQUIRE((*img).GetPType() == poutre::PType::PType_GrayUINT8);
    const auto coords = (*img).GetShape();
    const std::vector<std::size_t> expectedcoords = { 3, 4 };//-V112
    REQUIRE_THAT(coords, Catch::Matchers::Equals(expectedcoords));
    REQUIRE(dynamic_cast<poutre::details::image_t<poutre::pUINT8> *>(img.get()));
  }

  {
    const auto img = poutre::Create({ 3, 4 },
      poutre::CompoundType::CompoundType_Scalar,//-V112
      poutre::PType::PType_GrayINT32);//-V112
    REQUIRE(img != nullptr);
    REQUIRE((*img).GetCType() == poutre::CompoundType::CompoundType_Scalar);
    REQUIRE((*img).GetPType() == poutre::PType::PType_GrayINT32);
    const auto coords = (*img).GetShape();
    const std::vector<std::size_t> expectedcoords = { 3, 4 };//-V112
    REQUIRE_THAT(coords, Catch::Matchers::Equals(expectedcoords));
    REQUIRE(dynamic_cast<poutre::details::image_t<poutre::pINT32> *>(img.get()));
  }
  {
    const auto img = poutre::Create({ 3, 4 },
      poutre::CompoundType::CompoundType_Scalar,//-V112
      poutre::PType::PType_F32);//-V112
    REQUIRE(img != nullptr);
    REQUIRE((*img).GetCType() == poutre::CompoundType::CompoundType_Scalar);
    REQUIRE((*img).GetPType() == poutre::PType::PType_F32);
    const auto coords = (*img).GetShape();
    const std::vector<std::size_t> expectedcoords = { 3, 4 };//-V112
    REQUIRE_THAT(coords, Catch::Matchers::Equals(expectedcoords));
    REQUIRE(dynamic_cast<poutre::details::image_t<poutre::pFLOAT> *>(img.get()));
  }
  {
    const auto img = poutre::Create({ 3, 4 },
      poutre::CompoundType::CompoundType_Scalar,//-V112
      poutre::PType::PType_GrayINT64);//-V112
    REQUIRE(img != nullptr);
    REQUIRE((*img).GetPType() == poutre::PType::PType_GrayINT64);
    REQUIRE((*img).GetCType() == poutre::CompoundType::CompoundType_Scalar);
    const auto coords = (*img).GetShape();
    const std::vector<std::size_t> expectedcoords = { 3, 4 };//-V112
    REQUIRE_THAT(coords, Catch::Matchers::Equals(expectedcoords));
    REQUIRE(dynamic_cast<poutre::details::image_t<poutre::pINT64> *>(img.get()));
  }
  {
    REQUIRE_THROWS_AS(poutre::Create({ 3, 4 },
                        poutre::CompoundType::CompoundType_Scalar,//-V112
                        poutre::PType::PType_Undef),
      std::runtime_error);

    REQUIRE_THROWS_AS(poutre::Create({ 3, 4 },
                        poutre::CompoundType::CompoundType_Undef,//-V112
                        poutre::PType::PType_GrayINT64),
      std::runtime_error);
  }
}

TEST_CASE("basic_iteration", "[image]")
{
  poutre::details::image_t<poutre::pUINT8> img({ 3, 4 });//-V112
  //   REQUIRE(img.GetPType() == poutre::PType::PType_GrayUINT8);
  //   REQUIRE(img.GetCType() == poutre::CompoundType::CompoundType_Scalar);
  const poutre::pUINT8 constant = 10;
  const std::size_t size_image = img.size();
  img.fill(constant);

  std::size_t count = 0;
  for (const auto var : img)
  {
    REQUIRE(var == constant);
    ++count;
  }
  REQUIRE(size_image == count);
  // count = 0;
  // auto rit = img.rbegin();
  // auto rend = img.rend();
  // for (; rit != rend; ++rit, ++count) {
  //   EXPECT_EQ(*rit, 10);
  // }
  // EXPECT_EQ(count, 12);

  // count = 0;
  // auto crit = img.crbegin();
  // auto crend = img.crend();
  // for (; crit != crend; ++crit, ++count) {
  //   EXPECT_EQ(*crit, 10);
  // }
  // EXPECT_EQ(count, 12);

  // for (size_t i = 0u; i < img.size(); i++) {
  //   EXPECT_EQ(img[i], 10);
  //   EXPECT_EQ(img.at(i), 10);
  // }
  // EXPECT_EQ(img.front(), 10);
  // EXPECT_EQ(img.back(), 10);
}

TEST_CASE("move", "[image]")
{
  // todo check assembly
  auto img(poutre::details::image_t<poutre::pUINT8>{ 3, 4 });//-V112

  //   REQUIRE(img.GetPType() == poutre::PType::PType_GrayUINT8);
  //   REQUIRE(img.GetCType() == poutre::CompoundType::CompoundType_Scalar);

  const auto coords = img.GetShape();
  const std::vector<std::size_t> expectedcoords = { 3, 4 };//-V112
  REQUIRE_THAT(coords, Catch::Matchers::Equals(expectedcoords));

  poutre::details::image_t<poutre::pUINT8> img2({ 2, 3 });
  //   REQUIRE(img2.GetPType() == poutre::PType::PType_GrayUINT8);
  //   REQUIRE(img2.GetCType() == poutre::CompoundType::CompoundType_Scalar);
  const auto coords2 = img2.GetShape();
  const std::vector<std::size_t> expectedcoords2 = { 2, 3 };
  REQUIRE_THAT(coords2, Catch::Matchers::Equals(expectedcoords2));

  // move
  img2 = std::move(img);
  REQUIRE(img2.GetPType() == poutre::PType::PType_GrayUINT8);
  REQUIRE(img2.GetCType() == poutre::CompoundType::CompoundType_Scalar);
  const auto coordsmove = img2.GetShape();
  const std::vector<std::size_t> expectedcoordsmove = { 3, 4 };//-V112
  REQUIRE_THAT(coordsmove, Catch::Matchers::Equals(expectedcoordsmove));
}

TEST_CASE("default arry view", "[image]")
{
  poutre::details::image_t<poutre::pUINT8> img1({ 3, 4 });//-V112
  // start view
  const auto vimg1 = poutre::details::view(img1);
  REQUIRE(vimg1.size() == 12);
  const poutre::details::av::bd2d bnd = { 3, 4 };
  const poutre::details::av::idx2d idx = { 4, 1 };
  REQUIRE(vimg1.bound() == bnd);//-V112
  REQUIRE(vimg1.stride() == idx);//-V112
}

TEST_CASE("2D set/get pixel", "[image]")
{
  using ImageType = poutre::details::image_t<poutre::pINT32>;
  auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
   1 1 1 1 1 1\
   1 1 1 1 1 1\
   1 1 1 1 1 1\
   1 1 1 5 5 5\
   1 1 1 5 5 5\
  ");
  auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img);
  const poutre::pINT32 pix_value1 = 10;
  (*img).SetPixel(2, 0, pix_value1);// x then y //-V522
  const poutre::details::av::idx2d pix = { 3, 0 };// y then x //-V522
  const poutre::pINT32 pix_value2 = 8;
  (*img).SetPixel(pix, pix_value2);
  const poutre::details::av::idx2d pix2 = { 5, 3 };// x then y
  const poutre::pINT32 pix_value3 = 9;
  (*img).SetPixel(pix2, pix_value3);

  //  std::string expected = "Scalar GINT32 2 5 6\
  // 1 1 10 1 1 1\
  // 1 1 1 1 1 1\
  // 1 1 1 1 1 1\
  // 8 1 1 5 5 9\
  // 1 1 1 5 5 5\
  //";
  auto val = (*img).GetPixel(2, 0);// x then y
  REQUIRE(val == pix_value1);
  auto val2 = (*img).GetPixel(pix);
  REQUIRE(val2 == pix_value2);
  auto val3 = (*img).GetPixel(pix2);
  REQUIRE(val3 == pix_value3);
}

TEST_CASE("2D GetLineBuffer", "[image]")
{
  using ImageType = poutre::details::image_t<poutre::pINT32>;
  auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
   1 1 1 1 1 1\
   1 1 1 1 1 1\
   1 1 1 1 1 1\
   1 1 1 5 5 5\
   1 1 1 5 5 5\
  ");
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img);
  // const auto* line = (*img).GetLineBuffer(3);//-V522
  // REQUIRE(line[4] == 5);//-V522

  REQUIRE((*img).GetXSize() == 6);
  REQUIRE((*img).GetYSize() == 5);
}
