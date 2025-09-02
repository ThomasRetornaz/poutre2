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
// #include <catch2/matchers/catch_matchers.hpp>
// #include <catch2/matchers/catch_matchers_range_equals.hpp>
// #include <catch2/matchers/catch_matchers_string.hpp>
// #include <catch2/matchers/catch_matchers_vector.hpp>

#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <cstddef>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>
// #include <poutre/base/types_traits.hpp>
#include <array>
// #include <iostream>
#include <string>
#include <utility>
#include <vector>


TEST_CASE("bound iterator", "[iterator_view]")
{
  const std::string img_str =
    "Scalar GUINT8 2 3 4\
   1 2 3 4\
   10 12 13 14\
   20 22 23 24\
  ";
  const auto img = poutre::ImageFromString(img_str);
  auto *img_t = dynamic_cast<poutre::details::image_t<poutre::pUINT8, 2> *>(&(*img));
  const auto view1 = poutre::details::view(*img_t);
  REQUIRE(view1.size() == 12);
  auto bnd = view1.bound();
  REQUIRE(bnd.size() == 12);
  REQUIRE(bnd[0] == 3);
  REQUIRE(bnd[1] == 4);
  auto stride = view1.stride();
  REQUIRE(stride[0] == 4);
  REQUIRE(stride[1] == 1);

  const std::vector<poutre::pUINT8> expected={1, 2 ,3 ,4, 10, 12 ,13 ,14,20 ,22 ,23 ,24};
  {
    auto beg1 = begin(bnd);
    auto end1 = end(bnd);

    for( size_t idx=0; beg1 != end1 && (idx < expected.size()); ++beg1, ++idx ) {
      // retrieve offset from coord
      auto current_offset = poutre::details::av::get_offset_from_coord_uniform_stride<2>::op(bnd,*beg1);
      REQUIRE(std::cmp_equal(current_offset,idx));
      auto curr_val = view1[*beg1];
      REQUIRE(curr_val==expected[idx]);
      // retrieve coord
      poutre::details::av::index<2> retrieve_idx = {0,0};
      poutre::details::av::get_coord_from_offset_uniform_stride<2>::op(bnd, current_offset, retrieve_idx);
      REQUIRE((*beg1)[0]==retrieve_idx[0]);
      REQUIRE((*beg1)[1]==retrieve_idx[1]);
    }
  }
  auto beg1 = begin(bnd);
  auto end1 = end(bnd);

  for( size_t idx=0; beg1 != end1 && (idx < expected.size()); ++beg1, ++idx ) {
    // retrieve offset from coord
    auto current_offset = poutre::details::av::get_offset_from_coord<2>::op(*beg1, stride);
    REQUIRE(std::cmp_equal(current_offset,idx));
    auto curr_val = view1[*beg1];
    REQUIRE(curr_val==expected[idx]);
    // retrieve coord
    // poutre::details::av::index<2> retrieve_idx = {0,0};
    // poutre::details::av::get_coord_from_offset<2>::op(bnd, current_offset, retrieve_idx);
    // REQUIRE((*beg1)[0]==retrieve_idx[0]);
    // REQUIRE((*beg1)[1]==retrieve_idx[1]);
  }
}

