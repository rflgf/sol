#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace sol
{

class Log
{
private:
	static std::shared_ptr<spdlog::logger> core_logger;
	static std::shared_ptr<spdlog::logger> client_logger;

public:
	static void init();
	
	static std::shared_ptr<spdlog::logger> &get_core_logger() { return core_logger; }
	static std::shared_ptr<spdlog::logger> &get_client_logger() { return client_logger; }
};

}

#define SOL_CORE_TRACE(...) sol::Log::get_core_logger()->trace(__VA_ARGS__)
#define SOL_CORE_INFO(...)  sol::Log::get_core_logger()->info(__VA_ARGS__)
#define SOL_CORE_WARN(...)  sol::Log::get_core_logger()->warn(__VA_ARGS__)
#define SOL_CORE_ERROR(...) sol::Log::get_core_logger()->error(__VA_ARGS__)
#define SOL_CORE_FATAL(...) sol::Log::get_core_logger()->fatal(__VA_ARGS__)

#define SOL_TRACE(...) sol::Log::get_client_logger()->trace(__VA_ARGS__)
#define SOL_INFO(...)  sol::Log::get_client_logger()->info(__VA_ARGS__)
#define SOL_WARN(...)  sol::Log::get_client_logger()->warn(__VA_ARGS__)
#define SOL_ERROR(...) sol::Log::get_client_logger()->error(__VA_ARGS__)
#define SOL_FATAL(...) sol::Log::get_client_logger()->fatal(__VA_ARGS__)