// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutre/structuring_element/details/neighbor_list_se_t.hpp>
#include <poutre/structuring_element/predefined_nl_se.hpp>
// #include <poutre/base/details/data_structures/array_view.hpp>

namespace poutre::se {

const details::neighbor_list_t< 1 > SESegmentX1D({ // NOLINT
  { 0 },
  { -1 },
  { +1 },
});

const details::neighbor_list_t< 2 > SESegmentX2D({ // NOLINT
  { 0,0 },
  { 0,-1 },
  { 0,+1 },
});

const details::neighbor_list_t< 2 > SESegmentY2D({ // NOLINT
  { 0,0 },
  { -1,0 },
  { +1, 0 },
});

const details::neighbor_list_t< 2 > SESquare2D({ // NOLINT
  { 0,0 },
{-1, -1}, {+0, -1}, {+1, -1},
{-1, +0}, {+1, +0},
{-1, +1}, {+0, +1}, {+1, +1}
});


const details::neighbor_list_t< 2 > SECross2D({ // NOLINT
  { 0,0 },{+0, -1},
{-1, +0}, {+1, +0}, {+0, +1},
});

const details::neighbor_list_t< 3 > SECross3D({ // NOLINT
  { 0,0,0 },
  {+0, +0, -1},
  {+0, -1, +0},
  {-1, +0, +0},
  {+1, +0, +0},
  {+0, +1, +0},
  {+0, +0, +1}
});

const details::neighbor_list_t< 3 > SESquare3D({ // NOLINT
  { 0,0,0 },
{-1, -1, -1}, {+0, -1, -1}, {+1, -1, -1},
{-1, +0, -1}, {+0, +0, -1}, {+1, +0, -1},
{-1, +1, -1}, {+0, +1, -1}, {+1, +1, -1},
// ----
{-1, -1, +0}, {+0, -1, +0}, {+1, -1, +0},
{-1, +0, +0},               {+1, +0, +0},
{-1, +1, +0}, {+0, +1, +0}, {+1, +1, +0},
// ----
{-1, -1, +1}, {+0, -1, +1}, {+1, -1, +1},
{-1, +0, +1}, {+0, +0, +1}, {+1, +0, +1},
{-1, +1, +1}, {+0, +1, +1}, {+1, +1, +1},
  });

const details::neighbor_list_t< 3 > SESegmentX3D({ // NOLINT
  { 0, 0, 0 },
{ 0, 0, -1 },
{ 0, 0,+1 },
  });

const details::neighbor_list_t< 3 > SESegmentY3D({ // NOLINT
  { 0, 0, 0 },
{ 0, -1, 0 },
{ 0, +1, 0 },
  });

const details::neighbor_list_t< 3 > SESegmentZ3D({ // NOLINT
  { 0, 0, 0 },
{ -1, 0, 0 },
{ +1, 0, 0 },
  });
}
