/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include <catch2/catch_test_macros.hpp>

TEST_CASE("dummy", "[io]")
{
  const bool dummy = true;
  REQUIRE(dummy == true);
}