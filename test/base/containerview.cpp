// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================


#include <cstddef>
#include <poutre/base/details/data_structures/array_view.hpp>

#include <catch2/catch_test_macros.hpp>

// NOLINTBEGIN

TEST_CASE("ctor", "[view]")
{
  using view1DINt = poutre::details::av::array_view<int, 1>;

  view1DINt dummyview;
  REQUIRE(dummyview.size() == 0);
  REQUIRE(dummyview.data() == nullptr);
  REQUIRE(dummyview.stride() == poutre::details::av::index<1>{ 1 });// stride is 1 for empty view

  // on 1D
  using sview1DINt = poutre::details::av::strided_array_view<int, 1>;

  sview1DINt dummysview;
  REQUIRE(dummysview.size() == 0);
  // std::cout << "dummysview.stride()" << dummysview.stride() << std::endl;
  REQUIRE(dummysview.stride() == poutre::details::av::index<1>{ 0 });// stride is 0 for empty stridedview
}

TEST_CASE("over vector", "[view]")
{
  using view1DINt = poutre::details::av::array_view<int, 1>;
  using bd1d = poutre::details::av::bd1d;
  using idx1d = poutre::details::av::idx1d;
  using bd2d = poutre::details::av::bd2d;
  using idx2d = poutre::details::av::idx2d;

  std::vector<int> mif = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  auto view = view1DINt{ mif };

  REQUIRE(view.size() == 10);
  REQUIRE(view.bound() == poutre::details::av::bounds<1>{ 10 });
  view[idx1d{ 0 }] = 42;// grmmlll idx1d{} to force resolution operator[index] and not slice ....
  REQUIRE(mif[0] == 42);// v == 42

  auto viewcopy(view);
  REQUIRE(viewcopy[idx1d{ 0 }] == 42);// v == 42
  auto viewcopy2 = view;
  REQUIRE(viewcopy2[idx1d{ 0 }] == 42);// v == 42

  auto viewptr = view1DINt(mif.data(), poutre::details::av::bd1d{ static_cast<long>(mif.size()) });
  REQUIRE(viewptr[idx1d{ 0 }] == 42);// v == 42

  auto view2d = poutre::details::av::array_view<int, 2>(mif, { 2, 5 });// 2D view over vec
  REQUIRE(view2d.size() == 10);
  REQUIRE(view2d.bound() == (bd2d{ 2, 5 }));
  REQUIRE(view2d.stride() == (idx2d{ 5, 1 }));
  REQUIRE(view2d[(idx2d{ 0, 0 })] == 42);

  auto av1 = poutre::details::av::array_view<int, 1>(mif, { 5 });// 1D view over vec with explicit sub bound
  REQUIRE(av1.bound() == (bd1d{ 5 }));
  REQUIRE(av1.stride() == (idx1d{ 1 }));
  REQUIRE(av1[idx1d{ 0 }] == 42);

  auto av1DFrom2D = poutre::details::av::array_view<int, 1>(view2d, { 2 });// 1D view from 2D view      }
  REQUIRE(av1DFrom2D.bound() == (bd1d{ 2 }));
  REQUIRE(av1DFrom2D.stride() == (idx1d{ 1 }));
  REQUIRE(av1DFrom2D[idx1d{ 0 }] == 42);

  auto view2dsub = poutre::details::av::array_view<int, 2>(mif, { 2, 3 });// 2D view over vec
  REQUIRE(view2dsub.size() == 6);
  REQUIRE(view2dsub.bound() == (bd2d{ 2, 3 }));
  REQUIRE(view2dsub.stride() == (idx2d{ 3, 1 }));
  REQUIRE(view2dsub[(idx2d{ 0, 0 })] == 42);
}

TEST_CASE("basic_usage_strided_view_over_vector", "[view]")
{
  using sview1DINt = poutre::details::av::strided_array_view<int, 1>;
  using bd1d = poutre::details::av::bd1d;
  using idx1d = poutre::details::av::idx1d;

  std::vector<int> mif = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  auto view = sview1DINt(&mif[0], { 10 }, { 3 });

  REQUIRE(view.size() == 10);
  REQUIRE(view.bound() == bd1d{ 10 });

  view[idx1d{ 0 }] = 42;// grmmlll idx1d{} to force resolution operator[index] and not slice ....
  REQUIRE(mif[0] == 42);// v == 42

  auto viewcopy(view);
  REQUIRE(viewcopy[idx1d{ 0 }] == 42);// v == 42
  auto viewcopy2 = view;
  REQUIRE(viewcopy2[idx1d{ 0 }] == 42);// v == 42
}


