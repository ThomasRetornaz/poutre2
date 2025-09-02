// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutre/base/details/data_structures/array_view.hpp>

#include <catch2/catch_test_macros.hpp>


// NOLINTBEGIN
TEST_CASE("bound", "[coord]")
{
  poutre::details::av::bounds<1> bnd0{ 5 };
  REQUIRE(bnd0[0] == 5);
  // // REQUIRE(bnd0.at(0) == 5);
  // REQUIRE(bnd0.size() == 5);

  auto bnd1 = poutre::details::av::bounds<3>{ 3, 1, 4 };//-V112
  auto idx = poutre::details::av::index<3>{ 2, -1, 0 };

  // operate with index
  poutre::details::av::bounds<3> bnd2 = bnd1 + idx;// bnd2 is { 5, 0, 4 } and bnd1 unchanged
  REQUIRE(bnd2[0] == 5);
  REQUIRE(bnd2[1] == 0);
  REQUIRE(bnd2[2] == 4);//-V112
  // REQUIRE(bnd2.empty());

  bnd1 -= idx;// bnd1 is { 1, 2, 4 }
  REQUIRE(bnd1[0] == 1);
  REQUIRE(bnd1[1] == 2);
  REQUIRE(bnd1[2] == 4);//-V112
  REQUIRE(bnd1.size() == 8);

  // Arith
  poutre::details::av::bounds<3> bnd4 = bnd1;// bnd4 is {0,1,3} and bdnd1 unchanged
  bnd4 -= poutre::details::av::index<3>{ 1, 1, 1 };
  REQUIRE(bnd4[0] == 0);
  REQUIRE(bnd4[1] == 1);
  REQUIRE(bnd4[2] == 3);

  bnd4 += poutre::details::av::index<3>{ 2, 2, 2 };
  REQUIRE(bnd4[0] == 2);
  REQUIRE(bnd4[1] == 3);
  REQUIRE(bnd4[2] == 5);

  // scaling
  // poutre::details::av::bounds<3> bnd3 = bnd1 * 1.5f;// bnd3 is {1,3,6} and bdnd1 unchanged
  // REQUIRE(bnd3[0] == 1);
  // REQUIRE(bnd3[1] == 3);
  // REQUIRE(bnd3[2] == 6);

  // bnd1 /= 2;
  // REQUIRE(bnd1[0] == 0);
  // REQUIRE(bnd1[1] == 1);
  // REQUIRE(bnd1[2] == 2);

  // contains
  auto bnd = poutre::details::av::bounds<2>{ 3, 3 };
  auto index = poutre::details::av::index<2>{ 2, 2 };
  REQUIRE(bnd.contains(index) == true);
  auto index2 = poutre::details::av::index<2>{ 2, 4 };//-V112
  REQUIRE(!bnd.contains(index2) == true);
}

TEST_CASE("index", "[coord]")
{
  // Scaling
  poutre::details::av::index<2> idx{ 2, 3 };

  REQUIRE(idx[0] == 2);
  REQUIRE(idx[1] == 3);

  // idx *= 1.5f;// res is {3, 4}
  // REQUIRE(idx[0] == 3);
  // REQUIRE(idx[1] == 4);//-V112
}

TEST_CASE("index ordering", "[coord]")
{

  poutre::details::av::index<2> idx{ 2, 3 };
  poutre::details::av::index<2> idx2{ 1, 3 };
  poutre::details::av::index<2> idx3{ 3, 3 };

  REQUIRE(idx2 < idx);
  REQUIRE(idx2 <= idx);
  REQUIRE(idx3 > idx);
  REQUIRE(idx3 >= idx);

  REQUIRE_FALSE(idx == idx2);
  REQUIRE_FALSE(idx2 == idx3);
}

