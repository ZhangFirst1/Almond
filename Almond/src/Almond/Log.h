// 日志系统，使用了spdlog
#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Almond {
	class ALMOND_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClinetLogger() { return s_ClientLogger;  }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define AM_CORE_ERROR(...)   Almond::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AM_CORE_WARN(...)    Almond::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AM_CORE_INFO(...)    Almond::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AM_CORE_TRACE(...)   Almond::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AM_CORE_FATAL(...)   Almond::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// App log macros
#define AM_ERROR(...)        Almond::Log::GetClinetLogger()->error(__VA_ARGS__)
#define AM_WARN(...)         Almond::Log::GetClinetLogger()->warn(__VA_ARGS__)
#define AM_INFO(...)         Almond::Log::GetClinetLogger()->info(__VA_ARGS__)
#define AM_TRACE(...)        Almond::Log::GetClinetLogger()->trace(__VA_ARGS__)
#define AM_FATAL(...)        Almond::Log::GetClinetLogger()->fatal(__VA_ARGS__)