TEST_CASE("section and slice 1D", "[view]")
{
  std::vector<int> mif = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  auto view = poutre::details::av::array_view<int, 1>(mif);

  using bd1d = poutre::details::av::bd1d;
  using idx1d = poutre::details::av::idx1d;

  auto section = view.section(idx1d{ 2 });
  REQUIRE(section.size() == 8);
  REQUIRE(section.stride() == idx1d{ 1 });
  REQUIRE(section.bound() == (bd1d{ 8 }));
  REQUIRE(section[(idx1d{ 0 })] == 2);
  REQUIRE(section[(idx1d{ 1 })] == 3);

  auto sectionwithbound = view.section(idx1d{ 2 }, (bd1d{ 3 }));
  REQUIRE(sectionwithbound.size() == 3);
  REQUIRE(sectionwithbound.stride() == (idx1d{ 1 }));
  REQUIRE(sectionwithbound.bound() == (bd1d{ 3 }));
  REQUIRE(sectionwithbound[idx1d{ 0 }] == 2);
  REQUIRE(sectionwithbound[idx1d{ 1 }] == 3);
}

TEST_CASE("section and slice_2d", "[view]")
{
  // view 1d view 2d  with section {{1,2},{3,2}} highlighted
  //
  //  * * * * * * * 7 8 * * * 12 13 * * * 17 18 * * * * * *
  //
  //  * *  *  * *
  //  * *  7  8 *
  //  * * 12 13 *
  //  * * 17 18 *
  //  * *  *  * *

  using bd1d = poutre::details::av::bd1d;
  using idx1d = poutre::details::av::idx1d;
  using bd2d = poutre::details::av::bd2d;
  using idx2d = poutre::details::av::idx2d;

  std::vector<int> mif(25);
  for (std::size_t i = 0; i < 25; ++i) { mif[i] = static_cast<int>(i); }

  {
    auto view2d = poutre::details::av::array_view<int, 2>(mif, { 5, 5 });
    REQUIRE(view2d.size() == 25);
    REQUIRE(view2d.bound() == (bd2d{ 5, 5 }));
    REQUIRE(view2d.stride() == (idx2d{ 5, 1 }));

    auto slicesefirstline = view2d[0];
    REQUIRE(slicesefirstline.size() == 5);
    REQUIRE(slicesefirstline.bound() == (bd1d{ 5 }));
    REQUIRE(slicesefirstline.stride() == (idx1d{ 1 }));
    REQUIRE(slicesefirstline[idx1d{ 0 }] == 0);
    REQUIRE(slicesefirstline[idx1d{ 1 }] == 1);
    REQUIRE(slicesefirstline[idx1d{ 2 }] == 2);
    REQUIRE(slicesefirstline[idx1d{ 3 }] == 3);
    REQUIRE(slicesefirstline[idx1d{ 4 }] == 4);//-V112

    auto slicesecondline = view2d[1];
    REQUIRE(slicesecondline.size() == 5);
    REQUIRE(slicesecondline.bound() == (bd1d{ 5 }));
    REQUIRE(slicesecondline.stride() == (idx1d{ 1 }));
    REQUIRE(slicesecondline[idx1d{ 0 }] == 5);
    REQUIRE(slicesecondline[idx1d{ 1 }] == 6);
    REQUIRE(slicesecondline[idx1d{ 2 }] == 7);
    REQUIRE(slicesecondline[idx1d{ 3 }] == 8);
    REQUIRE(slicesecondline[idx1d{ 4 }] == 9);//-V112

    auto slicethrirdline = view2d[2];
    REQUIRE(slicethrirdline.size() == 5);
    REQUIRE(slicethrirdline.bound() == (bd1d{ 5 }));
    REQUIRE(slicethrirdline[idx1d{ 0 }] == 10);
    REQUIRE(slicethrirdline[idx1d{ 1 }] == 11);
    REQUIRE(slicethrirdline[idx1d{ 2 }] == 12);
    REQUIRE(slicethrirdline[idx1d{ 3 }] == 13);
    REQUIRE(slicethrirdline[idx1d{ 4 }] == 14);//-V112
    REQUIRE(slicethrirdline.stride() == (idx1d{ 1 }));
  }

  {
    auto view2d = poutre::details::av::array_view<int, 2>(mif, { 5, 5 });
    auto section2d = view2d.section(idx2d{ 1, 1 });
    REQUIRE(section2d.size() == 16);
    REQUIRE(section2d.bound() == (bd2d({ 4, 4 })));//-V112
    REQUIRE(section2d.stride() == (idx2d({ 5, 1 })));
    REQUIRE(section2d[idx2d({ 0, 0 })] == 6);
    REQUIRE(section2d[idx2d({ 0, 1 })] == 7);
    REQUIRE(section2d[idx2d({ 0, 2 })] == 8);
    REQUIRE(section2d[idx2d({ 0, 3 })] == 9);
    REQUIRE(section2d[idx2d({ 1, 0 })] == 11);
    REQUIRE(section2d[idx2d({ 1, 1 })] == 12);
    REQUIRE(section2d[idx2d({ 1, 2 })] == 13);
    REQUIRE(section2d[idx2d({ 1, 3 })] == 14);
    REQUIRE(section2d[idx2d({ 2, 0 })] == 16);
    REQUIRE(section2d[idx2d({ 2, 1 })] == 17);
    REQUIRE(section2d[idx2d({ 2, 2 })] == 18);
    REQUIRE(section2d[idx2d({ 2, 3 })] == 19);
    REQUIRE(section2d[idx2d({ 3, 0 })] == 21);
    REQUIRE(section2d[idx2d({ 3, 1 })] == 22);
    REQUIRE(section2d[idx2d({ 3, 2 })] == 23);
    REQUIRE(section2d[idx2d({ 3, 3 })] == 24);
  }
  {
    std::vector<int> tmp(30);
    for (std::size_t i = 0; i < 30; ++i) { tmp[i] = static_cast<int>(i); }
    auto view2d = poutre::details::av::array_view<int, 2>(tmp, { 5, 6 });
    auto section2d = view2d.section(idx2d{ 1, 1 });
    REQUIRE(section2d.size() == 20);
    REQUIRE(section2d.bound() == (bd2d({ 4, 5 })));//-V112
    REQUIRE(section2d.stride() == (idx2d({ 6, 1 })));
    REQUIRE(section2d[idx2d({ 0, 0 })] == 7);
    REQUIRE(section2d[idx2d({ 0, 1 })] == 8);
    REQUIRE(section2d[idx2d({ 0, 2 })] == 9);
    REQUIRE(section2d[idx2d({ 0, 3 })] == 10);
    REQUIRE(section2d[idx2d({ 0, 4 })] == 11);//-V112

    REQUIRE(section2d[idx2d({ 3, 0 })] == 25);
    REQUIRE(section2d[idx2d({ 3, 1 })] == 26);
    REQUIRE(section2d[idx2d({ 3, 2 })] == 27);
    REQUIRE(section2d[idx2d({ 3, 3 })] == 28);
    REQUIRE(section2d[idx2d({ 3, 4 })] == 29);//-V112
  }
  {
    auto view2d = poutre::details::av::array_view<int, 2>(mif, { 5, 5 });
    auto section2d = view2d.section(idx2d{ 1, 2 });
    REQUIRE(section2d.size() == 12);
    REQUIRE(section2d.bound() == (bd2d({ 4, 3 })));//-V112
    REQUIRE(section2d.stride() == (idx2d({ 5, 1 })));
    // auto bnd = section2d.bound();
    // std::cout << std::endl;
    // for (auto idx : bnd)
    //{
    //    std::cout << idx << std::endl;
    //}

    REQUIRE(section2d[idx2d({ 0, 0 })] == 7);
    REQUIRE(section2d[idx2d({ 0, 1 })] == 8);
    REQUIRE(section2d[idx2d({ 0, 2 })] == 9);
    REQUIRE(section2d[idx2d({ 1, 0 })] == 12);
    REQUIRE(section2d[idx2d({ 1, 1 })] == 13);
    REQUIRE(section2d[idx2d({ 1, 2 })] == 14);
    REQUIRE(section2d[idx2d({ 2, 0 })] == 17);
    REQUIRE(section2d[idx2d({ 2, 1 })] == 18);
    REQUIRE(section2d[idx2d({ 2, 2 })] == 19);
    REQUIRE(section2d[idx2d({ 3, 0 })] == 22);
    REQUIRE(section2d[idx2d({ 3, 1 })] == 23);
    REQUIRE(section2d[idx2d({ 3, 2 })] == 24);

    auto slice = section2d[0];
    REQUIRE(slice.size() == 3);
    REQUIRE(slice.bound() == (bd1d({ 3 })));
    REQUIRE(slice.stride() == (idx1d({ 1 })));

    REQUIRE(slice[idx1d({ 0 })] == 7);
    REQUIRE(slice[idx1d({ 1 })] == 8);
    REQUIRE(slice[idx1d({ 2 })] == 9);

    auto slice2 = section2d[1];
    REQUIRE(slice2.size() == 3);
    REQUIRE(slice2.bound() == (bd1d({ 3 })));
    REQUIRE(slice2.stride() == (idx1d({ 1 })));

    REQUIRE(slice2[idx1d({ 0 })] == 12);
    REQUIRE(slice2[idx1d({ 1 })] == 13);
    REQUIRE(slice2[idx1d({ 2 })] == 14);
  }

  {
    auto view2d = poutre::details::av::array_view<int, 2>(mif, { 5, 5 });
    auto section2d = view2d.section((idx2d{ 1, 2 }), (bd2d{ 3, 2 }));
    REQUIRE(section2d.size() == 6);
    REQUIRE(section2d.bound() == (bd2d({ 3, 2 })));
    REQUIRE(section2d.stride() == (idx2d({ 5, 1 })));

    //  for(auto idx : section2d.bound())
    //  {
    //      std::cout<<"\nidx"<<idx;
    //      std::cout<<"\nvalue"<<section2d[idx];
    //  }
    REQUIRE(section2d[idx2d({ 0, 0 })] == 7);
    REQUIRE(section2d[idx2d({ 0, 1 })] == 8);
    REQUIRE(section2d[idx2d({ 1, 0 })] == 12);
    REQUIRE(section2d[idx2d({ 1, 1 })] == 13);
    REQUIRE(section2d[idx2d({ 2, 0 })] == 17);
    REQUIRE(section2d[idx2d({ 2, 1 })] == 18);

    //  * 1  2  3 *
    //  * 6  7  8 *
    //  * *  *  * *
    //  * *  *  * *
    //  * *  *  * *
    auto sec2d = view2d.section((idx2d{ 0, 1 }), (bd2d{ 2, 3 }));
    REQUIRE(sec2d.size() == 6);
    REQUIRE(sec2d.bound() == (bd2d({ 2, 3 })));
    REQUIRE(sec2d.stride() == (idx2d({ 5, 1 })));

    //  for(auto idx : sec2d.bound())
    //  {
    //      std::cout<<"\nidx"<<idx;
    //      std::cout<<"\nvalue"<<sec2d[idx];
    //  }
    REQUIRE(sec2d[idx2d({ 0, 0 })] == 1);
    REQUIRE(sec2d[idx2d({ 0, 1 })] == 2);
    REQUIRE(sec2d[idx2d({ 0, 2 })] == 3);
    REQUIRE(sec2d[idx2d({ 1, 0 })] == 6);
    REQUIRE(sec2d[idx2d({ 1, 1 })] == 7);
    REQUIRE(sec2d[idx2d({ 1, 2 })] == 8);

    //  1 2 3
    //  6 7 8
    auto subsec2d = sec2d.section((idx2d{ 1, 1 }));//,(bd2d{1,2}));
    /* for (auto idx : subsec2d.bound())
     {
         std::cout << "\nidx" << idx;
         std::cout << "\nvalue" << subsec2d[idx];
     }*/

    REQUIRE(subsec2d.size() == 2);
    REQUIRE(subsec2d.bound() == (bd2d({ 1, 2 })));
    REQUIRE(subsec2d.stride() == (idx2d({ 5, 1 })));
    REQUIRE(subsec2d[idx2d({ 0, 0 })] == 7);
    REQUIRE(subsec2d[idx2d({ 0, 1 })] == 8);
  }
}

