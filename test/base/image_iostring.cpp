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
#include <catch2/matchers/catch_matchers_range_equals.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <cstddef>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>
#include <poutre/base/types_traits.hpp>
#include <string>
#include <vector>


TEST_CASE("read_scalar_u8_2d", "[iostring]")
{
  //*************************//
  // ImageType CompoundType Type NumDims pixvalue pixvalue .......
  const std::string img_str =
    "Scalar GUINT8 2 3 4\
   1 2 3 4\
   10 12 13 14\
   20 22 23 24\
  ";
  auto img = poutre::ImageFromString(img_str);
  REQUIRE(img->GetPType() == poutre::PType::PType_GrayUINT8);
  REQUIRE(img->GetCType() == poutre::CompoundType::CompoundType_Scalar);
  REQUIRE(img->GetRank() == 2);
  auto dims = img->GetShape();
  const std::vector<size_t> expectedcoords = { 3, 4 };
  REQUIRE_THAT(dims, Catch::Matchers::Equals(expectedcoords));

  const std::vector<poutre::pUINT8> expectedvalues = { 1, 2, 3, 4, 10, 12, 13, 14, 20, 22, 23, 24 };
  auto *img_t = dynamic_cast<poutre::details::image_t<poutre::pUINT8, 2> *>(&(*img));
  REQUIRE(img_t != nullptr);
  //   std::cout << "\n**************************************************\n";
  //   for (const auto &val : (*img_t)) {
  //     std::cout << "\nval\t"
  //               << (poutre::TypeTraits<poutre::pUINT8>::str_type)(val);
  //   }
  //   std::cout << "\n**************************************************\n";
  //   abort();
  // EXPECT_TRUE(CheckEqualCollections((*img_t).begin(), (*img_t).end(), expectedvalues.begin()));//-V522
  REQUIRE_THAT((*img_t), Catch::Matchers::RangeEquals(expectedvalues));
}

TEST_CASE("read_scalar_i32_2d", "[iostring]")
{
  //*************************//
  // ImageType CompoundType Type NumDims pixvalue pixvalue .......
  const std::string img_str =
    "Scalar GINT32 2 3 4\
   -1 2 3 4\
   10 12 13 14\
   20 22 23 2400\
  ";
  auto img = poutre::ImageFromString(img_str);
  REQUIRE(img->GetPType() == poutre::PType::PType_GrayINT32);
  REQUIRE(img->GetCType() == poutre::CompoundType::CompoundType_Scalar);
  REQUIRE(img->GetRank() == 2);
  auto dims = img->GetShape();
  const std::vector<size_t> expectedcoords = { 3, 4 };
  REQUIRE_THAT(dims, Catch::Matchers::Equals(expectedcoords));

  const std::vector<poutre::pINT32> expectedvalues = { -1, 2, 3, 4, 10, 12, 13, 14, 20, 22, 23, 2400 };
  auto *img_t = dynamic_cast<poutre::details::image_t<poutre::pINT32, 2> *>(&(*img));
  REQUIRE(img_t != nullptr);
  //    for(const auto& val : (*img_t))
  //        {
  //        std::cout<<"\nval\t"<<(poutre::TypeTraits<poutre::pUINT8>::str_type)(val);
  //        }
  REQUIRE_THAT((*img_t), Catch::Matchers::RangeEquals(expectedvalues));
}

TEST_CASE("read_scalar_3planes_u8_2d", "[iostring]")
{// ImageType CompoundType Type NumDims pixvalue pixvalue .......
  const std::string img_str =
    "3Planes GUINT8 2 3 4\
   1 1 1 2 2 2 3 3 3 4 4 4\
   10 10 10 12 12 12 13 13 13 14 14 14\
   20 20 20 22 22 22 23 23 23 24 24 24\
  ";
  auto img = poutre::ImageFromString(img_str);
  REQUIRE(img->GetPType() == poutre::PType::PType_GrayUINT8);
  REQUIRE(img->GetCType() == poutre::CompoundType::CompoundType_3Planes);
  REQUIRE(img->GetRank() == 2);
  auto dims = img->GetShape();
  const std::vector<size_t> expectedcoords = { 3, 4 };
  REQUIRE_THAT(dims, Catch::Matchers::Equals(expectedcoords));

  // NOLINTBEGIN
  const std::vector<poutre::compound_type<poutre::pUINT8, 3>> expectedvalues = { { 1, 1, 1 },
    { 2, 2, 2 },
    { 3, 3, 3 },
    { 4, 4, 4 },
    { 10, 10, 10 },
    { 12, 12, 12 },
    { 13, 13, 13 },
    { 14, 14, 14 },
    { 20, 20, 20 },
    { 22, 22, 22 },
    { 23, 23, 23 },
    { 24, 24, 24 } };
  // NOLINTEND
  using ImgType =
    poutre::details::image_t<typename poutre::TypeTraits<poutre::compound_type<poutre::pUINT8, 3>>::storage_type, 2>;
  auto *img_t = dynamic_cast<ImgType *>(&(*img));
  REQUIRE(img_t != nullptr);
  //    for(const auto& pix : (*img_t))
  //        {
  //          std::cout<<"\n";
  //          for(const auto& val : pix)
  //          {
  //              std::cout<<"
  //              "<<(poutre::TypeTraits<poutre::pUINT8>::str_type)(val);
  //          }
  //        }

  REQUIRE_THAT((*img_t), Catch::Matchers::RangeEquals(expectedvalues));
}

