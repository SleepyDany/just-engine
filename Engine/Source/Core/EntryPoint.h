#pragma once

#include "Application/Application.h"
#include "Engine.h"

/** Should be defined on CLIENT side. */
extern JE::FApplication* JE::CreateApplication();

// TODO: Implement platform specific main?
int main(int argc, char** argv)
{
	JE::gEngine = std::make_unique<JE::FEngine>(argc, argv);
	JE::FApplication* application = JE::CreateApplication();
	JE::gEngine->SetApplication(application);

	if (JE::gEngine->Initialize())
	{
		JE::gEngine->Run();
	}

	int32 exitCode = JE::gEngine->Shutdown();
	return exitCode;
}