TEST_CASE("carray", "[view]")
{
  using view_type = poutre::details::av::array_view<int, 1>;
  using bd1d = poutre::details::av::bd1d;
  using idx1d = poutre::details::av::idx1d;

  int a[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  view_type av{ a };
  REQUIRE(av.size() == 10);
  REQUIRE(av.bound() == bd1d{ 10 });
  // assignment through view
  av[idx1d{ 0 }] = 42;// grmmlll idx1d{} to force resolution operator[index] and not slice ....
  REQUIRE(a[0] == 42);// v == 42
  // copy view
  auto viewcopy(av);
  REQUIRE(viewcopy[idx1d{ 0 }] == 42);// v == 42
  auto viewcopy2 = av;
  REQUIRE(viewcopy2[idx1d{ 0 }] == 42);// v == 42
}

TEST_CASE("strided view", "[view]")
{
  using bd2d = poutre::details::av::bd2d;
  using idx2d = poutre::details::av::idx2d;

  // view 1d view 2d  with section {{1,2},{3,2}} highlighted
  //
  //  * * * * * * * 7 8 * * * 12 13 * * * 17 18 * * * * * *
  //
  //   0  1  2  3  4
  //   5  6  7  8  9
  //  10 11 12 13 14
  //   *  * 17 18  *
  //   *  *  *  *  *

  std::vector<int> mif(25);
  for (std::size_t i = 0; i < 25; ++i) { mif[i] = static_cast<int>(i); }
  {
    // auto view2d = poutre::details::av::array_view<int, 2>(mif, { 5, 5 });
    auto sview2d = poutre::details::av::strided_array_view<int, 2>(
      &mif[0], bd2d{ 5, 2 }, idx2d{ 5, 3 });// stride in least significant dimension

    REQUIRE(sview2d[idx2d({ 0, 0 })] == 0);
    REQUIRE(sview2d[idx2d({ 0, 1 })] == 3);
    REQUIRE(sview2d[idx2d({ 1, 0 })] == 5);
    REQUIRE(sview2d[idx2d({ 1, 1 })] == 8);
    REQUIRE(sview2d[idx2d({ 2, 0 })] == 10);
    REQUIRE(sview2d[idx2d({ 2, 1 })] == 13);
    REQUIRE(sview2d[idx2d({ 3, 0 })] == 15);
    REQUIRE(sview2d[idx2d({ 3, 1 })] == 18);
    REQUIRE(sview2d[idx2d({ 4, 0 })] == 20);//-V112
    REQUIRE(sview2d[idx2d({ 4, 1 })] == 23);//-V112
  }
  {
    auto sview2d = poutre::details::av::strided_array_view<int, 2>(&mif[0], bd2d{ 3, 5 }, idx2d{ 10, 1 });
    REQUIRE(sview2d[idx2d({ 0, 0 })] == 0);
    REQUIRE(sview2d[idx2d({ 0, 1 })] == 1);
    REQUIRE(sview2d[idx2d({ 0, 2 })] == 2);
    REQUIRE(sview2d[idx2d({ 0, 3 })] == 3);
    REQUIRE(sview2d[idx2d({ 0, 4 })] == 4);//-V112

    REQUIRE(sview2d[idx2d({ 1, 0 })] == 10);
    REQUIRE(sview2d[idx2d({ 1, 1 })] == 11);
    REQUIRE(sview2d[idx2d({ 1, 2 })] == 12);
    REQUIRE(sview2d[idx2d({ 1, 3 })] == 13);
    REQUIRE(sview2d[idx2d({ 1, 4 })] == 14);//-V112

    REQUIRE(sview2d[idx2d({ 2, 0 })] == 20);
    REQUIRE(sview2d[idx2d({ 2, 1 })] == 21);
    REQUIRE(sview2d[idx2d({ 2, 2 })] == 22);
    REQUIRE(sview2d[idx2d({ 2, 3 })] == 23);
    REQUIRE(sview2d[idx2d({ 2, 4 })] == 24);//-V112
  }

  {
    auto sview2d =
      poutre::details::av::strided_array_view<int, 2>(&mif[0], bd2d{ 3, 2 }, idx2d{ 10, 3 });// two different stride
    REQUIRE(sview2d[idx2d({ 0, 0 })] == 0);
    REQUIRE(sview2d[idx2d({ 0, 1 })] == 3);
    REQUIRE(sview2d[idx2d({ 1, 0 })] == 10);
    REQUIRE(sview2d[idx2d({ 1, 1 })] == 13);
    REQUIRE(sview2d[idx2d({ 2, 0 })] == 20);
    REQUIRE(sview2d[idx2d({ 2, 1 })] == 23);
  }
}

// TEST_CASE("static dispatch", "[view]")
// {
//   static_assert(false == poutre::details::av::is_strided<poutre::details::av::array_view<int, 1>>::value, "oups");
//   static_assert(
//     true == poutre::details::av::is_strided<poutre::details::av::strided_array_view<int, 1>>::value, "oups");
//   static_assert(false == poutre::details::av::is_strided<poutre::details::av::array_view<const int, 1>>::value,
//   "oups"); static_assert(
//     true == poutre::details::av::is_strided<poutre::details::av::strided_array_view<const int, 1>>::value, "oups");
// }
// NOLINTEND