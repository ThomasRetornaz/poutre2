/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include <chrono>
#include <ctime>
#include <memory>
#include <ostream>
#include <poutre/base/chronos.hpp>
#include <poutre/base/config.hpp>
#include <sstream>
#include <string>

namespace poutre {

//! Start the timing action
void Timer::Start() POUTRE_NOEXCEPT
{
  m_nbiter++;
  m_start = high_resolution_clock::now();
  m_start_cputime = std::clock();
}
//! Stop the timing action
void Timer::Stop() POUTRE_NOEXCEPT
{
  m_accu += std::chrono::duration_cast< double_milliseconds >(high_resolution_clock::now() - m_start).count();
  m_accu_cputime += std::clock() - m_start_cputime;
}

//! Grab wall time accumulated in ms
Timer::timerep Timer::GetCumulativeTime() const POUTRE_NOEXCEPT { return m_accu; }

//! Grab wall mean time of iteration in ms
Timer::timerep Timer::GetMeanTime() const POUTRE_NOEXCEPT
{
  if (m_nbiter == 0) {
    return static_cast< timerep >(0);
  }
  return GetCumulativeTime() / static_cast< timerep >(m_nbiter);
}

// NOLINTBEGIN
//! Grab cpu time accumulated in ms
Timer::timerep Timer::GetCumulativeCPUTime() const POUTRE_NOEXCEPT
{
  return static_cast< Timer::timerep >(m_accu_cputime) / static_cast< Timer::timerep >(CLOCKS_PER_SEC);
}

//! Grab wall mean time of iteration in ms
Timer::timerep Timer::GetMeanCpuTime() const POUTRE_NOEXCEPT
{
  if (m_nbiter == 0) {
    return static_cast< timerep >(0);
  }
  return GetCumulativeCPUTime() / static_cast< timerep >(m_nbiter);
}
// NOLINTEND

//! Grab number off triggered start
std::size_t Timer::NbIter() const POUTRE_NOEXCEPT { return m_nbiter; }

//! String serialization
std::string Timer::to_str() const
{
  std::stringstream sstr{};
  sstr << "Cumulative wall time in ms: " << this->GetCumulativeTime() << "\n Nb iter " << m_nbiter
       << "\n Mean wall time in ms: " << this->GetMeanTime();
  sstr << "Cumulative cpu time in ms: " << this->GetCumulativeCPUTime() << "\n Nb iter " << m_nbiter
       << "\n Mean cpu time in ms: " << this->GetMeanCpuTime();
  return sstr.str();
}

//! Reset the chrono
void Timer::Reset() POUTRE_NOEXCEPT
{
  m_accu = 0.;
  m_start_cputime = 0;
  m_accu_cputime = 0;
  m_nbiter = 0;
}

// std::ostream & poutre::operator<<(std::ostream &os, Timer& timer)
std::ostream &operator<<(std::ostream &ost, const Timer &timer)
{
  // NOLINTNEXTLINE(bugprone-signed-bitwise)
  ost << timer.to_str();
  return ost;
}

//! ctor
ScopedTimer::ScopedTimer(std::shared_ptr< Timer > &itimer) : m_timer(itimer)
{
  if (m_timer) {
    m_timer->Start();
  }
}
//! dtor
ScopedTimer::~ScopedTimer()
{
  if (m_timer) {
    m_timer->Stop();
  }
}

}// namespace poutre
