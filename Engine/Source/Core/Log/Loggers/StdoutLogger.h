#pragma once

#include "Log/Logger.h"

namespace JE
{
	class JE_API FStdoutLogger : public FLogger
	{
		//- Types ------------------------
	private:
		using Super = FLogger;

		//- Lifecycle --------------------
	public:
		FStdoutLogger(const std::string& _id, TLogFormatter _logFormatter);
		virtual ~FStdoutLogger() override = default;

		//- Methods ----------------------
	protected:
		virtual void Write(std::string_view _message, bool _bForceFlush) override;
	};
} // namespace JE