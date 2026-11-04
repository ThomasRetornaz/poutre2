// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

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
