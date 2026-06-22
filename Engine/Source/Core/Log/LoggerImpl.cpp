#include "Log/LoggerImpl.h"

JE::FLoggerImpl::FLoggerImpl(const ID& _id, const TLogFormatter& _logFormatter)
	: Formatter(_logFormatter)
	, Id(_id)
{
}

const JE::FLoggerImpl::ID& JE::FLoggerImpl::GetId() const
{
	return Id;
}

const JE::TLogFormatter& JE::FLoggerImpl::GetFormatter() const
{
	return Formatter;
}

void JE::FLoggerImpl::SetFormatter(const TLogFormatter& _formatter)
{
	Formatter = _formatter;
}

void JE::FLoggerImpl::Log(const FLogRecord& _logRecord)
{
	const bool bForceFlush = _logRecord.Verbosity == ELogVerbosity::Error || _logRecord.Verbosity == ELogVerbosity::Fatal;
	Log(Formatter ? Formatter(_logRecord) : _logRecord.Message, bForceFlush);
}
