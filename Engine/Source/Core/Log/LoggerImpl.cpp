#include "Log/LoggerImpl.h"

JE::FLoggerImpl::FLoggerImpl(const ID& _id, TLogFormatter&& _logFormatter)
	: Formatter(std::move(_logFormatter))
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
	Log(Formatter ? Formatter(_logRecord) : _logRecord.Message);
}
