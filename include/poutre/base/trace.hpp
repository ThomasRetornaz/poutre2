
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   trace.hpp
 * @author Thomas Retornaz
 * @brief  Trace capabilities
 *
 *
 */

#include <memory>
#include <poutre/base/base.hpp>
#include <poutre/base/config.hpp>

#include <fstream>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>


namespace poutre {
/**
 * @addtogroup trace_group Trace facilities
 * @ingroup poutre_base_group
 *@{
 */
class BASE_API GlobalLogger
{
  static std::once_flag m_initFlag;

public:
  enum class LoggerLevel : std::uint8_t {
    trace,
    debug,
    info,
    warn,
    err,
    critical
    /* off ,
     n_levels*/
  };

  // Meyers Singleton: Guarantees thread-safe initialization on first use (C++11+)
    static spdlog::logger& GetInstance() {
        static GlobalLogger instance;
        return *instance.m_logger;
    }

    // Delete copy/move constructors and assignment operators
    GlobalLogger(const GlobalLogger&) = delete;
    GlobalLogger(GlobalLogger&&) = delete;
    GlobalLogger& operator=(const GlobalLogger&) = delete;
    GlobalLogger& operator=(GlobalLogger&&) = delete;

  ~GlobalLogger();

  void setLoggerLevel(LoggerLevel level)
  {
    switch (level) {
    case LoggerLevel::debug:
      (*m_logger).set_level(spdlog::level::debug);
      break;
    case LoggerLevel::info:
      (*m_logger).set_level(spdlog::level::info);
      break;
    case LoggerLevel::critical:
      (*m_logger).set_level(spdlog::level::critical);
      break;
    case LoggerLevel::err:
      (*m_logger).set_level(spdlog::level::err);
      break;
    case LoggerLevel::trace:
      (*m_logger).set_level(spdlog::level::trace);
      break;
    case LoggerLevel::warn:
      (*m_logger).set_level(spdlog::level::warn);
      break;
    default:
      POUTRE_RUNTIME_ERROR("Logger unsupported enum case");
    }
  }
  void call(const char *MSG, LoggerLevel level = LoggerLevel::debug)
  {
    switch (level) {
    case LoggerLevel::debug:
      m_logger->debug(MSG);
      break;
    case LoggerLevel::info:
      m_logger->info(MSG);
      break;
    case LoggerLevel::critical:
      m_logger->critical(MSG);
      break;
    case LoggerLevel::err:
      m_logger->error(MSG);
      break;
    case LoggerLevel::trace:
      m_logger->trace(MSG);
      break;
    case LoggerLevel::warn:
      m_logger->warn(MSG);
      break;
    default:
      POUTRE_RUNTIME_ERROR("Logger unsupported enum case");
    }
  }

private:
    std::shared_ptr<spdlog::logger> m_logger;
    GlobalLogger();
};

//! operator<< for GlobalLogger::LoggerLevel
BASE_API std::ostream& operator<<(std::ostream &ost, GlobalLogger::LoggerLevel level);

/** @name Global logger macros
 */
/**@{*/
// NOLINTBEGIN
#define POUTRE_TRACE(...)    SPDLOG_LOGGER_CALL(&GlobalLogger::GetInstance(), spdlog::level::trace, __VA_ARGS__)
#define POUTRE_DEBUG(...)    SPDLOG_LOGGER_CALL(&GlobalLogger::GetInstance(), spdlog::level::debug, __VA_ARGS__)
#define POUTRE_INFO(...)     SPDLOG_LOGGER_CALL(&GlobalLogger::GetInstance(), spdlog::level::info, __VA_ARGS__)
#define POUTRE_WARN(...)     SPDLOG_LOGGER_CALL(&GlobalLogger::GetInstance(), spdlog::level::warn, __VA_ARGS__)
#define POUTRE_ERROR(...)    SPDLOG_LOGGER_CALL(&GlobalLogger::GetInstance(), spdlog::level::err, __VA_ARGS__)
#define POUTRE_CRITICAL(...) SPDLOG_LOGGER_CALL(&GlobalLogger::GetInstance(), spdlog::level::critical, __VA_ARGS__)
#define POUTRE_ENTERING(...) SPDLOG_LOGGER_CALL(&GlobalLogger::GetInstance(), spdlog::level::info, __VA_ARGS__)
// NOLINTEND
/**@}*/

// !@} doxygroup: trace_group

}// namespace poutre
