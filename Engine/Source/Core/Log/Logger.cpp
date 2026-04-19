#include "Logger.h"

#include "Log/LogMisc.h"
#include "Log/LoggerImpl.h"

using namespace JE;

FLogger::FLogger(
	const FLogCategory& _category, const TLogFormatter& _formatter, const std::vector<std::shared_ptr<FLoggerImpl>>& _loggerImpls)
	: Category(_category)
	, Formatter(_formatter)
	, LoggerImpls(_loggerImpls)
{
}

ELogVerbosity FLogger::GetVerbosity() const
{
	return Category.GetVerbosity();
}

void FLogger::SetVerbosity(ELogVerbosity _verbosity)
{
	Category.SetVerbosity(_verbosity);
}

FLogCategory& FLogger::GetCategory()
{
	return Category;
}

const FLogCategory& FLogger::GetCategory() const
{
	return Category;
}

const TLogFormatter& FLogger::GetFormatter() const
{
	return Formatter;
}

void FLogger::SetFormatter(const TLogFormatter& _formatter)
{
	Formatter = _formatter;
}

void FLogger::Log(const FLogRecord& _logRecord)
{
	const ELogVerbosity verbosity = GetVerbosity();
	if (_logRecord.Verbosity > verbosity)
	{
		return;
	}

	for (auto& impl : LoggerImpls)
	{
		if (!impl)
		{
			continue;
		}

		const TLogFormatter& formatter = impl->GetFormatter() ? impl->GetFormatter() : Formatter;
		impl->Log(formatter(_logRecord));
	}

	if (verbosity == ELogVerbosity::Fatal)
	{
		JE_PLATFORM_BREAK();
	}
}

void FLogger::RegisterImpl(const std::shared_ptr<FLoggerImpl>& _logImpl)
{
	// TODO: check for duplicates
	LoggerImpls.emplace_back(_logImpl);
}

void FLogger::UnregisterImpl(const std::shared_ptr<FLoggerImpl>& _logImpl)
{
	LoggerImpls.erase(std::remove(LoggerImpls.begin(), LoggerImpls.end(), _logImpl), LoggerImpls.end());
}
