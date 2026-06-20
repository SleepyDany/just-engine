#pragma once

#include "Application.h"

/** Should be defined on CLIENT side */
extern JE::FApplication* JE::CreateApplication(int32 _argCount, char** _argString);

// TODO: Implement platform specific main?
int main(int argc, char** argv)
{
	int32 exitCode = EXIT_FAILURE;
	JE::FApplication* application = JE::CreateApplication(argc, argv);

	if (application->Initialize())
	{
		application->Run();
		exitCode = application->Shutdown();
	}

	delete application;

	return exitCode;
}