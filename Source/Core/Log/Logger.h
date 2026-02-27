#pragma once

#include <Core/CoreDefines.h>
#include <Core/Log/LogDetails.h>
#include <Core/Types/NonCopyable.h>

#include <memory>
#include <vector>

namespace JE
{
	class FLoggerImpl;

	template <class TLogCategory>
	class JE_API FLogger final : public NonCopyable
	{
		//- Variables --------------------
	private:
		TLogCategory Category;

		ELogVerbosity Verbosity;

		// TODO: move to settings/cvar?
		bool bAlwaysBreakOnLog = false;

		TLogFormatter DefaultFormatter = nullptr;

		std::vector<std::shared_ptr<FLoggerImpl>> LoggerImpls;

		//- Lifecycle --------------------
	private:
		FLogger(ELogVerbosity _verbosity, TLogFormatter&& _formatter = nullptr);
		virtual ~FLogger() = default;

		//- Methods ----------------------
	public:
		static FLogger& GetInstance();

		void RegisterImpl(const std::shared_ptr<FLoggerImpl>& _logImpl);
		void UnregisterImpl(const std::shared_ptr<FLoggerImpl>& _logImpl);

		ELogVerbosity GetVerbosity() const;
		void SetVerbosity(ELogVerbosity _verbosity);

		FLogCategory::ID GetCategoryId() const;

		const TLogFormatter& GetDefaultFormatter() const;
		void SetDefaultFormatter(TLogFormatter&& _defaultFormatter);

		void Log(const FLogDetails& _logDetails);
	};
} // namespace JE

// TODO: Rework
#define JE_DEFINE_LOG_CATEGORY(CategoryName)                            \
	namespace JE                                                        \
	{                                                                   \
		struct F##CategoryName##LogCategory final : public FLogCategory \
		{                                                               \
			inline std::string toString() const                         \
			{                                                           \
				return #CategoryName;                                   \
			}                                                           \
		};                                                              \
	}

// TODO: Rework
#define JE_LOG(Category, Verbosity, Format, ...)                                                                                           \
	JE::LogDetails logDetails##__LINE__(JE::LogVerbosity::Verbosity, std::format(Format, ##__VA_ARGS__), std::source_location::current()); \
	static_assert(JE::LogVerbosity::Verbosity <= JE::LogVerbosity::NUM);                                                                   \
	{                                                                                                                                      \
		JE::LogDetails& logDetails = logDetails##__LINE__;                                                                                 \
		Logger<Category##LogCategory>& currentLogger = Logger<Category##LogCategory>::GetInstance();                                       \
		if (currentLogger.GetVerbosity() >= JE::LogVerbosity::Verbosity)                                                                   \
		{                                                                                                                                  \
			currentLogger.Log(logDetails);                                                                                                 \
		}                                                                                                                                  \
	}