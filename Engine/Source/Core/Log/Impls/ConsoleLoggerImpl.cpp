#include "ConsoleLoggerImpl.h"

JE::FConsoleLoggerImpl::FConsoleLoggerImpl(const ID& _id, const TLogFormatter& _logFormatter)
	: FLoggerImpl(_id, _logFormatter)
{
	// std::clog will be flushed if std::cerr is triggered
	std::clog.tie(&std::cerr);
}

JE::FConsoleLoggerImpl::~FConsoleLoggerImpl()
{
	FLoggerImpl::~FLoggerImpl();
}

void JE::FConsoleLoggerImpl::Log(const std::string& _message, bool _bForceFlush)
{
	// TODO: decide which std::cerr / std::clog / std::cout to use depending on Verbosity
	// cerr is not buffered, as others; can be tied: std::clog.tie(&std::cerr)
	std::ostream& stream = _bForceFlush ? std::cerr : std::clog;
	stream << _message;
}