#pragma once

#include <Core/CoreDefines.h>
#include <Core/Log/LogMisc.h>

#include <memory>
#include <vector>

namespace JE
{
	class FLoggerImpl;

	class JE_API FLogger // TODO: final : private FNonCopyable
	{
		friend class FLogManager;

		//- Variables --------------------
	private:
		FLogCategory Category;

		/** Formatter for current logger. Will be overwritten with implementation's formatters if exist. */
		TLogFormatter Formatter = nullptr;

		/** Different logger implementations. */
		std::vector<std::shared_ptr<FLoggerImpl>> LoggerImpls;

		//- Lifecycle --------------------
		// TODO: Rework NonCopyable objects creation!
	public:
		FLogger(const FLogCategory& _logCategory,
			const TLogFormatter& _formatter = nullptr,
			const std::vector<std::shared_ptr<FLoggerImpl>>& _loggerImpls = {});

		//- Methods ----------------------
	public:
		void RegisterImpl(const std::shared_ptr<FLoggerImpl>& _logImpl);
		void UnregisterImpl(const std::shared_ptr<FLoggerImpl>& _logImpl);

		ELogVerbosity GetVerbosity() const;
		void SetVerbosity(ELogVerbosity _verbosity);

		FLogCategory& GetCategory();
		const FLogCategory& GetCategory() const;

		const TLogFormatter& GetFormatter() const;
		void SetFormatter(const TLogFormatter& _formatter);

		void Log(const FLogRecord& _logRecord);
	};
} // namespace JE
