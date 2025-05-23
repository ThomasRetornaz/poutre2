// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <cstdint>
#include <poutre/base/details/data_structures/pq.hpp>
#include <poutre/base/types.hpp>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

// TEST_CASE("increase nostable", "[pqueue]")
// {
//   poutre::details::poutre_pq<poutre::pINT32, uint64_t> pqueue;
//   // NOLINTBEGIN
//   pqueue.emplace(0, 1);//-V525
//   pqueue.emplace(50, 1);
//   pqueue.emplace(0, 2);
//   pqueue.emplace(50, 2);
//   pqueue.emplace(80, 1);
//   pqueue.emplace(80, 2);
//   pqueue.emplace(80, 3);
//   pqueue.emplace(0, 3);
//   // NOLINTEND

//   const std::vector<std::pair<poutre::pINT32, uint64_t>> expected = {
//     { 80, 1 }, { 80, 2 }, { 80, 3 }, { 50, 1 }, { 50, 2 }, { 0, 2 }, { 0, 1 }, { 0, 3 }// NOLINT
//   };
//   std::vector<std::pair<poutre::pINT32, uint64_t>> results;
//   while (!pqueue.empty()) {
//     results.push_back(pqueue.top());
//     pqueue.pop();
//   }
//   REQUIRE(results.size() == expected.size());
//   auto iterres = results.cbegin();
//   auto iterexpected = expected.cbegin();
//   for (; iterres != results.cend(); ++iterres, ++iterexpected) { REQUIRE(*iterexpected == *iterres); }
// }

TEST_CASE("increase stable", "[pqueue]")
{
  poutre::details::poutre_pq_stable<poutre::pINT32, uint64_t> pqueue;
  // NOLINTBEGIN
  pqueue.emplace(0, 1);//-V525
  pqueue.emplace(50, 1);
  pqueue.emplace(0, 2);
  pqueue.emplace(50, 2);
  pqueue.emplace(80, 1);
  pqueue.emplace(80, 2);
  pqueue.emplace(80, 3);
  pqueue.emplace(0, 3);

  const std::vector<std::pair<poutre::pINT32, uint64_t>> expected = {
    { 80, 1 }, { 80, 2 }, { 80, 3 }, { 50, 1 }, { 50, 2 }, { 0, 1 }, { 0, 2 }, { 0, 3 }
  };
  // NOLINTEND
  std::vector<std::pair<poutre::pINT32, uint64_t>> results;
  while (!pqueue.empty()) {
    results.push_back(pqueue.top());
    pqueue.pop();
  }
  REQUIRE(results.size() == expected.size());
  auto iterres = results.cbegin();
  auto iterexpected = expected.cbegin();
  for (; iterres != results.cend(); ++iterres, ++iterexpected) { REQUIRE(*iterexpected == *iterres); }
}

// TEST_CASE("decrease no stable", "[pqueue]")
// {
//   poutre::details::poutre_rpq<poutre::pINT32, uint64_t> pqueue;
//   // NOLINTBEGIN
//   pqueue.emplace(0, 1);//-V525
//   pqueue.emplace(50, 1);
//   pqueue.emplace(0, 2);
//   pqueue.emplace(50, 2);
//   pqueue.emplace(80, 1);
//   pqueue.emplace(80, 2);
//   pqueue.emplace(80, 3);
//   pqueue.emplace(0, 3);

//   const std::vector<std::pair<poutre::pINT32, uint64_t>> expected = {
//     { 0, 1 }, { 0, 2 }, { 0, 3 }, { 50, 2 }, { 50, 1 }, { 80, 2 }, { 80, 3 }, { 80, 1 }
//   };
//   // NOLINTEND
//   std::vector<std::pair<poutre::pINT32, uint64_t>> results;
//   while (!pqueue.empty()) {
//     results.push_back(pqueue.top());
//     pqueue.pop();
//   }
//   REQUIRE(results.size() == expected.size());
//   auto iterres = results.cbegin();
//   auto iterexpected = expected.cbegin();
//   for (; iterres != results.cend(); ++iterres, ++iterexpected) { REQUIRE(*iterexpected == *iterres); }
// }

TEST_CASE("decrease stable", "[pqueue]")
{
  poutre::details::poutre_rpq_stable<poutre::pINT32, uint64_t> pqueue;
  // NOLINTBEGIN
  pqueue.emplace(0, 1);//-V525
  pqueue.emplace(50, 1);
  pqueue.emplace(0, 2);
  pqueue.emplace(50, 2);
  pqueue.emplace(80, 1);
  pqueue.emplace(80, 2);
  pqueue.emplace(80, 3);
  pqueue.emplace(0, 3);
  const std::vector<std::pair<poutre::pINT32, uint64_t>> expected = {
    { 0, 1 }, { 0, 2 }, { 0, 3 }, { 50, 1 }, { 50, 2 }, { 80, 1 }, { 80, 2 }, { 80, 3 }
  };
  // NOLINTEND
  std::vector<std::pair<poutre::pINT32, uint64_t>> results;
  while (!pqueue.empty()) {
    results.push_back(pqueue.top());
    pqueue.pop();
  }
  REQUIRE(results.size() == expected.size());
  auto iterres = results.cbegin();
  auto iterexpected = expected.cbegin();
  for (; iterres != results.cend(); ++iterres, ++iterexpected) { REQUIRE(*iterexpected == *iterres); }
}