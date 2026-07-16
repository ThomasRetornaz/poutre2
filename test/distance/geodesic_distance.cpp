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
#include <poutre/distance/geodesic_distance.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

#include <memory>
#include <string>


TEST_CASE("simple case geo cross2D", "[distance]")
{
  using PtrInterface = std::unique_ptr<poutre::IInterface>;
  const auto img_marker = poutre::ImageFromString(
    "Scalar GINT64 2 5 5 \
0 1 0 0 0 \
0 0 0 0 0 \
0 0 0 0 0 \
0 0 0 0 0 \
0 0 0 0 0");

  const auto img_mask = poutre::ImageFromString(
    "Scalar GINT64 2 5 5 \
0 1 1 1 1 \
1 1 1 1 0 \
1 1 1 0 0 \
1 1 1 1 1 \
1 1 1 1 1");


  PtrInterface img_out = poutre::CloneGeometry(*img_marker);
  REQUIRE(img_out.get() != nullptr);

  poutre::dist::geodesic_distance(*img_marker, *img_mask, poutre::se::Common_NL_SE::SECross2D, *img_out);
  const std::string expected =
    "Scalar GINT64 2 5 5 \
0 0 1 2 3 \
2 1 2 3 0 \
3 2 3 0 0 \
4 3 4 5 6 \
5 4 5 6 7";
  const auto img_str = poutre::ImageToString(*img_out);
  REQUIRE_THAT(img_str, Catch::Matchers::Equals(expected));
}