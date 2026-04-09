#include "LoggerImpl.h"

using namespace JE;

FLoggerImpl::FLoggerImpl(const ID& _id, TLogFormatter&& _logFormatter)
	: Formatter(std::move(_logFormatter))
	, Id(_id)
{
}

const FLoggerImpl::ID& FLoggerImpl::GetId() const
{
	return Id;
}

const TLogFormatter& FLoggerImpl::GetFormatter() const
{
	return Formatter;
}

void FLoggerImpl::SetFormatter(const TLogFormatter& _formatter)
{
	Formatter = _formatter;
}

void FLoggerImpl::Log(const FLogRecord& _logRecord)
{
	Log(Formatter ? Formatter(_logRecord) : _logRecord.Message);
}
