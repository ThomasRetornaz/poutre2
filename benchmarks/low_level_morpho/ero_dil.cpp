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
#include <poutre/base/types.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/low_level_morpho/details/ero_dil_static_se_t.hpp>
#include <poutre/low_level_morpho/details/ero_dil_runtime_nl_se_t.hpp>
#include <poutre/structuring_element/predefined_nl_se.hpp>
#include <cstdlib>
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

class EroDilFixture : public ::benchmark::Fixture
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
BENCHMARK_DEFINE_F(EroDilFixture, DilateSquare2DStatic)(benchmark::State &state)
{
  const auto size = state.range(0);
  std::ptrdiff_t sizeextent = static_cast<std::ptrdiff_t>(std::sqrt(size));
  for (auto _ : state) {
    auto view2din = poutre::details::av::array_view<const poutre::pINT32, 2>(m_vect_in, { sizeextent, sizeextent});
    auto view2dout = poutre::details::av::array_view<poutre::pINT32, 2>(m_vect_out, { sizeextent, sizeextent });
    poutre::llm::details::t_Dilate(view2din, poutre::se::Common_NL_SE::SESquare2D, view2dout);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(EroDilFixture, DilateSquare3DStatic)(benchmark::State &state)
{
  const auto size = state.range(0);
  std::ptrdiff_t sizeextent = static_cast<std::ptrdiff_t>(std::sqrt(std::sqrt(size)));
  for (auto _ : state) {
    auto view2din = poutre::details::av::array_view<const poutre::pINT32, 3>(m_vect_in, { sizeextent, sizeextent, sizeextent});
    auto view2dout = poutre::details::av::array_view<poutre::pINT32, 3>(m_vect_out, { sizeextent, sizeextent, sizeextent});
    poutre::llm::details::t_Dilate(view2din, poutre::se::Common_NL_SE::SESquare3D, view2dout);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

// cppcheck-suppress unknownMacro
BENCHMARK_DEFINE_F(EroDilFixture, DilateSquare2DRuntime)(benchmark::State &state)
{
  const auto size = state.range(0);
  std::ptrdiff_t sizeextent = static_cast<std::ptrdiff_t>(std::sqrt(size));
  for (auto _ : state) {
    auto view2din = poutre::details::av::array_view<const poutre::pINT32, 2>(m_vect_in, { sizeextent, sizeextent});
    auto view2dout = poutre::details::av::array_view<poutre::pINT32, 2>(m_vect_out, { sizeextent, sizeextent });
    poutre::llm::details::t_Dilate(view2din, poutre::se::SESquare2D, view2dout);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(EroDilFixture, DilateSquare3DRuntime)(benchmark::State &state)
{
  const auto size = state.range(0);
  std::ptrdiff_t sizeextent = static_cast<std::ptrdiff_t>(std::sqrt(std::sqrt(size)));
  for (auto _ : state) {
    auto view2din = poutre::details::av::array_view<const poutre::pINT32, 3>(m_vect_in, { sizeextent, sizeextent, sizeextent});
    auto view2dout = poutre::details::av::array_view<poutre::pINT32, 3>(m_vect_out, { sizeextent, sizeextent, sizeextent});
    poutre::llm::details::t_Dilate(view2din, poutre::se::SESquare3D, view2dout);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

// cppcheck-suppress unknownMacro
BENCHMARK_REGISTER_F(EroDilFixture, DilateSquare2DStatic)
  ->Arg(16 * 16)
  ->Arg(32 * 32)//-V112
  ->Arg(64 * 64)
  ->Arg(128 * 128)
  ->Arg(256 * 256)
  ->Arg(512 * 512)
  ->Arg(1024 * 1024)->Unit(benchmark::kMillisecond); //-V112

// cppcheck-suppress unknownMacro
BENCHMARK_REGISTER_F(EroDilFixture, DilateSquare3DStatic)
  ->Arg(64 * 64 * 64 )
  ->Arg(128 * 128 * 128)
  ->Arg(256 * 256 * 256 )->Unit(benchmark::kMillisecond); //-V112


// cppcheck-suppress unknownMacro
BENCHMARK_REGISTER_F(EroDilFixture, DilateSquare2DRuntime)
  ->Arg(16 * 16)
  ->Arg(32 * 32)//-V112
  ->Arg(64 * 64)
  ->Arg(128 * 128)
  ->Arg(256 * 256)
  ->Arg(512 * 512)
  ->Arg(1024 * 1024)->Unit(benchmark::kMillisecond); //-V112

// cppcheck-suppress unknownMacro
BENCHMARK_REGISTER_F(EroDilFixture, DilateSquare3DRuntime)
  ->Arg(64 * 64 * 64 )
  ->Arg(128 * 128 * 128)
  ->Arg(256 * 256 * 256 )->Unit(benchmark::kMillisecond); //-V112



// NOLINTEND