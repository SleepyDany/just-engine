#pragma once

#include <Core/Log/LogManager.h>
#include <Core/Log/LogMisc.h>
#include <Core/Log/Logger.h>

#define JE_DEFINE_LOG_CATEGORY(CategoryName, Verbosity)                                   \
	namespace                                                                             \
	{                                                                                     \
		const JE::FLogCategory CategoryName(#CategoryName, JE::ELogVerbosity::Verbosity); \
	}

#define JE_PRIVATE_GET_LOG_CATEGORY(CategoryName) ::CategoryName

#define JE_PRIVATE_LOG_IMPL(CategoryName, Verbosity, Format, ...)                                                                       \
	do                                                                                                                                  \
	{                                                                                                                                   \
		if constexpr (JE::ELogVerbosity::Verbosity > JE::ELogVerbosity::None && JE::ELogVerbosity::Verbosity <= JE::ELogVerbosity::MAX) \
		{                                                                                                                               \
			/** TODO: do we need std::source_location::current()?  */                                                                   \
			const JE::FLogRecord logRecord{JE_PRIVATE_GET_LOG_CATEGORY(CategoryName),                                                   \
				JE::ELogVerbosity::Verbosity,                                                                                           \
				std::format(Format __VA_OPT__(, ) __VA_ARGS__),                                                                         \
				__FILE__,                                                                                                               \
				__FUNCTION__,                                                                                                           \
				__LINE__,                                                                                                               \
				std::chrono::system_clock::now()};                                                                                      \
			JE::FLogManager::Get().GetLogger(JE_PRIVATE_GET_LOG_CATEGORY(CategoryName)).Log(logRecord);                                 \
		}                                                                                                                               \
	} while (false)

#define JE_LOG(Category, Verbosity, Format, ...) JE_PRIVATE_LOG_IMPL(Category, Verbosity, Format, __VA_ARGS__)

#define JE_CLOG(Condition, Category, Verbosity, Format, ...)           \
	if (!!(Condition))                                                 \
	{                                                                  \
		JE_PRIVATE_LOG_IMPL(Category, Verbosity, Format, __VA_ARGS__); \
	}