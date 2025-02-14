// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <mutex>
#include <set>
#include <thread>

#include <poutre/base/details/threading/poutreThreading.hpp>

#include <catch2/catch_test_macros.hpp>

#ifndef POUTRE_CI// tests hang on TRAVIS

// NOLINTBEGIN

TEST_CASE("ctor", "[threading]")
{
  poutre::thread::TreadPool pool;
  REQUIRE(pool.capacity() == std::thread::hardware_concurrency() - 2);
}

TEST_CASE("tread pool ctor", "[threading]")
{
  poutre::thread::TreadPool pool(0);
  REQUIRE(pool.capacity() == 1);
}

TEST_CASE("tread pool more thread", "[threading]")
{

  using namespace std::chrono_literals;
  const size_t THREAD_COUNT{ 2U };
  const size_t TASK_COUNT{ 20U };

  std::mutex mutex;
  size_t result = 0u;
  std::set<std::thread::id> thread_ids;

  poutre::thread::TreadPool pool{ THREAD_COUNT };
  std::vector<poutre::thread::TaskFuture<void>> futures;
  for (size_t i = 0; i < TASK_COUNT; ++i) {
    futures.push_back(pool.submit([&] {
      std::this_thread::sleep_for(1ms);
      std::lock_guard<std::mutex> l{ mutex };
      ++result;
      thread_ids.emplace(std::this_thread::get_id());
    }));
  }
  for (auto &fut : futures) { fut.get(); }
  // EXPECT_EQ(pool.empty(), true);
  REQUIRE(result == TASK_COUNT);
  REQUIRE(thread_ids.size() == THREAD_COUNT);
}

TEST_CASE("tread pool various task", "[threading]")
{

  using namespace poutre::thread;
  TreadPool pool{ 2U };
  auto futi = pool.submit([] { return 42U; });
  auto futs = pool.submit([] { return std::string{ "42" }; });
  REQUIRE(futi.get() == 42);
  REQUIRE(futs.get() == std::string{ "42" });
}

TEST_CASE("tread pool lambdas", "[threading]")
{

  using namespace poutre::thread;
  using namespace std::chrono_literals;
  const size_t TASK_COUNT{ 4u };//-V112
  std::vector<poutre::thread::TaskFuture<size_t>> v;
  TreadPool pool{ 4u };
  for (size_t i = 0; i < TASK_COUNT; ++i) {
    v.push_back(pool.submit([task_num = i] {
      std::this_thread::sleep_for(1ms);
      return task_num;
    }));
  }
  for (size_t i = 0; i < TASK_COUNT; ++i) { REQUIRE(i == v[i].get()); }
  // EXPECT_EQ(pool.empty(), true);
}

TEST_CASE("tread pool exception", "[threading]")
{

  using namespace poutre::thread;
  TreadPool pool{ 1u };
  auto f = pool.submit([] { throw std::runtime_error{ "Error" }; });

  REQUIRE_THROWS_AS(f.get(), std::runtime_error);
}

TEST_CASE("tread pool capacity", "[threading]")
{

  using namespace poutre::thread;
  TreadPool pool{ 1U };
  REQUIRE(pool.capacity() == 1U);
}

TEST_CASE("tread pool empty", "[threading]")
{
  using namespace std::chrono_literals;
  using namespace poutre::thread;
  TreadPool pool{ 4U };
  std::this_thread::sleep_for(1s);
}

static int sum(int a, int b) { return a + b; }

TEST_CASE("tread pool func with args", "[threading]")
{
  using namespace std::chrono_literals;
  using namespace poutre::thread;
  TreadPool pool{ 4U };
  auto fut = pool.submit(sum, 2, 2);
  REQUIRE(fut.get() == 4);//-V112
}

static std::thread::id test_function(size_t delay)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(delay + 1));
  return std::this_thread::get_id();
}

TEST_CASE("tread pool reused thread", "[threading]")
{

  using namespace std::chrono_literals;
  using namespace poutre::thread;

  const size_t THREAD_COUNT{ 2U };
  TreadPool pool{ THREAD_COUNT };
  std::vector<poutre::thread::TaskFuture<std::thread::id>> futures;
  std::set<std::thread::id> thread_ids;

  for (size_t i = 0U; i < THREAD_COUNT; ++i) { futures.push_back(pool.submit(test_function, i)); }

  for (size_t i = 0U; i < THREAD_COUNT; ++i) {
    auto res = futures[i].get();
    auto iter = thread_ids.insert(res);
    // New thread is used
    REQUIRE(iter.second == true);
  }

  futures.clear();

  for (size_t i = 0U; i < THREAD_COUNT; ++i) { futures.push_back(pool.submit(test_function, i)); }

  for (size_t i = 0U; i < THREAD_COUNT; ++i) {
    auto res = futures[i].get();
    auto iter = thread_ids.find(res);
    REQUIRE_FALSE(iter == thread_ids.end());
    thread_ids.erase(iter);
  }
}
// NOLINTEND

#endif