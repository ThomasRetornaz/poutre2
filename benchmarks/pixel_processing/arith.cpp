/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "benchmark/benchmark.h"
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/pixel_processing/details/arith_op_t.hpp>
#include <vector>
// #include <iostream>
#include <cmath>

// NOLINTBEGIN

namespace {

decltype(auto) ConstructVector(std::ptrdiff_t size)
{
  std::vector<poutre::pINT32> m_vect(static_cast<std::size_t>(size));
  return m_vect;
}


}// namespace

class ArithmeticsFixture : public ::benchmark::Fixture
{
public:
  void SetUp(const ::benchmark::State &state) override
  {
    m_vect_in = ConstructVector(state.range(0));
    m_vect_out = ConstructVector(state.range(0));
  }
  void TearDown(const ::benchmark::State &) override
  {
    m_vect_in.clear();
    m_vect_out.clear();
  }
  std::vector<poutre::pINT32> m_vect_in;
  std::vector<poutre::pINT32> m_vect_out;
};

// cppcheck-suppress unknownMacro
BENCHMARK_DEFINE_F(ArithmeticsFixture, Sup)(benchmark::State &state)
{
  const auto size = state.range(0);
  std::ptrdiff_t sizeextent = static_cast<std::ptrdiff_t>(std::sqrt(size));
  for (auto _ : state) {
    auto view2din = poutre::details::av::array_view<poutre::pINT32, 2>(m_vect_in, { sizeextent, sizeextent });
    auto view2dout = poutre::details::av::array_view<poutre::pINT32, 2>(m_vect_out, { sizeextent, sizeextent });
    poutre::details::t_ArithSup(view2din, view2din, view2dout);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

// cppcheck-suppress unknownMacro
BENCHMARK_DEFINE_F(ArithmeticsFixture, SupSection)(benchmark::State &state)
{
  const auto size = state.range(0);
  std::ptrdiff_t sizeextent = static_cast<std::ptrdiff_t>(std::sqrt(size));
  for (auto _ : state) {
    auto view2din = poutre::details::av::array_view<poutre::pINT32, 2>(m_vect_in, { sizeextent, sizeextent });
    auto view2dout = poutre::details::av::array_view<poutre::pINT32, 2>(m_vect_out, { sizeextent, sizeextent });
    auto stridedview2dout = view2dout.section({ 0, 0 });
    poutre::details::t_ArithSup(view2din, view2din, stridedview2dout);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

// cppcheck-suppress unknownMacro
BENCHMARK_DEFINE_F(ArithmeticsFixture, AddConst)(benchmark::State &state)
{
  const auto size = state.range(0);
  std::ptrdiff_t sizeextent = static_cast<std::ptrdiff_t>(std::sqrt(size));
  for (auto _ : state) {
    auto view2din = poutre::details::av::array_view<poutre::pINT32, 2>(m_vect_in, { sizeextent, sizeextent });
    auto view2dout = poutre::details::av::array_view<poutre::pINT32, 2>(m_vect_out, { sizeextent, sizeextent });
    poutre::details::t_ArithSaturatedAddConstant(view2din, 10, view2dout);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

// cppcheck-suppress unknownMacro
BENCHMARK_REGISTER_F(ArithmeticsFixture, Sup)
  ->Arg(16 * 16)
  ->Arg(32 * 32)//-V112
  ->Arg(64 * 64)
  ->Arg(128 * 128)
  ->Arg(256 * 256)
  ->Arg(512 * 512)
  ->Arg(1024 * 1024);//->Unit(benchmark::kMillisecond); //-V112

BENCHMARK_REGISTER_F(ArithmeticsFixture, SupSection)
  ->Arg(16 * 16)
  ->Arg(32 * 32)//-V112
  ->Arg(64 * 64)
  ->Arg(128 * 128)
  ->Arg(256 * 256)
  ->Arg(512 * 512)
  ->Arg(1024 * 1024);//->Unit(benchmark::kMillisecond); //-V112

BENCHMARK_REGISTER_F(ArithmeticsFixture, AddConst)
  ->Arg(16 * 16)
  ->Arg(32 * 32)//-V112
  ->Arg(64 * 64)
  ->Arg(128 * 128)
  ->Arg(256 * 256)
  ->Arg(512 * 512)
  ->Arg(1024 * 1024);//->Unit(benchmark::kMillisecond); //-V112

// NOLINTEND