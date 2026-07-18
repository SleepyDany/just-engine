#include "ConsoleApplication.h"

JE::FConsoleApplication::FConsoleApplication()
	: FApplication()
{
}

JE::FConsoleApplication::~FConsoleApplication()
{
}

bool JE::FConsoleApplication::IsWindowed() const
{
	return false;
}
