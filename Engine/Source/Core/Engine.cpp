#include "Engine.h"

#include "Application/Window/Window.h"
#include "Assertions/Assert.h"
#include "Types/Time/Timespan.h"

#include <GLFW/glfw3.h>

/** Global Engine instance definition. */
std::unique_ptr<JE::FEngine> JE::gEngine = nullptr;

JE::FEngine::FEngine(int32 _argCount, char** _argString)
{
	std::ostringstream record;

	// TODO: cmdline parser
	CommandlineParameters.reserve(_argCount - 1);
	// skip .exe full path at 0 index
	for (int32 index = 1; index < _argCount; ++index)
	{
		CommandlineParameters.emplace_back(_argString[index]);
		record << CommandlineParameters.back() << (index != _argCount - 1 ? " " : "");
	}
	Commandline = record.str();

	JE_LOG(LogEngine, Log, "Engine commandline: {}.", record.str());
}

JE::FEngine::~FEngine()
{
}

bool JE::FEngine::Initialize()
{
	EngineStartTime = FDateTime::Now();

	// TODO: how to start application and when?
	JE_ASSERT(Application);
	Application->Initialize();

	if (Application->IsWindowed())
	{
		FWindowProperties windowProperties{
			.Title = "Just Engine",
			.Width = 1280,
			.Height = 720,
		};

		Window = std::shared_ptr<IWindow>(IWindow::Create(windowProperties));
		if (!Window)
		{
			ExitCode = EXIT_FAILURE;
			return false;
		}
		Window->SetActive(true);
	}

	bIsRunning = true;
	return true;
}

void JE::FEngine::Run()
{
	using namespace std::chrono_literals;
	PrevFrameTime = FDateTime::Now() - 1ms;
	FDateTime curFrameTime = PrevFrameTime;

	while (bIsRunning)
	{
		++FrameCounter;

		curFrameTime = FDateTime::Now();
		DeltaTime = (curFrameTime - PrevFrameTime).GetTotalSeconds();
		PrevFrameTime = curFrameTime;

		PollEvents();
		Update(DeltaTime);
		Render();

		EndFrame();

		if (Window && Window->ShouldClose())
		{
			bIsRunning = false;
		}
	}
}

void JE::FEngine::PollEvents()
{
	if (Window)
	{
		Window->PollEvents();

		// TODO: foreach event?
		OnEvent();
		Application->OnEvent();
	}
}

void JE::FEngine::OnEvent()
{
}

void JE::FEngine::Update(float _deltaTime)
{
	JE_LOG(LogEngine, Log, "Engine update: FrameCounter={}, DeltaTime={:.5f}.", FrameCounter, DeltaTime);
	Application->OnUpdate(_deltaTime);
}

void JE::FEngine::Render()
{
	glClearColor(1.0f, 0.5f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Application->OnRender();
}

void JE::FEngine::EndFrame()
{
	if (Window)
	{
		Window->SwapBuffers();
	}
}

int32 JE::FEngine::Shutdown()
{
	Application->Shutdown();
	return ExitCode;
}

void JE::FEngine::SetApplication(FApplication* _application)
{
	Application.reset(_application);
}

std::weak_ptr<JE::FApplication> JE::FEngine::GetApplication()
{
	return Application;
}

std::weak_ptr<JE::IWindow> JE::FEngine::GetActiveWindow()
{
	return Window;
}