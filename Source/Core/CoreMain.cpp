#include "Log/LogMacros.h"

#include <iostream>

#include <LogTemp.h>

JE_DEFINE_LOG_CATEGORY(LogCore, Log);

int main()
{
	std::cout << "Hello, World!\nIt's JustEngine." << '\n';

	double f = 0;
	JE_LOG(LogCore, Log, "{} {}", f + 1, 1);
	JE_LOG(LogTemp, Warning, "Testing...");

	int i = 0;
	for (; i < 10; ++i)
	{
		JE_CLOG(i > 6, LogCore, Error, "CLog testing: {}", i);
	}

	return 0;
}