TEST_CASE("read_scalar_4planes_d64_2d", "[iostring]")
{// ImageType CompoundType Type NumDims pixvalue pixvalue .......
  const std::string img_str =
    "4Planes D64 2 3 4\
   1 1 1 1 2 2 2 2 3 3 3 3 4 4 4 4\
   10 10 10 10 12 12 12 12 13 13 13 13 14 14 14 14\
   20 20 20 20 22 22 22 22 23 23 23 23 24 24 24 24\
  ";
  auto img = poutre::ImageFromString(img_str);
  REQUIRE(img->GetPType() == poutre::PType::PType_D64);
  REQUIRE(img->GetCType() == poutre::CompoundType::CompoundType_4Planes);
  REQUIRE(img->GetRank() == 2);
  auto dims = img->GetShape();
  const std::vector<size_t> expectedcoords = { 3, 4 };
  REQUIRE_THAT(dims, Catch::Matchers::Equals(expectedcoords));

  // NOLINTBEGIN
  std::vector<poutre::compound_type<poutre::pDOUBLE, 4>> expectedvalues = { { 1, 1, 1, 1 },
    { 2, 2, 2, 2 },
    { 3, 3, 3, 3 },
    { 4, 4, 4, 4 },
    { 10, 10, 10, 10 },
    { 12, 12, 12, 12 },
    { 13, 13, 13, 13 },
    { 14, 14, 14, 14 },
    { 20, 20, 20, 20 },
    { 22, 22, 22, 22 },
    { 23, 23, 23, 23 },
    { 24, 24, 24, 24 } };
  // NOLINTEND
  using ImgType =
    /*typename*/ poutre::details::
      image_t<typename poutre::TypeTraits<poutre::compound_type<poutre::pDOUBLE, 4>>::storage_type, 2>;
  auto *img_t = dynamic_cast<ImgType *>(&(*img));
  REQUIRE(img_t != nullptr);
  REQUIRE_THAT((*img_t), Catch::Matchers::RangeEquals(expectedvalues));
}

TEST_CASE("write_scalar_u8_2d", "[iostring]")
{
  poutre::details::image_t<poutre::pUINT8> img({ 3, 4 });//-V112
  const poutre::pUINT8 value = 10;
  img.fill(value);
  REQUIRE(img.GetPType() == poutre::PType::PType_GrayUINT8);
  REQUIRE(img.GetCType() == poutre::CompoundType::CompoundType_Scalar);
  REQUIRE(img.GetRank() == 2);

  auto img_str = poutre::ImageToString(img);
  const std::string expected =
    "Scalar GUINT8 2 3 4 \
10 10 10 10 \
10 10 10 10 \
10 10 10 10";
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("write_scalar_f32_2d", "[iostring]")
{
  poutre::details::image_t<poutre::pFLOAT> img({ 3, 4 });//-V112
  img.fill(-5.);// NOLINT
  REQUIRE(img.GetPType() == poutre::PType::PType_F32);
  REQUIRE(img.GetCType() == poutre::CompoundType::CompoundType_Scalar);
  REQUIRE(img.GetRank() == 2);

  auto img_str = poutre::ImageToString(img);
  const std::string expected =
    "Scalar F32 2 3 4 \
-5 -5 -5 -5 \
-5 -5 -5 -5 \
-5 -5 -5 -5";
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("write_3planes_u8_2d", "[iostring]")
{
  poutre::details::image_t<poutre::compound_type<poutre::pUINT8, 3>> img({ 3, 4 });//-V112

  img.fill({ 10, 11, 12 });// NOLINT

  REQUIRE(img.GetPType() == poutre::PType::PType_GrayUINT8);
  REQUIRE(img.GetCType() == poutre::CompoundType::CompoundType_3Planes);
  REQUIRE(img.size() == 12);

  auto img_str = poutre::ImageToString(img);
  const std::string expected =
    "3Planes GUINT8 2 3 4 \
10 11 12 10 11 12 10 11 12 10 11 12 \
10 11 12 10 11 12 10 11 12 10 11 12 \
10 11 12 10 11 12 10 11 12 10 11 12";
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("write_3planes_d64_2d", "[iostring]")
{
  poutre::details::image_t<poutre::compound_type<poutre::pDOUBLE, 3>> img({ 3, 4 });//-V112
  img.fill({ 10., -11., 1.2 });// NOLINT
  REQUIRE(img.GetPType() == poutre::PType::PType_D64);
  REQUIRE(img.GetCType() == poutre::CompoundType::CompoundType_3Planes);
  REQUIRE(img.size() == 12);
  REQUIRE(img.GetRank() == 2);

  auto img_str = poutre::ImageToString(img);
  const std::string expected =
    "3Planes D64 2 3 4 \
10 -11 1.2 10 -11 1.2 10 -11 1.2 10 -11 1.2 \
10 -11 1.2 10 -11 1.2 10 -11 1.2 10 -11 1.2 \
10 -11 1.2 10 -11 1.2 10 -11 1.2 10 -11 1.2";
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}