#pragma once

#ifndef __LOG_H__
#define __LOG_H__

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Engine
{
	class Log
	{
	private:

		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }
	};
}

//core log macros
#define EN_CORE_TRACE(...)     ::Engine::Log::getCoreLogger()->trace(__VA_ARGS__)
#define EN_CORE_INFO(...)      ::Engine::Log::getCoreLogger()->info(__VA_ARGS__)
#define EN_CORE_WARN(...)      ::Engine::Log::getCoreLogger()->warn(__VA_ARGS__)
#define EN_CORE_ERROR(...)     ::Engine::Log::getCoreLogger()->error(__VA_ARGS__)
#define EN_CORE_FATAL(...)     ::Engine::Log::getCoreLogger()->fatal(__VA_ARGS__)

//client log macros
#define EN_TRACE(...)          ::Engine::Log::getClientLogger()->trace(__VA_ARGS__)
#define EN_INFO(...)           ::Engine::Log::getClientLogger()->info(__VA_ARGS__)
#define EN_WARN(...)           ::Engine::Log::getClientLogger()->warn(__VA_ARGS__)
#define EN_ERROR(...)          ::Engine::Log::getClientLogger()->error(__VA_ARGS__)
#define EN_FATAL(...)          ::Engine::Log::getClientLogger()->fatal(__VA_ARGS__)

#ifdef EN_ENABLE_ASSERTS
	#define EN_ASSERT(x, ...) { if(!(x)) { EN_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EN_CORE_ASSERT(x, ...){ if(!(x)) { EN_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define EN_ASSERT(x, ...)
	#define EN_CORE_ASSERT(x, ...)
#endif // EN_ENABLE_ASSERTS

#endif // !__LOG_H__