TEST_CASE("bound iter 1d", "[coord]")
{
  using idx = poutre::details::av::index<1>;
  using bnd = poutre::details::av::bounds<1>;
  // using iterator = poutre::details::av::bounds_iterator<1>;

  bnd bnd1D = { 10 };

  std::vector<idx> expected = { { 0 }, { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 }, { 7 }, { 8 }, { 9 } };
  // TODO ?
  // std::vector<idx> rexpected = { {9}, {8}, {7}, {6}, {5}, {4}, {3}, {2}, {1}, {0} };
  using citer = std::vector<idx>::const_iterator;
  citer iter2 = expected.cbegin();
  for (auto iter = bnd1D.begin(); iter != bnd1D.end(); ++iter, ++iter2) { REQUIRE(*iter == *iter2); }

  /*EXPECT_EQ_COLLECTIONS(bnd1D.rbegin( ), bnd1D.rend( ), rexpected.begin( ), rexpected.end( ));
  EXPECT_EQ_COLLECTIONS(rbegin(bnd1D), rend(bnd1D), rexpected.begin( ), rexpected.end( ));*/

  // iterator it(bnd1D);
  // ++it;
  // REQUIRE(it - bnd1D.begin() == 1);
  // iterator it5(bnd1D, poutre::details::av::index<1>{ 5 });
  // REQUIRE(it5 - bnd1D.begin() == 5);
  // --it5;
  // REQUIRE(it5 - bnd1D.begin() == 4);//-V112

  // idx res;
  // poutre::details::get_coord_from_offset_nostride<bnd, idx>::op(bnd1D, 4, res);//-V112
  // REQUIRE(res == idx{ 4 });//-V112

  // auto off = poutre::details::get_offset_from_coord_nostride<bnd, idx>::op(bnd1D, res);
  // REQUIRE(off == 4);//-V112

  // idx outofbound = { 15 };
  // iterator itFromOut(bnd1D, outofbound);
  // REQUIRE(itFromOut == bnd1D.end());
}

TEST_CASE("bound iter 2d", "[coord]")
{
  using bnd = poutre::details::av::bounds<2>;
  using idx = poutre::details::av::index<2>;
  using iterator = poutre::details::av::bounds_iterator<2>;

  bnd bnd2D{ 2, 4 };
  std::vector<idx> expected = {
    idx{ 0, 0 }, idx{ 0, 1 }, idx{ 0, 2 }, idx{ 0, 3 }, idx{ 1, 0 }, idx{ 1, 1 }, idx{ 1, 2 }, idx{ 1, 3 }
  };
  using citer_idx = std::vector<idx>::const_iterator;
  citer_idx iter2 = expected.begin();
  for (auto iter = bnd2D.begin(); iter != bnd2D.end(); ++iter, ++iter2) { REQUIRE(*iter == *iter2); }

  iterator it(bnd2D);
  ++it;
  idx test{ 0, 1 };
  REQUIRE(test == *it);
  // REQUIRE(it - bnd2D.begin() == 1);
  test = idx{ 1, 0 };
  iterator it5(bnd2D, test);
  REQUIRE(*it5 == test);
  // REQUIRE(it5 - bnd2D.begin() == 4);//-V112
  --it5;
  test = idx{ 0, 3 };
  REQUIRE(*it5 == test);
  // REQUIRE(it5 - bnd2D.begin() == 3);


  // idx res;
  // idx expectedidx = { 1, 1 };
  // poutre::details::get_coord_from_offset_nostride<bnd, idx>::op(bnd2D, 5, res);
  // REQUIRE(res == expectedidx);

  // auto off = poutre::details::get_offset_from_coord_nostride<bnd, idx>::op(bnd2D, res);
  // REQUIRE(off == 5);

  // idx outofbound = { 15, 15 };
  // iterator itFromOut(bnd2D, outofbound);
  // REQUIRE(itFromOut == bnd2D.end());


  auto b = bnd{ 3, 2 };
  auto it2d = iterator(b, idx{ 0, 0 });
  // std::vector<idx> expected = {{0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}};
  REQUIRE(*it2d == (idx{ 0, 0 }));
  ++it2d;
  REQUIRE(*it2d == (idx{ 0, 1 }));
  ++it2d;
  REQUIRE(*it2d == idx{ 1, 0 });
  ++it2d;
  REQUIRE(*it2d == idx{ 1, 1 });
  ++it2d;
  REQUIRE(*it2d == idx{ 2, 0 });
  ++it2d;
  REQUIRE(*it2d == idx{ 2, 1 });
}

