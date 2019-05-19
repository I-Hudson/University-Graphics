#pragma once

#ifndef __LOG_H__
#define __LOG_H__

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

#ifdef _DEBUG
#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_NEW
#endif

#include <memory>
#include "spdlog/spdlog.h"

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

#endif // !__LOG_H__