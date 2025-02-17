#include "ampch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Almond {
	// 静态成员变量初始化
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	// 初始化日志
	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");				// 设置日志格式
		s_CoreLogger = spdlog::stdout_color_mt("Almond");	// 设置日志级别、颜色
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}