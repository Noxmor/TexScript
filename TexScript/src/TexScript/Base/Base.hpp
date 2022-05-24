#pragma once

#include <memory>

#include "TexScript/Base/PlatformDetection.hpp"

#ifdef TS_DEBUG
#if defined(TS_PLATFORM_WINDOWS)
#define TS_DEBUGBREAK() __debugbreak()
#elif defined(TS_PLATFORM_LINUX)
#include <signal.h>
#define TS_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define TS_ENABLE_ASSERTS
#else
#define TS_DEBUGBREAK()
#endif

#define TS_EXPAND_MACRO(x) x
#define TS_STRINGIFY_MACRO(x) #x

#ifdef TS_ENABLE_ASSERTS
#define TS_ASSERT(x, ...) { if(!(x)) { TS_ERROR("Assertion Failed: {0}", __VA_ARGS__); TS_DEBUGBREAK(); } }
#else
#define TS_ASSERT(x, ...)
#endif

#define BIT(x) 1 << x

namespace TexScript {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}