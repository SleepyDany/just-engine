#include "Log/LogMacros.h"
#include "Utilities/StringUtilities.h"

#include <iostream>
#include <thread>

JE_DEFINE_LOG_CATEGORY(LogCore, Log);
JE_DEFINE_LOG_CATEGORY(LogCADAnimInstanceRacingClub, Log);

int main()
{
	std::cout << "Hello, World!\nIt's JustEngine." << '\n';

	double f = 0;
	JE_LOG(LogCore, Log, "{} {}", f + 1, 1);

	int i = 0;
	for (; i < 10; ++i)
	{
		JE_CLOG(i > 6, LogCore, Error, "CLog testing: {}", i);
	}

	for (; i < 100; ++i)
	{
		JE_LOG(LogCADAnimInstanceRacingClub, Warning, "BIIG testing ", i);
	}

	return 0;
}