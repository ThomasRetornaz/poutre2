/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

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
