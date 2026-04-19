#pragma once

#include "Log/LoggerImpl.h"

namespace JE
{
	class JE_API FConsoleLoggerImpl : public FLoggerImpl
	{
		//- Lifecycle --------------------
	public:
		using FLoggerImpl::FLoggerImpl;

		//- Methods ----------------------
	public:
		virtual void Log(const std::string& _message) override;
	};
} // namespace JE