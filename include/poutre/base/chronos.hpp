
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   chronos.hpp
 * @author Thomas Retornaz
 * @brief  Define helper objects around std::chrono facilities
 *
 *
 */


#include <poutre/base/base.hpp>

#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>

namespace poutre {
/**
 * @addtogroup timer_group Timing facilities
 * @ingroup poutre_base_group
 *@{
 */

/**
 * @brief Simple Timer class (wrapping std::chronos) with classical
 * start,stop interface and serialisation capabilities
 * @todo think about multi-threading
 */
class BASE_API Timer// see http://en.cppreference.com/w/cpp/chrono/c/clock
{
public:
  using high_resolution_clock = std::chrono::high_resolution_clock;
  using double_milliseconds = std::chrono::duration<double, std::milli>;// switch to other duration through
                                                                        // template<rep,duration> ?
  using timerep = double_milliseconds::rep;// todo use
                                           // decltype(auto)
  //! ctor
  Timer();

  //! Start the timing action
  void Start() POUTRE_NOEXCEPT;

  //! Stop the timing action
  void Stop() POUTRE_NOEXCEPT;

  //! Grab wall time accumulated in ms
  [[nodiscard]] timerep GetCumulativeTime() const POUTRE_NOEXCEPT;

  //! Grab wall mean time of iteration in ms
  [[nodiscard]] timerep GetMeanTime() const POUTRE_NOEXCEPT;

  //! Grab cpu time accumulated in ms
  [[nodiscard]] timerep GetCumulativeCPUTime() const POUTRE_NOEXCEPT;

  //! Grab wall mean time of iteration in ms
  [[nodiscard]] timerep GetMeanCpuTime() const POUTRE_NOEXCEPT;

  //! Grab number off triggered start
  [[nodiscard]] std::size_t NbIter() const POUTRE_NOEXCEPT;

  //! String serialization
  [[nodiscard]] std::string to_str() const POUTRE_NOEXCEPT;

  //! Reset the chrono
  void Reset() POUTRE_NOEXCEPT;

private:
  high_resolution_clock::time_point m_start;//! start wall time
  timerep m_accu;//! accumulate wall time over all iteration
  std::clock_t m_start_cputime;//! start cpu time
  std::clock_t m_accu_cputime;//! accumulate cpu time
  std::size_t m_nbiter;//! nb iteration
};

//! Timer stream serialization
BASE_API std::ostream &operator<<(std::ostream &ost, const Timer &timer);

/**
 * @brief Scoped (RAII) timer, start and stop of embedded timer are automatically triggered
 *
 * @todo think about multi-threading
 */
class BASE_API ScopedTimer
{
private:
  std::shared_ptr<Timer> m_timer;//! Inner timer

public:
  //! ctor
  explicit ScopedTimer(std::shared_ptr<Timer> &itimer);

  ScopedTimer(const ScopedTimer &) = default;
  ScopedTimer &operator=(const ScopedTimer &) = default;
  ScopedTimer(const ScopedTimer &&) = delete;
  ScopedTimer &operator=(const ScopedTimer &&) = delete;
  //! dtor
  ~ScopedTimer();
};
/**
//! @} doxygroup: timer_group
*/
}// namespace poutre

// EXPIMP_TEMPLATE template class BASE_API std::chrono::high_resolution_clock::time_point;