TEST_CASE("bound iterator and nl center", "[iterator_view]")
{
  const std::string img_str =
    "Scalar GUINT8 2 3 3\
   1 2 3\
   10 12 13\
   20 22 23\
  ";
  const auto img = poutre::ImageFromString(img_str);
  auto *img_t = dynamic_cast<poutre::details::image_t<poutre::pUINT8, 2> *>(&(*img));
  const auto view1 = poutre::details::view(*img_t);
  REQUIRE(view1.size() == 9);
  auto bnd = view1.bound();
  REQUIRE(bnd.size() == 9);
  REQUIRE(bnd[0] == 3);
  REQUIRE(bnd[1] == 3);
  auto stride = view1.stride();
  REQUIRE(stride[0] == 3);
  REQUIRE(stride[1] == 1);

  using neighbor_element = poutre::details::av::index<2>;
  constexpr std::array<neighbor_element, 4> nl_upper = {
    neighbor_element{-1, -1},
    neighbor_element{-1, 0},
    neighbor_element{-1, 1},
    neighbor_element{0, -1},
   };

  constexpr std::array<neighbor_element, 4> nl_lower = {
    neighbor_element{0, +1},
    neighbor_element{1, -1},
    neighbor_element{+1, +0},
    neighbor_element{+1, +1},
   };

  const std::vector<poutre::pUINT8> expected={1, 2 ,3 , 10, 12 ,13 , 20 ,22 ,23 };
  const std::vector<poutre::pUINT8> expected_nl_upper={1, 2 , 3 , 10 };
  const std::vector<poutre::pUINT8> expected_nl_lower={13 , 20 , 22 ,23 };

    auto beg1 = begin(bnd);
    auto end1 = end(bnd);

    for( size_t idx=0; beg1 != end1 && (idx < expected.size()); ++beg1, ++idx ) {
      auto curr_val = view1[*beg1];
      REQUIRE(curr_val==expected[idx]);

      if (idx==4) // center point
       {
         {
           const poutre::scoord centerx = (*beg1)[1];
           const poutre::scoord centery = (*beg1)[0];
           REQUIRE(centerx == 1);
           REQUIRE(centery == 1);
           size_t count = 0;
           for( const auto &idx_nl_upper : nl_upper ) {
             const auto delta_nl_idx = *beg1 + idx_nl_upper;
             if( !bnd.contains(delta_nl_idx) ) {
               continue;
             }
             auto curr_val_nl = view1[delta_nl_idx];
             REQUIRE(expected_nl_upper[count] == curr_val_nl);
             count++;
           }
         }
         {
           const poutre::scoord centerx = (*beg1)[1];
           const poutre::scoord centery = (*beg1)[0];
           REQUIRE(centerx == 1);
           REQUIRE(centery == 1);
           size_t count = 0;
           for( const auto &idx_nl_lower : nl_lower ) {
             const auto delta_nl_idx = *beg1 + idx_nl_lower;
             if( !bnd.contains(delta_nl_idx) ) {
               continue;
             }
             auto curr_val_nl = view1[delta_nl_idx];
             REQUIRE(expected_nl_lower[count] == curr_val_nl);
             count++;
           }
         }
      }
    }
}
TEST_CASE("bound iterator and nl boundaries upper", "[iterator_view]")
{
  const std::string img_str =
    "Scalar GUINT8 2 3 3\
   1 2 3\
   10 12 13\
   20 22 23\
  ";
  const auto img = poutre::ImageFromString(img_str);
  auto *img_t = dynamic_cast<poutre::details::image_t<poutre::pUINT8, 2> *>(&(*img));
  const auto view1 = poutre::details::view(*img_t);
  REQUIRE(view1.size() == 9);
  auto bnd = view1.bound();
  REQUIRE(bnd.size() == 9);
  REQUIRE(bnd[0] == 3);
  REQUIRE(bnd[1] == 3);
  auto stride = view1.stride();
  REQUIRE(stride[0] == 3);
  REQUIRE(stride[1] == 1);

  using neighbor_element = poutre::details::av::index<2>;
  constexpr std::array<neighbor_element, 4> nl_upper = {
    neighbor_element{-1, -1},
    neighbor_element{-1, 0},
    neighbor_element{-1, 1},
    neighbor_element{0, -1},
   };

  constexpr std::array<neighbor_element, 4> nl_lower = {
    neighbor_element{0, +1},
    neighbor_element{1, -1},
    neighbor_element{+1, +0},
    neighbor_element{+1, +1},
   };

  const std::vector<poutre::pUINT8> expected={1, 2 ,3 , 10, 12 ,13 , 20 ,22 ,23 };
  // const std::vector<poutre::pUINT8> expected_nl_upper={1, 2 , 3 , 10 };
  // const std::vector<poutre::pUINT8> expected_nl_lower={13 , 20 , 22 ,23 };

  auto beg1 = begin(bnd);
  auto end1 = end(bnd);

  for( size_t idx=0; beg1 != end1 && (idx < expected.size()); ++beg1, ++idx ) {
    auto curr_val = view1[*beg1];
    REQUIRE(curr_val==expected[idx]);
    if (idx==0) // left top most
    {
      const poutre::scoord centerx = (*beg1)[1];
      const poutre::scoord centery = (*beg1)[0];
      REQUIRE(centerx == 0);
      REQUIRE(centery == 0);
      size_t count = 0;
      for( const auto &idx_nl_upper : nl_upper ) {
        const auto delta_nl_idx = *beg1 + idx_nl_upper;
        if( !bnd.contains(delta_nl_idx) ) {continue;}
        count++;
      }
      REQUIRE(count == 0);
      count=0;
      for( const auto &idx_nl_lower : nl_lower ) {
        const auto delta_nl_idx = *beg1 + idx_nl_lower;
        if( !bnd.contains(delta_nl_idx) ) {
          continue;
        }
        count++;
      }
      REQUIRE(count == 3);
    }
  }
}

