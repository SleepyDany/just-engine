#include "ConsoleApplication.h"

JE::FConsoleApplication::FConsoleApplication()
	: FApplication()
{
}

JE::FConsoleApplication::~FConsoleApplication()
{
}

bool JE::FConsoleApplication::Initialize()
{
	return true;
}

void JE::FConsoleApplication::PollEvents()
{
}

void JE::FConsoleApplication::OnEvent(FEvent& _event)
{
}

void JE::FConsoleApplication::OnUpdate(double _deltaTime)
{
}

void JE::FConsoleApplication::OnRender()
{
}

void JE::FConsoleApplication::OnEndFrame()
{
}

void JE::FConsoleApplication::Shutdown()
{
}

bool JE::FConsoleApplication::IsWindowed() const
{
	return false;
}

bool JE::FConsoleApplication::IsRunning() const
{
	static bool bFirstRun = true;
	if (bFirstRun)
	{
		bFirstRun = false;
		return true;
	}

	return false;
}
