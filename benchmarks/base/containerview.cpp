// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "benchmark/benchmark.h"
#include <poutre/base/details/data_structures/array_view.hpp>

#include <cstdlib>
#include <vector>
// #include <iostream>
#include <cmath>

// NOLINTBEGIN

namespace {

decltype(auto) ConstructVector(std::ptrdiff_t size)
{
  std::vector<unsigned int> m_vect(static_cast<std::size_t>(size));
  // m_vect.reserve(size);
  /*for (auto i = 0; i < size; ++i) {
      m_vect.push_back(rand() % size);
  }*/
  return m_vect;
}

}// namespace

class ViewOnVect1DFixture : public ::benchmark::Fixture
{
public:
  void SetUp(const ::benchmark::State &state) override { m_vect = ConstructVector(state.range(0)); }
  void TearDown(const ::benchmark::State &) override { m_vect.clear(); }
  std::vector<unsigned int> m_vect;
};

// 2 to 3 times slower than common iterator
// cppcheck-suppress unknownMacro
BENCHMARK_DEFINE_F(ViewOnVect1DFixture, BoundIterator)(benchmark::State &state)
{
  const auto size = state.range(0);
  for (auto sta : state) {
    // benchmark::DoNotOptimize(m_vect[i]);
    auto view = poutre::details::av::array_view<unsigned int, 1>(m_vect);
    auto bnd = view.bound();
    auto it = bnd.begin();
    auto itend = bnd.end();
    for (; it != itend; ++it) {
      benchmark::DoNotOptimize(view[*it]);
      // auto val= view[*it];
    }
  }
  state.SetItemsProcessed(state.iterations() * size);
}

// 12 to 15 times slower
// cppcheck-suppress unknownMacro
BENCHMARK_DEFINE_F(ViewOnVect1DFixture, BoundIterator2d)(benchmark::State &state)
{
  const auto size = state.range(0);
  const ptrdiff_t sqrtsize = static_cast<ptrdiff_t>(std::sqrt(size));
  for (auto sta : state) {
    // benchmark::DoNotOptimize(m_vect[i]);
    auto view2d = poutre::details::av::array_view<unsigned int, 2>(m_vect, { sqrtsize, sqrtsize });
    auto bnd = view2d.bound();
    auto it = bnd.begin();
    auto itend = bnd.end();
    for (; it != itend; ++it) {
      benchmark::DoNotOptimize(view2d[*it]);
      // auto val= view[*it];
    }
  }
  state.SetItemsProcessed(state.iterations() * size);
}

// cppcheck-suppress unknownMacro
BENCHMARK_DEFINE_F(ViewOnVect1DFixture, Iterator)(benchmark::State &state)
{
  const auto size = state.range(0);
  for (auto sta : state) {
    auto iter = m_vect.begin();
    auto itend = m_vect.end();
    for (; iter != itend; ++iter) {
      benchmark::DoNotOptimize(*iter);
      // auto val = *it;
    }
  }
  state.SetItemsProcessed(state.iterations() * size);
}

// cppcheck-suppress unknownMacro
BENCHMARK_REGISTER_F(ViewOnVect1DFixture, Iterator)
  ->Arg(16 * 16)
  ->Arg(32 * 32)//-V112
  ->Arg(64 * 64)
  ->Arg(128 * 128)
  ->Arg(256 * 256);//->Unit(benchmark::kMillisecond); //-V112

// 3 times slower ....
// cppcheck-suppress unknownMacro
BENCHMARK_REGISTER_F(ViewOnVect1DFixture, BoundIterator)
  ->Arg(16 * 16)
  ->Arg(32 * 32)//-V112
  ->Arg(64 * 64)
  ->Arg(128 * 128)
  ->Arg(256 * 256);//->Unit(benchmark::kMillisecond); //-V112

// cppcheck-suppress unknownMacro
BENCHMARK_REGISTER_F(ViewOnVect1DFixture, BoundIterator2d)
  ->Arg(16 * 16)
  ->Arg(32 * 32)//-V112
  ->Arg(64 * 64)
  ->Arg(128 * 128)
  ->Arg(256 * 256);//->Unit(benchmark::kMillisecond); //-V112
// NOLINTEND