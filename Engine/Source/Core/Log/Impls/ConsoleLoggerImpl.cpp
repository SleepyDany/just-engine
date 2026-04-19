#include "ConsoleLoggerImpl.h"

using namespace JE;

void FConsoleLoggerImpl::Log(const std::string& _message)
{
	std::cout << _message;
}