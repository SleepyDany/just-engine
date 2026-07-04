#include "Engine.h"

#include "Assertions/Assert.h"
#include "Assertions/Check.h"

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
		// JE_LOG(LogCore, Log, "JustEngine loop...");

		JE_CHECK(false);
		JE_CHECK_F(false, "check with format!");
		JE_CHECK_CF(false, LogCore, "check with category!");

		bool boolka = true;
		for (;;)
		{
			if (boolka)
			{
				boolka = false;
				JE_CHECK_CONTINUE_F(false, "check with continue!");
			}
			JE_CHECK_BREAK_F(false, "check with break!");
		}

		JE_CHECK_RETURN_F(false, void(0), "check with return!");

		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

int32 JE::FEngine::Shutdown()
{
	return EXIT_SUCCESS;
}
