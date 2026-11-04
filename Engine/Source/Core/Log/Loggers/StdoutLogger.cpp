#include "StdoutLogger.h"

#include <iostream>

JE::FStdoutLogger::FStdoutLogger(const std::string& _id, TLogFormatter _formatter)
	: FLogger(_id, std::move(_formatter))
{
	// std::clog will be flushed if std::cerr is triggered
	std::clog.tie(&std::cerr);
}

void JE::FStdoutLogger::Write(std::string_view _message, bool _bForceFlush)
{
	// TODO: decide which std::cerr / std::clog / std::cout to use depending on Verbosity
	// cerr is not buffered, as others; can be tied: std::clog.tie(&std::cerr)
	std::ostream& stream = _bForceFlush ? std::cerr : std::clog;
	stream << _message;
}