
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   simd_helpers.hpp
 * @author Thomas Retornaz
 * @brief  Add STL like algorithm on top of xsimd
 *
 *
 */

#include <cstddef>
#include <memory>
#include <poutre/base/config.hpp>
#include <poutre/base/types.hpp>
#include <vector>

#if defined(POUTRE_IS_GCC) || defined(POUTRE_IS_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#if defined(POUTRE_IS_CLANG)
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#elif defined(POUTRE_IS_GCC) 
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#endif
#include <xsimd/xsimd.hpp>
#if defined(POUTRE_IS_GCC) || defined(POUTRE_IS_CLANG)
#pragma GCC diagnostic pop
#endif

namespace xs = xsimd;

const POUTRE_CONSTEXPR size_t SIMD_IDEAL_MAX_ALIGN_BYTES = xsimd::default_arch::alignment();

namespace poutre::simd {
/**
 * @addtogroup simd_group SIMD facilities
 * @ingroup poutre_base_group
 *@{
 */
POUTRE_ALWAYS_INLINE bool IsAligned(const void *ptr,
  std::size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES)
{
  // NOLINTBEGIN
  POUTRE_ASSERTCHECK(((alignment & (alignment - 1)) == 0), "bad alignment value");
  return (reinterpret_cast<std::size_t>(ptr) & (alignment - 1)) == 0;// from boost\align\detail\is_aligned.hpp
  // NOLINTEND
}

POUTRE_ALWAYS_INLINE bool IsAligned(std::size_t val,
  std::size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES) POUTRE_NOEXCEPTONLYNDEBUG
{
  POUTRE_ASSERTCHECK(((alignment & (alignment - 1)) == 0), "bad alignment value");
  return (val & (alignment - 1)) == 0;// from boost\align\detail\is_aligned.hpp
}

template<typename T>
POUTRE_ALWAYS_INLINE T *t_ReachNextAligned(T *ptr,
  std::size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES) POUTRE_NOEXCEPTONLYNDEBUG
{
  POUTRE_ASSERTCHECK(((alignment & (alignment - 1)) == 0), "bad alignment value");
  // NOLINTBEGIN
  return reinterpret_cast<T *>(
    (reinterpret_cast<std::size_t>(ptr) + alignment - 1) & ~(alignment - 1));// from boost\align\detail\align_up.hpp
  // NOLINTEND
}

template<typename T> size_t t_ReachNextAlignedSize(size_t size)
{
  // compute required padding, reach the next multiple aligned
  size_t padd = SIMD_IDEAL_MAX_ALIGN_BYTES / sizeof(T);
  // have a look at
  // http://stackoverflow.com/questions/227897/solve-the-memory-alignment-in-c-interview-question-that-stumped-me
  // for explanation
  return ((size + padd - 1) & ~(padd - 1));// from boost\align\detail\align_up.hpp
}

/**
Extract from contigous range [first,last[
The two loop counter
-the scalar prologue [start,size_prologue_loop[ i.e the range defined between
the original begin and the first location to be properly aligned to be used
through simd operators
- the main simd_loop_part,[size_prologue_loop,size_simd_loop[ i.e the range
where we could apply simd operators
- Note epilogue equals [size_simd_loop,stop[
*/
template<typename T>
const std::pair<ptrdiff_t, ptrdiff_t> POUTRE_ALWAYS_INLINE t_SIMDInputRange(const T *first,
  const T *last) POUTRE_NOEXCEPTONLYNDEBUG // TODO bench this tricks with erode/dilate
{
  POUTRE_ASSERTCHECK(first, "null ptr");
  POUTRE_ASSERTCHECK(last, "null ptr");
  const ptrdiff_t simd_size = xs::simd_type<T>::size;
  const ptrdiff_t size = last - first;
  // get aligned address from first
  const T *ptr_aligned_first = t_ReachNextAligned(first, SIMD_IDEAL_MAX_ALIGN_BYTES);
  // Next aligned address may be out of range, so make sure size_prologue_loop
  // is not bigger than size
  const ptrdiff_t size_prologue_loop = std::min(size, std::distance(first, ptr_aligned_first));
  const ptrdiff_t size_simd_loop =
  (size >= size_prologue_loop) ? (simd_size * ((size - size_prologue_loop) / simd_size)) : (0u);

  return std::make_pair(size_prologue_loop, size_simd_loop);
}

template<typename T> decltype(auto) t_ExpandAligned(scoord ySize, scoord xSize) POUTRE_NOEXCEPT
{
  return std::vector<T, xs::aligned_allocator<T, SIMD_IDEAL_MAX_ALIGN_BYTES>>(ySize * t_ReachNextAlignedSize<T>(xSize));
}
//! @} doxygroup: simd_group
}// namespace poutre::simd
