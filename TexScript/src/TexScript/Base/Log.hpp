#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "TexScript/Base/Base.hpp"

namespace TexScript {

	class Log
	{
	public:
		Log() = delete;
		~Log() = delete;

		static void Init();

	public:
		static Ref<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static Ref<spdlog::logger> s_Logger;
	};

}

#define TS_TRACE(...)         ::TexScript::Log::GetLogger()->trace(__VA_ARGS__)
#define TS_INFO(...)          ::TexScript::Log::GetLogger()->info(__VA_ARGS__)
#define TS_WARN(...)          ::TexScript::Log::GetLogger()->warn(__VA_ARGS__)
#define TS_ERROR(...)         ::TexScript::Log::GetLogger()->error(__VA_ARGS__)
#define TS_CRITICAL(...)      ::TexScript::Log::GetLogger()->critical(__VA_ARGS__)