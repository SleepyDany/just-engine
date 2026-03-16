#pragma once

#include <Core/CoreDefines.h>
#include <Core/Log/LogMisc.h>

#include <string>

namespace JE
{
	struct FLogRecord;

	class JE_API FLoggerImpl
	{
		//- Types ------------------------
	public:
		using ID = std::string;

		//- Variables --------------------
	protected:
		// TODO: probably remove it
		/** Custom formatter for logger implementation. */
		TLogFormatter Formatter = nullptr;

		/** Unique identifier for logger implementation inside manager. */
		ID Id{};

		//- Lifecycle --------------------
	public:
		FLoggerImpl(const ID& _id, TLogFormatter&& _logFormatter = nullptr);
		virtual ~FLoggerImpl() = default;

		//- Methods ----------------------
	public:
		const ID& GetId() const;
		const TLogFormatter& GetFormatter() const;

		void SetFormatter(const TLogFormatter& _formatter);

		// TODO: probably remove it
		virtual void Log(const FLogRecord& _logRecord);
		virtual void Log(const std::string& _message) = 0;
	};

} // namespace JE