TEST_CASE("bound iter 3d", "[coord]")
{
  using idx = poutre::details::av::index<3>;
  using bound = poutre::details::av::bounds<3>;
  using iterator = poutre::details::av::bounds_iterator<3>;

  const bound bnd{ 1, 2, 3 };


  std::vector<idx> expected = {
    idx{ 0, 0, 0 }, idx{ 0, 0, 1 }, idx{ 0, 0, 2 }, idx{ 0, 1, 0 }, idx{ 0, 1, 1 }, idx{ 0, 1, 2 }
  };

  using citer_idx = std::vector<idx>::const_iterator;
  citer_idx iter2 = expected.begin();
  for (auto iter = bnd.begin(); iter != bnd.end(); ++iter, ++iter2) { REQUIRE(*iter == *iter2); }

  iterator it(bnd);
  ++it;
  idx expectedidx{ 0, 0, 1 };
  REQUIRE(expectedidx == *it);
  // REQUIRE(it - bnd.begin() == 1);
  idx start{ 0, 1, 1 };
  iterator it2(bnd, start);
  REQUIRE(*it2 == start);
  // REQUIRE(it2 - bnd.begin() == 4);//-V112
  --it2;
  expectedidx = idx{ 0, 1, 0 };
  REQUIRE(*it2 == expectedidx);
  // REQUIRE(it2 - bnd.begin() == 3);


  // idx res;// NOLINT(misc-const-correctness)
  // const idx expidx = idx{ 0, 1, 2 };
  // poutre::details::get_coord_from_offset_nostride<bound, idx>::op(bnd, 5, res);
  // REQUIRE(res == expidx);

  // auto off = poutre::details::get_offset_from_coord_nostride<bound, idx>::op(bnd, res);
  // REQUIRE(off == 5);

  // const idx outofbound = { 15, 15, 15 };
  // iterator itFromOut(bnd, outofbound);
  // REQUIRE(itFromOut == bnd.end());
}

TEST_CASE("bound reverse iter 1d", "[coord]")
{
  using idx = poutre::details::av::index<1>;
  using bnd = poutre::details::av::bounds<1>;
  // using iterator = poutre::details::av::bounds_iterator<1>;

  bnd bnd1D = { 10 };

  std::vector<idx> expected = { { 0 }, { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 }, { 7 }, { 8 }, { 9 } };
  // TODO ?
  // std::vector<idx> rexpected = { {9}, {8}, {7}, {6}, {5}, {4}, {3}, {2}, {1}, {0} };
  //using citer = std::vector<idx>::const_reverse_iterator;
  auto iter2 = expected.rbegin();
  for (auto iter = bnd1D.rbegin(); iter != bnd1D.rend(); ++iter, ++iter2) {
    REQUIRE(*iter == *iter2);
  }
}

TEST_CASE("bound reverse iter 2d", "[coord]")
{
  using bnd = poutre::details::av::bounds<2>;
  using idx = poutre::details::av::index<2>;

  bnd bnd2D{ 2, 4 };
  std::vector<idx> expected = {
    idx{ 0, 0 }, idx{ 0, 1 }, idx{ 0, 2 }, idx{ 0, 3 }, idx{ 1, 0 }, idx{ 1, 1 }, idx{ 1, 2 }, idx{ 1, 3 }
  };
  auto iter2 = expected.rbegin();
  for (auto iter = bnd2D.rbegin(); iter != bnd2D.rend(); ++iter, ++iter2) { REQUIRE(*iter == *iter2); }
}

TEST_CASE("bound reverse iter 3d", "[coord]")
{
  using idx = poutre::details::av::index<3>;
  using bound = poutre::details::av::bounds<3>;

  const bound bnd{ 1, 2, 3 };


  std::vector<idx> expected = {
    idx{ 0, 0, 0 }, idx{ 0, 0, 1 }, idx{ 0, 0, 2 }, idx{ 0, 1, 0 }, idx{ 0, 1, 1 }, idx{ 0, 1, 2 }
  };
  auto iter2 = expected.rbegin();
  auto bditer=  rbegin(bnd);
  auto bditerend =  rend(bnd);

  for (; bditer!= bditerend; ++bditer, ++iter2) { REQUIRE(*bditer == *iter2); }
}

// NOLINTEND