TEST_CASE("bound iterator and nl boundaries lower", "[iterator_view]")
{
  const std::string img_str =
    "Scalar GUINT8 2 3 3\
   1 2 3\
   10 12 13\
   20 22 23\
  ";
  const auto img = poutre::ImageFromString(img_str);
  auto *img_t = dynamic_cast<poutre::details::image_t<poutre::pUINT8, 2> *>(&(*img));
  const auto view1 = poutre::details::view(*img_t);
  REQUIRE(view1.size() == 9);
  auto bnd = view1.bound();
  REQUIRE(bnd.size() == 9);
  REQUIRE(bnd[0] == 3);
  REQUIRE(bnd[1] == 3);
  auto stride = view1.stride();
  REQUIRE(stride[0] == 3);
  REQUIRE(stride[1] == 1);

  using neighbor_element = poutre::details::av::index<2>;
  constexpr std::array<neighbor_element, 4> nl_upper = {
    neighbor_element{-1, -1},
    neighbor_element{-1, 0},
    neighbor_element{-1, 1},
    neighbor_element{0, -1},
   };

  constexpr std::array<neighbor_element, 4> nl_lower = {
    neighbor_element{0, +1},
    neighbor_element{1, -1},
    neighbor_element{+1, +0},
    neighbor_element{+1, +1},
   };

  const std::vector<poutre::pUINT8> expected={1, 2 ,3 , 10, 12 ,13 , 20 ,22 ,23 };
  // const std::vector<poutre::pUINT8> expected_nl_upper={1, 2 , 3 , 10 };
  // const std::vector<poutre::pUINT8> expected_nl_lower={13 , 20 , 22 ,23 };

  auto beg1 = begin(bnd);
  auto end1 = end(bnd);

  for( size_t idx=0; beg1 != end1 && (idx < expected.size()); ++beg1, ++idx ) {
    auto curr_val = view1[*beg1];
    REQUIRE(curr_val==expected[idx]);
    if (idx==expected.size()-1) // right down
    {
      const poutre::scoord centerx = (*beg1)[1];
      const poutre::scoord centery = (*beg1)[0];
      REQUIRE(centerx == 2);
      REQUIRE(centery == 2);
      size_t count = 0;
      for( const auto &idx_nl_upper : nl_upper ) {
        const auto delta_nl_idx = *beg1 + idx_nl_upper;
        if( !bnd.contains(delta_nl_idx) ) {continue;}
        count++;
      }
      REQUIRE(count == 3);
      count=0;
      for( const auto &idx_nl_lower : nl_lower ) {
        const auto delta_nl_idx = *beg1 + idx_nl_lower;
        if( !bnd.contains(delta_nl_idx) ) {
          continue;
        }
        count++;
      }
      REQUIRE(count == 0);
    }
  }
}