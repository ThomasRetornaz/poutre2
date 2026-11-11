/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#include <memory>
#include <poutre/base/details/data_structures/tufa.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("basic", "[tufa]") { 
    constexpr std::size_t size = 5;     
    poutre::details::UnionFind tufa(size); 
    REQUIRE(tufa.find(0) == 0);
    REQUIRE(tufa.find(1) == 1);
    REQUIRE(tufa.find(2) == 2);
    REQUIRE(tufa.find(3) == 3);
    REQUIRE(tufa.find(4) == 4);

    REQUIRE(tufa.unite(1, 2) == true);
    REQUIRE(tufa.find(1) == tufa.find(2));
    REQUIRE(tufa.unite(3, 4) == true);
    REQUIRE(tufa.find(3) == tufa.find(4));
    REQUIRE(tufa.unite(2, 3) == true);
    REQUIRE(tufa.find(1) == tufa.find(4));  
}
