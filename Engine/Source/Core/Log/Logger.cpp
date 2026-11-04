#include "Log/Logger.h"

#include "LogMacros.h"

JE::FLogger::FLogger(const std::string& _id, TLogFormatter _formatter)
	: Name(_id)
	, Formatter(std::move(_formatter))
{
}

const std::string& JE::FLogger::GetName() const
{
	return Name;
}

const JE::TLogFormatter& JE::FLogger::GetFormatter() const
{
	return Formatter;
}

bool JE::FLogger::IsSupported(const FLogCategory& _category) const
{
	return CategoryFilter.empty() || CategoryFilter.contains(_category.GetName());
}

void JE::FLogger::Write(const FLogRecord& _logRecord)
{
	const bool bForceFlush = _logRecord.Verbosity == ELogVerbosity::Error || _logRecord.Verbosity == ELogVerbosity::Fatal;
	Write(Formatter ? Formatter(_logRecord) : _logRecord.Message, bForceFlush);
}
