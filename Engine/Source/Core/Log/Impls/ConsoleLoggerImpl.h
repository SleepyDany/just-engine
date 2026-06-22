#pragma once

#include "Log/LoggerImpl.h"

namespace JE
{
	class JE_API FConsoleLoggerImpl : public FLoggerImpl
	{
		//- Types ------------------------
	private:
		using Super = FLoggerImpl;

		//- Events -----------------------
		//- Variables --------------------

		//- Lifecycle --------------------
	public:
		FConsoleLoggerImpl(const ID& _id, const TLogFormatter& _logFormatter = nullptr);
		virtual ~FConsoleLoggerImpl() override;

		//- Methods ----------------------
	public:
		virtual void Log(const std::string& _message, bool _bForceFlush) override;
	};
} // namespace JE