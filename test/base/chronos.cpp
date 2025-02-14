// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <chrono>
#include <memory>
#include <poutre/base/chronos.hpp>
#include <thread>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("basic timer", "[timer]")
{
  const poutre::Timer timer;
  REQUIRE(timer.GetMeanTime() == 0);
  REQUIRE(timer.GetCumulativeTime() == 0);
  REQUIRE(timer.NbIter() == 0);
  // std::cout <<"\n"<< timer<<"\n";
}

TEST_CASE("scoped timer", "[timer]")
{
  std::shared_ptr<poutre::Timer> timer = std::make_shared<poutre::Timer>();
  const std::chrono::milliseconds dura(100);
  for (std::size_t i = 0; i < 10; i++) {// NOLINT
    poutre::ScopedTimer stimer(timer);// NOLINT
    std::this_thread::sleep_for(dura);
  }
  /*EXPECT_NEAR(timer.GetMeanTime(), 100.f, 2);
  EXPECT_NEAR(timer.GetCumulativeTime(), 1000.f, 2);*/
  REQUIRE(timer->NbIter() == 10);
  // std::cout << "\n" << timer << "\n";
}
