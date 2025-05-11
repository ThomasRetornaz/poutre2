// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //Z
//==============================================================================

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
// #include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>
// #include <poutre/base/types_traits.hpp>
#include <cstddef>
#include <poutre/pixel_processing/details/compare_op_t.hpp>
#include <string>
#include <vector>



TEST_CASE("sss whole image equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  poutre::details::t_CompareImage_sss(*img, poutre::CompOpType::CompOpEqual, 1, 1, 0, imgout); //NOLINT

  const std::string expected =
    "Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 0 0 0 \
1 1 1 0 0 0\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sss whole image not equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  poutre::details::t_CompareImage_sss(*img, poutre::CompOpType::CompOpDiff, 1, 1, 0, imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 1 1 1 \
0 0 0 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sss whole image sup", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  poutre::details::t_CompareImage_sss(*img, poutre::CompOpType::CompOpSup, 1, 1, 0, imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 1 1 1 \
0 0 0 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sss whole image sup equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  poutre::details::t_CompareImage_sss(*img, poutre::CompOpType::CompOpSupEqual, 1, 1, 0, imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sss whole image inf", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  const poutre::pINT32 valcomp = 5;
  const poutre::pINT32 valtrue = 1;
  const poutre::pINT32 valfalse = 0;
  poutre::details::t_CompareImage_sss(*img, poutre::CompOpType::CompOpInf,valcomp, valtrue,valfalse, imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 0 0 0 \
1 1 1 0 0 0\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sss whole image inf equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  poutre::details::t_CompareImage_sss(*img, poutre::CompOpType::CompOpInfEqual, 5, 1, 0, imgout); //NOLINT

  const std::string expected =
    "Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sss section image equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 5 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img);
  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  // Extract section
  auto v_imgin = poutre::details::view(*img);//-V522
  auto v_imgout = poutre::details::view(imgout);

  auto s_img = v_imgin.section({ 1, 1 }, { 3, 2 });
  auto s_imgout = v_imgout.section({ 1, 1 }, { 3, 2 });

  poutre::details::ViewCompare_sss_func_helper(s_img, poutre::CompOpType::CompOpEqual, 1, 1, 0, s_imgout); //NOLINT

  const std::string expected =
    "Scalar GINT32 2 5 6 \
4 4 4 4 4 4 \
4 1 1 4 4 4 \
4 1 0 4 4 4 \
4 1 1 4 4 4 \
4 4 4 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iss whole image equal ", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img);
  auto v_img1 = poutre::details::view(*img);//-V522

  const auto imgin2 = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
  const auto *img2 = dynamic_cast<ImageType *>(imgin2.get());
  REQUIRE(img2);
  auto v_img2 = poutre::details::view(*img2);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iss_func_helper(v_img1, poutre::CompOpType::CompOpEqual, v_img2, 1, 0, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 0 0 0 \
1 1 1 0 0 0\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iss whole image not equal ", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img);
  auto v_img1 = poutre::details::view(*img);//-V522

  const auto imgin2 = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
  const auto *img2 = dynamic_cast<ImageType *>(imgin2.get());
  REQUIRE(img2);
  auto v_img2 = poutre::details::view(*img2);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iss_func_helper(v_img1, poutre::CompOpType::CompOpDiff, v_img2, 1, 0, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 1 1 1 \
0 0 0 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}


TEST_CASE("iss whole image sup ", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img);
  auto v_img1 = poutre::details::view(*img);//-V522

  const auto imgin2 = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
  const auto *img2 = dynamic_cast<ImageType *>(imgin2.get());
  REQUIRE(img2);
  auto v_img2 = poutre::details::view(*img2);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iss_func_helper(v_img1, poutre::CompOpType::CompOpSup, v_img2, 1, 0, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 1 1 1 \
0 0 0 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iss whole image sup equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img);
  auto v_img1 = poutre::details::view(*img);//-V522

  const auto imgin2 = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
  const auto *img2 = dynamic_cast<ImageType *>(imgin2.get());
  REQUIRE(img2);
  auto v_img2 = poutre::details::view(*img2);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iss_func_helper(v_img1, poutre::CompOpType::CompOpSupEqual, v_img2, 1, 0, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iss whole image inf ", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img);
  auto v_img1 = poutre::details::view(*img);//-V522

  const auto imgin2 = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
  const auto *img2 = dynamic_cast<ImageType *>(imgin2.get());
  REQUIRE(img2);
  auto v_img2 = poutre::details::view(*img2);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iss_func_helper(v_img1, poutre::CompOpType::CompOpInf, v_img2, 1, 0, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iss whole image inf equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img);
  auto v_img1 = poutre::details::view(*img);//-V522

  const auto imgin2 = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
  const auto *img2 = dynamic_cast<ImageType *>(imgin2.get());
  REQUIRE(img2);
  auto v_img2 = poutre::details::view(*img2);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iss_func_helper(v_img1, poutre::CompOpType::CompOpInfEqual, v_img2, 1, 0, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 0 0 0 \
1 1 1 0 0 0\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}


TEST_CASE("sis whole image equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sis_func_helper(v_img,
    poutre::CompOpType::CompOpEqual,
    static_cast<poutre::pINT32>(1), //NOLINT
    v_imgtrue,
    static_cast<poutre::pINT32>(0), //NOLINT
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 0 0 0 \
2 2 2 0 0 0\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sis whole image not equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sis_func_helper(v_img,
    poutre::CompOpType::CompOpDiff,
    static_cast<poutre::pINT32>(1),
    v_imgtrue,
    static_cast<poutre::pINT32>(0),
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 4 4 4 \
0 0 0 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sis whole image sup", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sis_func_helper(v_img,
    poutre::CompOpType::CompOpSup,
    static_cast<poutre::pINT32>(1),
    v_imgtrue,
    static_cast<poutre::pINT32>(0),
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 4 4 4 \
0 0 0 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sis whole image sup equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sis_func_helper(v_img,
    poutre::CompOpType::CompOpSupEqual,
    static_cast<poutre::pINT32>(1),
    v_imgtrue,
    static_cast<poutre::pINT32>(0),
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 4 4 4 \
2 2 2 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sis whole image inf", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sis_func_helper(v_img,
    poutre::CompOpType::CompOpInf,
    static_cast<poutre::pINT32>(1),
    v_imgtrue,
    static_cast<poutre::pINT32>(0),
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sis whole image inf equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sis_func_helper(v_img,
    poutre::CompOpType::CompOpInfEqual,
    static_cast<poutre::pINT32>(1),
    v_imgtrue,
    static_cast<poutre::pINT32>(0),
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 0 0 0 \
2 2 2 0 0 0\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}


TEST_CASE("ssi whole image equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_ssi_func_helper(v_img,
    poutre::CompOpType::CompOpEqual,
    static_cast<poutre::pINT32>(5), //NOLINT
    static_cast<poutre::pINT32>(1), //NOLINT
    v_imgfalse,
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 1 1 1 \
2 2 2 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssi whole image not equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_ssi_func_helper(v_img,
    poutre::CompOpType::CompOpDiff,
    static_cast<poutre::pINT32>(5), //NOLINT
    static_cast<poutre::pINT32>(1), //NOLINT
    v_imgfalse,
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 4 4 4 \
1 1 1 4 4 4\
";

  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssi whole image sup", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_ssi_func_helper(v_img,
    poutre::CompOpType::CompOpSup,
    static_cast<poutre::pINT32>(5), //NOLINT
    static_cast<poutre::pINT32>(1), //NOLINT
    v_imgfalse,
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("ssi whole image sup equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_ssi_func_helper(v_img,
    poutre::CompOpType::CompOpSupEqual,
    static_cast<poutre::pINT32>(5), //NOLINT
    static_cast<poutre::pINT32>(1), //NOLINT
    v_imgfalse,
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 1 1 1\
 2 2 2 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}


TEST_CASE("ssi whole image inf", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_ssi_func_helper(v_img,
    poutre::CompOpType::CompOpInf,
    static_cast<poutre::pINT32>(5), //NOLINT
    static_cast<poutre::pINT32>(1), //NOLINT
    v_imgfalse,
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}


TEST_CASE("ssi whole image inf equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  const poutre::pINT32 valcomp = 5;
  const poutre::pINT32 valtrue = 1;

  poutre::details::ViewCompare_ssi_func_helper(v_img,
    poutre::CompOpType::CompOpInfEqual,
    valcomp,
    valtrue,
    v_imgfalse,
    v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}


TEST_CASE("sii whole image equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  const poutre::pINT32 valcomp = 5;
  poutre::details::ViewCompare_sii_dispatch(
    v_img, poutre::CompOpType::CompOpEqual, valcomp, v_imgtrue, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sii whole image not equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sii_dispatch(
    v_img, poutre::CompOpType::CompOpDiff, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout); //NOLINT

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sii whole image sup", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sii_dispatch(
    v_img, poutre::CompOpType::CompOpSup, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout); //NOLINT

  const std::string expected =
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sii whole image sup equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sii_dispatch(
    v_img, poutre::CompOpType::CompOpSupEqual, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout); //NOLINT

  const std::string expected =
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("sii whole image inf", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sii_dispatch(
    v_img, poutre::CompOpType::CompOpInf, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout); //NOLINT

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}


TEST_CASE("sii whole image inf equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_sii_dispatch(
    v_img, poutre::CompOpType::CompOpInfEqual, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout); //NOLINT

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iii whole image equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iii_dispatch(
    v_img, poutre::CompOpType::CompOpEqual, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iii whole image not equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);

  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iii_dispatch(
    v_img, poutre::CompOpType::CompOpDiff, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
TEST_CASE("iii whole image sup", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iii_dispatch(
    v_img, poutre::CompOpType::CompOpSup, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iii whole image sup equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iii_dispatch(
    v_img, poutre::CompOpType::CompOpSupEqual, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iii whole image inf", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iii_dispatch(
    v_img, poutre::CompOpType::CompOpInf, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iii whole image inf equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522


  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iii_dispatch(
    v_img, poutre::CompOpType::CompOpInfEqual, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iis whole image equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iis_dispatch(
    v_img, poutre::CompOpType::CompOpEqual, v_imgcomp, v_imgtrue, 4, v_imgout);//-V112

  const std::string expected =
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("iis whole image not equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  ;
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iis_dispatch(
    v_img, poutre::CompOpType::CompOpDiff, v_imgcomp, v_imgtrue, 4, v_imgout);//-V112

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
TEST_CASE("iis whole image sup", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iis_dispatch(
    v_img, poutre::CompOpType::CompOpSup, v_imgcomp, v_imgtrue, 4, v_imgout);//-V112

  const std::string expected =
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
TEST_CASE("iis whole image sup equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iis_dispatch(
    v_img, poutre::CompOpType::CompOpSupEqual, v_imgcomp, v_imgtrue, 4, v_imgout);//-V112

  const std::string expected =
    "Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
TEST_CASE("iis whole image inf", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iis_dispatch(
    v_img, poutre::CompOpType::CompOpInf, v_imgcomp, v_imgtrue, 4, v_imgout);//-V112

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
TEST_CASE("iis whole image inf equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgtrue = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgt = dynamic_cast<ImageType *>(imgtrue.get());
  REQUIRE(imgt != nullptr);
  auto v_imgtrue = poutre::details::view(*imgt);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_iis_dispatch(
    v_img, poutre::CompOpType::CompOpInfEqual, v_imgcomp, v_imgtrue, 4, v_imgout);//-V112

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("isi whole image equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_isi_dispatch(v_img, poutre::CompOpType::CompOpEqual, v_imgcomp, 1, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 1 1 1\
 2 2 2 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("isi whole image not equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_isi_dispatch(v_img, poutre::CompOpType::CompOpDiff, v_imgcomp, 1, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 2 2 2\
 1 1 1 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
TEST_CASE("isi whole image sup", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_isi_dispatch(v_img, poutre::CompOpType::CompOpSup, v_imgcomp, 1, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
TEST_CASE("isi whole image sup equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };
  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_isi_dispatch(
    v_img, poutre::CompOpType::CompOpSupEqual, v_imgcomp, 1, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 1 1 1\
 2 2 2 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("isi whole image inf", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_isi_dispatch(v_img, poutre::CompOpType::CompOpInf, v_imgcomp, 1, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 2 2 2\
 1 1 1 2 2 2\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("isi whole image inf equal", "[compare]")
{
  const std::vector<std::size_t> shape = { 5, 6 };

  const auto imgin = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  using ImageType = const poutre::details::image_t<poutre::pINT32>;
  const auto *img = dynamic_cast<ImageType *>(imgin.get());
  REQUIRE(img != nullptr);
  auto v_img = poutre::details::view(*img);//-V522

  const auto imgcomp = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
  const auto *imgc = dynamic_cast<ImageType *>(imgcomp.get());
  REQUIRE(imgc != nullptr);
  auto v_imgcomp = poutre::details::view(*imgc);//-V522

  const auto imgfalse = poutre::ImageFromString(
    "Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
  const auto *imgf = dynamic_cast<ImageType *>(imgfalse.get());
  REQUIRE(imgf != nullptr);
  auto v_imgfalse = poutre::details::view(*imgf);//-V522

  poutre::details::image_t<poutre::pINT32> imgout(shape);
  const poutre::pINT32 constant = 4;
  imgout.fill(constant);
  auto v_imgout = poutre::details::view(imgout);

  poutre::details::ViewCompare_isi_dispatch(
    v_img, poutre::CompOpType::CompOpInfEqual, v_imgcomp, 1, v_imgfalse, v_imgout);

  const std::string expected =
    "Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
";
  const auto img_str = poutre::ImageToString(imgout);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
