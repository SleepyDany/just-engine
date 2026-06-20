#include "ConsoleLoggerImpl.h"

void JE::FConsoleLoggerImpl::Log(const std::string& _message)
{
	std::cout << _message;
}