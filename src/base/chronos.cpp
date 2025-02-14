// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <chrono>
#include <ctime>
#include <memory>
#include <ostream>
#include <poutre/base/chronos.hpp>
#include <poutre/base/config.hpp>
#include <sstream>
#include <string>

namespace poutre {
//! ctor
Timer::Timer() : m_accu(), m_start_cputime(0), m_accu_cputime(0), m_nbiter(0) {}

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
  m_accu += std::chrono::duration_cast<double_milliseconds>(high_resolution_clock::now() - m_start).count();
  m_accu_cputime += std::clock() - m_start_cputime;
}

//! Grab wall time accumulated in ms
Timer::timerep Timer::GetCumulativeTime() const POUTRE_NOEXCEPT { return m_accu; }

//! Grab wall mean time of iteration in ms
Timer::timerep Timer::GetMeanTime() const POUTRE_NOEXCEPT
{
  if (m_nbiter == 0) { return static_cast<timerep>(0); }
  return GetCumulativeTime() / static_cast<timerep>(m_nbiter);
}

//! Grab cpu time accumulated in ms
Timer::timerep Timer::GetCumulativeCPUTime() const POUTRE_NOEXCEPT
{
  return static_cast<double>(m_accu_cputime) / CLOCKS_PER_SEC;
}

//! Grab wall mean time of iteration in ms
Timer::timerep Timer::GetMeanCpuTime() const POUTRE_NOEXCEPT
{
  if (m_nbiter == 0) { return static_cast<timerep>(0); }
  return GetCumulativeCPUTime() / static_cast<timerep>(m_nbiter);
}

//! Grab number off triggered start
std::size_t Timer::NbIter() const POUTRE_NOEXCEPT { return m_nbiter; }

//! String serialization
std::string Timer::to_str() const POUTRE_NOEXCEPT
{
  std::stringstream sstr;
  sstr << "Cumulative wall time in ms: " << GetCumulativeTime() << "\n Nb iter " << m_nbiter
       << "\n Mean wall time in ms: " << GetMeanTime();
  sstr << "Cumulative cpu time in ms: " << GetCumulativeCPUTime() << "\n Nb iter " << m_nbiter
       << "\n Mean cpu time in ms: " << GetMeanCpuTime();
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
  ost << timer.to_str();
  return ost;
}

//! ctor
ScopedTimer::ScopedTimer(std::shared_ptr<Timer> &itimer) : m_timer(itimer)
{
  if (m_timer) { (*m_timer).Start(); }
}
//! dtor
ScopedTimer::~ScopedTimer()
{
  if (m_timer) { (*m_timer).Stop(); }
}

}// namespace poutre
