#pragma once

#include <memory>

#ifdef AM_PLATFORM_WINDOWS
#ifdef AM_DYNAMIC_LINK
	#ifdef AM_BUILD_DLL
		#define ALMOND_API __declspec(dllexport)
	#else
		#define ALMOND_API __declspec(dllimport)
	#endif
#else 
	#define ALMOND_API 
#endif
#else
	#error Almond Only Supports Windows!
#endif

#ifdef AM_DEBUG
	#define AM_ENABLE_ASSERTS
#endif

// __debugbreak()用于在崩溃时打断点 else 在未启用宏时不会编译出任何东西
#ifdef AM_ENABLE_ASSERTS
	#define AM_ASSERT(x, ...) { if(!(x)) { AM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define AM_CORE_ASSERT(x, ...) { if(!(x)) { AM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
 #else
	#define AM_ASSERT(x, ...)
	#define AM_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define AM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Almond{

	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}