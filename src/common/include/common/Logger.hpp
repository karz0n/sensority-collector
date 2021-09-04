#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/Application.h>

// clang-format off

#define LOG_DEBUG(msg) \
    poco_debug(Poco::Util::Application::instance().logger(), msg);
#define LOG_INFO(msg) \
    poco_information(Poco::Util::Application::instance().logger(), msg);
#define LOG_NOTICE(msg) \
    poco_notice(Poco::Util::Application::instance().logger(), msg);
#define LOG_WARN(msg) \
    poco_warning(Poco::Util::Application::instance().logger(), msg);
#define LOG_ERROR(msg) \
    poco_error(Poco::Util::Application::instance().logger(), msg);
#define LOG_CRITICAL(msg) \
    poco_critical(Poco::Util::Application::instance().logger(), msg);
#define LOG_FATAL(msg) \
    poco_fatal(Poco::Util::Application::instance().logger(), msg);

#define LOG_DEBUG_F(fmt, ...) \
    poco_debug_f(Poco::Util::Application::instance().logger(), fmt, __VA_ARGS__);
#define LOG_INFO_F(fmt, ...) \
    poco_information_f(Poco::Util::Application::instance().logger(), fmt, __VA_ARGS__);
#define LOG_NOTICE_F(fmt, ...) \
    poco_notice_f(Poco::Util::Application::instance().logger(), fmt, __VA_ARGS__);
#define LOG_WARN_F(fmt, ...) \
    poco_warning_f(Poco::Util::Application::instance().logger(), fmt, __VA_ARGS__);
#define LOG_ERROR_F(fmt, ...) \
    poco_error_f(Poco::Util::Application::instance().logger(), fmt, __VA_ARGS__);
#define LOG_CRITICAL_F(fmt, ...) \
    poco_critical_f(Poco::Util::Application::instance().logger(), fmt, __VA_ARGS__);
#define LOG_FATAL_F(fmt, ...) \
    poco_fatal_f(Poco::Util::Application::instance().logger(), fmt, __VA_ARGS__);

// clang-format on