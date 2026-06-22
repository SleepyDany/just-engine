#include "Engine.h"

#include "Log/Log.h"

JE::FEngine::FEngine(int32 _argCount, char** _argString)
{
	LaunchParameters.reserve(_argCount);

	// TODO: do we need first parameter (full .exe path)?
	for (int32 i = 0; i < _argCount; ++i)
	{
		LaunchParameters.emplace_back(_argString[i]);
	}
}

JE::FEngine::~FEngine()
{
	LaunchParameters.clear();
}

bool JE::FEngine::Initialize()
{
	return true;
}

void JE::FEngine::Run()
{
	while (true)
	{
		JE_LOG(LogCore, Log, "JustEngine loop...");
		std::this_thread::sleep_for(std::chrono::milliseconds(2'000));
	}
}

int32 JE::FEngine::Shutdown()
{
	return EXIT_SUCCESS;
}
