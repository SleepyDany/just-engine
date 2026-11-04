#pragma once

#include <Core/Log/LogManager.h>
#include <Core/Log/LogMisc.h>
#include <Core/Log/Logger.h>

#include <format>

#define JE_DECLARE_LOG_CATEGORY(CategoryName) \
	namespace JE::LOG                         \
	{                                         \
		extern JE::FLogCategory CategoryName; \
	}

#define JE_DEFINE_LOG_CATEGORY(CategoryName, Verbosity)                             \
	namespace JE::LOG                                                               \
	{                                                                               \
		JE::FLogCategory CategoryName(#CategoryName, JE::ELogVerbosity::Verbosity); \
	}

#define JE_DEFINE_LOG_CATEGORY_STATIC(CategoryName, Verbosity)                             \
	namespace JE::LOG                                                                      \
	{                                                                                      \
		static JE::FLogCategory CategoryName(#CategoryName, JE::ELogVerbosity::Verbosity); \
	}

#define JE_GET_LOG_CATEGORY(CategoryName) JE::LOG::CategoryName

#define JE_GET_LOG_VERBOSITY(CategoryName) JE_GET_LOG_CATEGORY(CategoryName).GetVerbosity()

#define JE_SET_LOG_VERBOSITY(CategoryName, Verbosity) JE_GET_LOG_CATEGORY(CategoryName).SetVerbosity(JE::ELogVerbosity::Verbosity);

#define JE_LOG(CategoryName, Verbosity, Format, ...) JE_PRIVATE_LOG_IMPL(CategoryName, Verbosity, Format, __VA_ARGS__)

#define JE_CLOG(Condition, CategoryName, Verbosity, Format, ...)               \
	do                                                                         \
	{                                                                          \
		if (!!(Condition))                                                     \
		{                                                                      \
			JE_PRIVATE_LOG_IMPL(CategoryName, Verbosity, Format, __VA_ARGS__); \
		}                                                                      \
	} while (false)

#define JE_PRIVATE_LOG_IMPL(CategoryName, Verbosity, Format, ...)                                                                      \
	do                                                                                                                                 \
	{                                                                                                                                  \
		if constexpr (JE::ELogVerbosity::Verbosity > JE::ELogVerbosity::None && JE::ELogVerbosity::Verbosity < JE::ELogVerbosity::MAX) \
		{                                                                                                                              \
			JE::FLogCategory& logCategory = JE_GET_LOG_CATEGORY(CategoryName);                                                         \
			if (logCategory.GetVerbosity() >= JE::ELogVerbosity::Verbosity)                                                            \
			{                                                                                                                          \
				const JE::FLogRecord logRecord{&logCategory,                                                                           \
					std::format(Format __VA_OPT__(, ) __VA_ARGS__),                                                                    \
					__FILE__,                                                                                                          \
					__FUNCTION__,                                                                                                      \
					FDateTime::Now(),                                                                                                  \
					__LINE__,                                                                                                          \
					JE::ELogVerbosity::Verbosity};                                                                                     \
				JE::FLogManager::Get().Log(logRecord);                                                                                 \
			}                                                                                                                          \
		}                                                                                                                              \
	} while (false)