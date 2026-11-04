#pragma once

#include "CoreDefines.h"
#include "Log/LogMisc.h"

#include <set>

namespace JE
{
	class JE_API FLogger
	{
		//- Variables --------------------
	protected:
		/** Unique identifier for logger. */
		std::string Name;

		TLogFormatter Formatter;

		/**
		 * Filter of the categories logger is allowed to handle.
		 * Empty filter means 'all' categories could be handled.
		 */
		std::set<std::string> CategoryFilter;

		//- Lifecycle --------------------
	public:
		FLogger(const std::string& _id, TLogFormatter _formatter);
		virtual ~FLogger() = default;

		FLogger(const FLogger&) = delete;
		FLogger& operator=(const FLogger&) = delete;
		FLogger(FLogger&&) = delete;
		FLogger& operator=(FLogger&&) = delete;

		//- Methods ----------------------
	public:
		const std::string& GetName() const;
		const TLogFormatter& GetFormatter() const;

		bool IsSupported(const FLogCategory& _category) const;

		virtual void Write(const FLogRecord& _logRecord);

	protected:
		virtual void Write(std::string_view _message, bool _bForceFlush = false) = 0;
	};

} // namespace JE
