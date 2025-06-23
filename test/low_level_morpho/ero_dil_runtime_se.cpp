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
//#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>
#include <cstddef>
//#include <poutre/structuring_element/details/neighbor_list_static_se_t.hpp>
#include <poutre/structuring_element/predefined_nl_se.hpp>
#include <poutre/low_level_morpho/details/ero_dil_runtime_nl_se_t.hpp>
#include <string>
#include <vector>

TEST_CASE("dilate segmentX1D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 5 };
  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 1 5
 0 5 6 5 0
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,1>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,1> img_out(shape);
  poutre::llm::details::t_Dilate(*img, poutre::se::SESegmentX1D, img_out);
  const std::string expected =
    "Scalar GINT64 1 5\
 5 6 6 6 5";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("erode segmentX1D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 5 };
  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 1 5
 0 5 6 5 0
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,1>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,1> img_out(shape);
  poutre::llm::details::t_Erode(*img, poutre::se::SESegmentX1D, img_out);
  const std::string expected =
    "Scalar GINT64 1 5\
 0 0 5 0 0";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("dilate square2D runtime", "[low_level_morpho]")
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
  poutre::llm::details::t_Dilate(*img, poutre::se::SESquare2D, img_out);

  const std::string expected =
    "Scalar GINT64 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("erode square2D runtime", "[low_level_morpho]")
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
  poutre::llm::details::t_Erode(*img, poutre::se::SESquare2D, img_out);

  const std::string expected =
    "Scalar GINT64 2 5 5\
 0 0 0 0 0\
 0 0 0 0 0\
 0 0 5 0 0\
 0 0 0 0 0\
 0 0 0 0 0";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("dilate cross2D runtime", "[low_level_morpho]")
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
  poutre::llm::details::t_Dilate(*img, poutre::se::SECross2D, img_out);

  const std::string expected =
    "Scalar GINT64 2 5 5\
 8 8 5 5 0\
 8 5 6 5 5\
 5 6 6 6 5\
 5 5 6 5 5\
 0 5 5 5 0";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("erode cross2D runtime", "[low_level_morpho]")
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
  poutre::llm::details::t_Erode(*img, poutre::se::SECross2D, img_out);

  const std::string expected =
    "Scalar GINT64 2 5 5\
 0 0 0 0 0\
 0 0 0 0 0\
 0 0 5 0 0\
 0 0 0 0 0\
 0 0 0 0 0";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("dilate segmentX2D runtime", "[low_level_morpho]")
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
  poutre::llm::details::t_Dilate(*img, poutre::se::SESegmentX2D, img_out);

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

TEST_CASE("erode segmentX2D runtime", "[low_level_morpho]")
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
  poutre::llm::details::t_Erode(*img, poutre::se::SESegmentX2D, img_out);

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

TEST_CASE("dilate segmentY2D runtime", "[low_level_morpho]")
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
  poutre::llm::details::t_Dilate(*img, poutre::se::SESegmentY2D, img_out);

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

TEST_CASE("erode segmentY2D runtime", "[low_level_morpho]")
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
  poutre::llm::details::t_Erode(*img, poutre::se::SESegmentY2D, img_out);

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

TEST_CASE("dilate segmentX3D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 3,3,3};

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 3 3 3 3
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,3>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,3> img_out(shape);
  poutre::llm::details::t_Dilate(*img, poutre::se::SESegmentX3D, img_out);

  const std::string expected =
    "Scalar GINT64 3 3 3 3\
 8 8 0\
 5 5 5\
 5 6 6\
 8 8 0\
 5 5 5\
 5 6 6\
 8 8 0\
 5 5 5\
 5 6 6";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("erode segmentX3D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 3,3,3};

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 3 3 3 3
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,3>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,3> img_out(shape);
  poutre::llm::details::t_Erode(*img, poutre::se::SESegmentX3D, img_out);

  const std::string expected =
    "Scalar GINT64 3 3 3 3\
 0 0 0\
 0 0 5\
 0 0 5\
 0 0 0\
 0 0 5\
 0 0 5\
 0 0 0\
 0 0 5\
 0 0 5";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("dilate segmentY3D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 3,3,3};

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 3 3 3 3
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,3>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,3> img_out(shape);
  poutre::llm::details::t_Dilate(*img, poutre::se::SESegmentY3D, img_out);

  const std::string expected =
    "Scalar GINT64 3 3 3 3\
 8 5 5\
 8 5 6\
 0 5 6\
 8 5 5\
 8 5 6\
 0 5 6\
 8 5 5\
 8 5 6\
 0 5 6";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("erode segmentY3D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 3,3,3};

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 3 3 3 3
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,3>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,3> img_out(shape);
  poutre::llm::details::t_Erode(*img, poutre::se::SESegmentY3D, img_out);

  const std::string expected =
    "Scalar GINT64 3 3 3 3\
 0 0 0\
 0 0 0\
 0 5 5\
 0 0 0\
 0 0 0\
 0 5 5\
 0 0 0\
 0 0 0\
 0 5 5";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}
TEST_CASE("dilate segmentZ3D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 3,3,3};

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 3 3 3 3
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,3>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,3> img_out(shape);
  poutre::llm::details::t_Dilate(*img, poutre::se::SESegmentZ3D, img_out);

  const std::string expected =
    "Scalar GINT64 3 3 3 3\
 8 0 0\
 0 5 5\
 0 5 6\
 8 0 0\
 0 5 5\
 0 5 6\
 8 0 0\
 0 5 5\
 0 5 6";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("erode segmentZ3D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 3,3,3};

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 3 3 3 3
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,3>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,3> img_out(shape);
  poutre::llm::details::t_Erode(*img, poutre::se::SESegmentZ3D, img_out);

  const std::string expected =
    "Scalar GINT64 3 3 3 3\
 8 0 0\
 0 5 5\
 0 5 6\
 8 0 0\
 0 5 5\
 0 5 6\
 8 0 0\
 0 5 5\
 0 5 6";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("dilate square3D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 3,3,3};

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 3 3 3 3
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,3>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,3> img_out(shape);
  poutre::llm::details::t_Dilate(*img, poutre::se::SESquare3D, img_out);

  const std::string expected =
    "Scalar GINT64 3 3 3 3\
 8 8 5\
 8 8 6\
 5 6 6\
 8 8 5\
 8 8 6\
 5 6 6\
 8 8 5\
 8 8 6\
 5 6 6";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("erode square3D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 3,3,3};

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 3 3 3 3
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,3>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,3> img_out(shape);
  poutre::llm::details::t_Erode(*img, poutre::se::SESquare3D, img_out);

  const std::string expected =
    "Scalar GINT64 3 3 3 3\
 0 0 0\
 0 0 0\
 0 0 5\
 0 0 0\
 0 0 0\
 0 0 5\
 0 0 0\
 0 0 0\
 0 0 5";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("dilate cross3D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 3,3,3};

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 3 3 3 3
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,3>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,3> img_out(shape);
  poutre::llm::details::t_Dilate(*img, poutre::se::SECross3D, img_out);

  const std::string expected =
    "Scalar GINT64 3 3 3 3\
 8 8 5\
 8 5 6\
 5 6 6\
 8 8 5\
 8 5 6\
 5 6 6\
 8 8 5\
 8 5 6\
 5 6 6";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}

TEST_CASE("erode cross3D runtime", "[low_level_morpho]")
{
  const std::vector<std::size_t> shape = { 3,3,3};

  const auto img_in = poutre::ImageFromString(
    R"(Scalar GINT64 3 3 3 3
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
 8 0 0
 0 5 5
 0 5 6
)");
  using ImageType = const poutre::details::image_t<poutre::pINT64,3>;
  const auto *img = dynamic_cast<ImageType *>(img_in.get());
  poutre::details::image_t<poutre::pINT64,3> img_out(shape);
  poutre::llm::details::t_Erode(*img, poutre::se::SECross3D, img_out);

  const std::string expected =
    "Scalar GINT64 3 3 3 3\
 0 0 0\
 0 0 0\
 0 0 5\
 0 0 0\
 0 0 0\
 0 0 5\
 0 0 0\
 0 0 0\
 0 0 5";
  const auto img_str = poutre::ImageToString(img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}