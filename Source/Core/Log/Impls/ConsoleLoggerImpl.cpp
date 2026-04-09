#include "ConsoleLoggerImpl.h"

#include <iostream>

using namespace JE;

void FConsoleLoggerImpl::Log(const std::string& _message)
{
	std::cout << _message;
}