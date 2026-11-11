/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include <memory>
#include <mutex>
#include <vector>
#include <ostream>
#include <poutre/base/trace.hpp>

// Set default active level if not defined externally by the build system
#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_ERROR
#endif

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace poutre {

//! Private ctor
GlobalLogger::GlobalLogger() {

            const std::string logger_name = "poutre_global_logger";
            try {
            m_logger = spdlog::get(logger_name);
            
            if(!m_logger) // TODO ? ADD mutex ?
            {
            std::vector<spdlog::sink_ptr> sinks;
            // 1. Thread-safe Thread-colored Console Sink
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::trace);
            // Format: [Timestamp] [Thread ID] [Log Level] Log Message
            console_sink->set_pattern("%Y-%m-%d %H:%M:%S.%e [%t] [%^%l%$] %v");
            sinks.push_back(console_sink);

            // 2. Thread-safe Rotating File Sink (Max 5MB per file, max 3 files)
            auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                "logs/app_log.txt", 1024 * 1024 * 5, 3 // NOLINT
            );
            file_sink->set_level(spdlog::level::debug);
            // Include source information (%s:%#) for the file sink format if desired
            file_sink->set_pattern("%Y-%m-%d %H:%M:%S.%e [%t] [%l] [%s:%#] %v");
            sinks.push_back(file_sink);

            // Create the combined master logger
            m_logger = std::make_shared<spdlog::logger>(logger_name, sinks.begin(), sinks.end());
            
            // Set global tracking properties
            m_logger->set_level(spdlog::level::warn); // Minimum level to log
            
            // Flush immediately on Warn or higher to protect critical error tracking
            m_logger->flush_on(spdlog::level::warn); 

            // Optional: Register globally within spdlog's internal registry
            spdlog::register_logger(m_logger);
          }
          } catch (const spdlog::spdlog_ex& ex) {
            // Fallback strategy if initialization crashes (e.g., directory creation permission errors)
            std::fprintf(stderr, "Log initialization failed: %s\n", ex.what()); // NOLINT
            std::abort();
        }
    }

GlobalLogger::~GlobalLogger() { spdlog::shutdown(); }

std::ostream &operator<<(std::ostream &ost, GlobalLogger::LoggerLevel level)// NOLINT(misc-unused-parameters)
{
  switch (level) {
  case GlobalLogger::LoggerLevel::trace:
    ost << "trace";
    break;
  case GlobalLogger::LoggerLevel::critical:
    ost << "critical";
    break;
  case GlobalLogger::LoggerLevel::debug:
    ost << "debug";
    break;
  case GlobalLogger::LoggerLevel::err:
    ost << "err";
    break;
  case GlobalLogger::LoggerLevel::info:
    ost << "info";
    break;
  case GlobalLogger::LoggerLevel::warn:
    ost << "warn";
    break;
  default:
    ost << "Unknown GlobalLogger::LoggerLevel";
    break;
  }
  return ost;
}
}// namespace poutre