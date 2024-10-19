
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   poutreSimdAlgorithm.hpp
 * @author Thomas Retornaz
 * @brief  Add STL like algorithm on top of nsimd
 *
 *
 */

#include <poutre/base/poutreConfig.hpp>
#include <poutre/base/poutreTypes.hpp>
#include <vector>

#include <xsimd/xsimd.hpp>

namespace xs = xsimd;

const POUTRE_CONSTEXPR size_t SIMD_IDEAL_MAX_ALIGN_BYTES = xsimd::default_arch::alignment();

#if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX512_VERSION
const POUTRE_CONSTEXPR int SIMD_BATCH_INT8_SIZE = 64;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT16_SIZE = 32;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT32_SIZE = 16;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT64_SIZE = 8;
const POUTRE_CONSTEXPR int SIMD_BATCH_FLOAT_SIZE = 16;
const POUTRE_CONSTEXPR int SIMD_BATCH_DOUBLE_SIZE = 8;
#elif XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX_VERSION
const POUTRE_CONSTEXPR int SIMD_BATCH_INT8_SIZE = 32;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT16_SIZE = 16;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT32_SIZE = 8;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT64_SIZE = 4;
const POUTRE_CONSTEXPR int SIMD_BATCH_FLOAT_SIZE = 8;
const POUTRE_CONSTEXPR int SIMD_BATCH_DOUBLE_SIZE = 4;
#elif XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE2_VERSION
const POUTRE_CONSTEXPR int SIMD_BATCH_INT8_SIZE = 16;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT16_SIZE = 8;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT32_SIZE = 4;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT64_SIZE = 2;
const POUTRE_CONSTEXPR int SIMD_BATCH_FLOAT_SIZE = 4;
const POUTRE_CONSTEXPR int SIMD_BATCH_DOUBLE_SIZE = 2;
#elif XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
const POUTRE_CONSTEXPR int SIMD_BATCH_INT16_SIZE = 16;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT32_SIZE = 8;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT64_SIZE = 4;
const POUTRE_CONSTEXPR int SIMD_BATCH_FLOAT_SIZE = 8;
const POUTRE_CONSTEXPR int SIMD_BATCH_DOUBLE_SIZE = 4;
#elif XSIMD_ARM_INSTR_SET >= XSIMD_ARM7_NEON_VERSION
const POUTRE_CONSTEXPR int SIMD_BATCH_INT8_SIZE = 16;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT16_SIZE = 8;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT32_SIZE = 4;
const POUTRE_CONSTEXPR int SIMD_BATCH_INT64_SIZE = 2;
const POUTRE_CONSTEXPR int SIMD_BATCH_FLOAT_SIZE = 4;
const POUTRE_CONSTEXPR int SIMD_BATCH_DOUBLE_SIZE = 2;
#else
#error "Unknow ideal alignment for current architecture"
#endif


namespace poutre::simd {
/**
 * @addtogroup simd_group SIMD facilities
 * @ingroup poutre_base_group
 *@{
 */
POUTRE_ALWAYS_INLINE bool IsAligned(const void *ptr,
  std::size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES)// NSIMD_MAX_ALIGNMENT)
                                                     // POUTRE_NOEXCEPTONLYNDEBUG
{
  // NOLINTBEGIN
  POUTRE_ASSERTCHECK(((alignment & (alignment - 1)) == 0), "bad alignment value");
  return ((std::size_t)ptr & (alignment - 1)) == 0;// from boost\align\detail\is_aligned.hpp //NOLINT
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
    ((std::size_t)ptr + alignment - 1) & ~(alignment - 1));// from boost\align\detail\align_up.hpp
  // NOLINTEND
}

template<typename T> size_t t_ReachNextAlignedSize(size_t size)
{
  // compute required padding, reach the next multiple aligned
  size_t padd = SIMD_IDEAL_MAX_ALIGN_BYTES / sizeof(T);
  // have a look at
  // http://stackoverflow.com/questions/227897/solve-the-memory-alignment-in-c-interview-question-that-stumped-me
  // for explanation
  return ((static_cast<size_t>(size) + padd - 1) & ~(padd - 1));// from boost\align\detail\align_up.hpp
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
  const T *last) POUTRE_NOEXCEPTONLYNDEBUG
{
  POUTRE_ASSERTCHECK(first, "null ptr");
  POUTRE_ASSERTCHECK(last, "null ptr");
  const auto simd_size = xs::simd_type<T>::size;
  const auto size = last - first;
  // get aligned adress from first
  const T *ptr_aligned_first = t_ReachNextAligned(first, SIMD_IDEAL_MAX_ALIGN_BYTES);
  // Next aligned address may be out of range, so make sure size_prologue_loop
  // is not bigger than size
  const auto size_prologue_loop = std::min(size, std::distance(first, ptr_aligned_first));
  const auto size_simd_loop =
    (size >= size_prologue_loop) ? (simd_size * ((size - size_prologue_loop) / simd_size)) : (0u);

  return std::make_pair(size_prologue_loop, size_simd_loop);
}

template<typename T> decltype(auto) t_ExpandAligned(scoord ySize, scoord xSize) POUTRE_NOEXCEPT
{
  return std::vector<T, xs::aligned_allocator<T, SIMD_IDEAL_MAX_ALIGN_BYTES>>(ySize * t_ReachNextAlignedSize<T>(xSize));
}
//! @} doxygroup: simd_group
}// namespace poutre::simd
