#pragma once

// 用以性能分析

#include "Almond/Core/Log.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string> 
#include <thread>
#include <mutex>
#include <sstream>

namespace Almond {
	// 以微秒为单位的浮点类型
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;							// 代码块名称
						
		FloatingPointMicroseconds Start;			// 开始时间
		std::chrono::microseconds ElapsedTime;		// 持续时间
		std::thread::id ThreadID;					// 线程ID
	};
	// 用于标识一段性能分析 session，可以理解为一次完整的记录生命周期
	struct InstrumentationSession
	{
		std::string Name;							// 会话名称
	};

	class Instrumentor
	{
	public:
		// 单例
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;
		// 开始一个新的记录 session，并打开输出文件。如果已有 session 会自动结束旧的。
		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					AM_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
				}
				InternalEndSession();
			}
			m_OutputStream.open(filepath);

			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else
			{
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					AM_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}
		// 调用内部方法 InternalEndSession()，写入 JSON 尾部并清理资源。
		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		Instrumentor()
			: m_CurrentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			EndSession();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}
	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	//// 用于处理字符串的工具函数
	//namespace InstrumentorUtils {

	//	template <size_t N>
	//	struct ChangeResult
	//	{
	//		char Data[N];
	//	};

	//	template <size_t N, size_t K>
	//	constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
	//	{
	//		ChangeResult<N> result = {};

	//		size_t srcIndex = 0;
	//		size_t dstIndex = 0;
	//		while (srcIndex < N)
	//		{
	//			size_t matchIndex = 0;
	//			while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
	//				matchIndex++;
	//			if (matchIndex == K - 1)
	//				srcIndex += matchIndex;
	//			result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
	//			srcIndex++;
	//		}
	//		return result;
	//	}
	//}
}

//// 暂时禁用
//#define AM_PROFILE 0
//
//#ifdef AM_PROFILE
//	#define AM_PROFILE_BEGIN_SESSION(name, filepath) ::Almond::Instrumentor::Get().BeginSession(name, filepath)
//	#define AM_PROFILE_END_SESSION() ::Almond::Instrumentor::Get().EndSession()
//	// ##__LINE__(name)可以用当前行号拼接变量名，如InstrumentationTimer timer42(name)
//	#define AM_PROFILE_SCOPE(name) ::Almond::InstrumentationTimer timer##__LINE__(name);
//	// __FUNSIG__用于返回完整的函数签名作为名称，如	AM_PROFILE_SCOPE("void MyClass::Foo(int)")
//	#define AM_PROFILE_FUNCTION() AM_PROFILE_SCOPE(__FUNCSIG__)
//#else
//	#define AM_PROFILE_BEGIN_SESSION(name, filepath)
//	#define AM_PROFILE_END_SESSION()
//	#define AM_PROFILE_SCOPE(name)
//	#define AM_PROFILE_FUNCTION()
//#endif

#define AM_PROFILE 0
#if AM_PROFILE

#define AM_PROFILE_BEGIN_SESSION(name, filepath) ::Hazel::Instrumentor::Get().BeginSession(name, filepath)
#define AM_PROFILE_END_SESSION() ::Hazel::Instrumentor::Get().EndSession()
#define AM_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Hazel::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   ::Hazel::InstrumentationTimer timer##line(fixedName##line.Data)
#define AM_PROFILE_SCOPE_LINE(name, line) AM_PROFILE_SCOPE_LINE2(name, line)
#define AM_PROFILE_SCOPE(name) AM_PROFILE_SCOPE_LINE(name, __LINE__)
#define AM_PROFILE_FUNCTION() AM_PROFILE_SCOPE(AM_FUNC_SIG)
#else
#define AM_PROFILE_BEGIN_SESSION(name, filepath)
#define AM_PROFILE_END_SESSION()
#define AM_PROFILE_SCOPE(name)
#define AM_PROFILE_FUNCTION()
#endif