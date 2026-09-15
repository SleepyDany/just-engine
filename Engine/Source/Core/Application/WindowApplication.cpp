#include "WindowApplication.h"

#include "Engine.h"
#include "Window/Window.h"

JE::FWindowApplication::FWindowApplication()
{
}

JE::FWindowApplication::~FWindowApplication()
{
	FApplication::~FApplication();
}

bool JE::FWindowApplication::Initialize()
{
	FWindowProperties windowProperties{
		.Title = "Just Engine",
		.Width = 1280,
		.Height = 720,
	};

	std::shared_ptr<IWindow> Window = std::shared_ptr<IWindow>(IWindow::Create(windowProperties));
	if (!Window)
	{
		return false;
	}

	AddWindow(Window);
	return true;
}

void JE::FWindowApplication::PollEvents()
{
	GetCurrentWindow()->PollEvents();
}

void JE::FWindowApplication::OnEvent(FEvent& _event)
{
}

void JE::FWindowApplication::OnUpdate(double _deltaTime)
{
}

void JE::FWindowApplication::OnRender()
{
}

void JE::FWindowApplication::OnEndFrame()
{
	if (std::shared_ptr<IWindow> currentWindow = GetCurrentWindow())
	{
		currentWindow->SwapBuffers();
	}
}

void JE::FWindowApplication::Shutdown()
{
	Windows.clear();
}

bool JE::FWindowApplication::IsWindowed() const
{
	return true;
}

bool JE::FWindowApplication::IsRunning() const
{
	std::shared_ptr<IWindow> currentWindow = GetCurrentWindow();
	return currentWindow && !currentWindow->ShouldClose();
}

std::shared_ptr<JE::IWindow> JE::FWindowApplication::GetCurrentWindow() const
{
	if (auto windowIter = Windows.find(CurrentWindowId); windowIter != Windows.end())
	{
		return windowIter->second;
	}
	return nullptr;
}

void JE::FWindowApplication::AddWindow(const std::shared_ptr<IWindow>& _window)
{
	JE_CHECK_RETURN_CF(_window, , LogApplication, "Couldn't add invalid window to application.");

	gEngine->GetGlobalDelegate(EEventType::WindowClosed).Add(_window, &IWindow::OnClosed);
	gEngine->GetGlobalDelegate(EEventType::WindowResized).Add(_window, &IWindow::OnResized);
	gEngine->GetGlobalDelegate(EEventType::WindowFocused).Add(_window, &IWindow::OnFocused);
	gEngine->GetGlobalDelegate(EEventType::WindowMoved).Add(_window, &IWindow::OnMoved);
	gEngine->GetGlobalDelegate(EEventType::WindowMaximized).Add(_window, &IWindow::OnMaximized);
	gEngine->GetGlobalDelegate(EEventType::WindowIconified).Add(_window, &IWindow::OnIconified);

	_window->SetFocus(true);
	Windows[_window->GetId()] = _window;
	CurrentWindowId = _window->GetId();
}

bool JE::FWindowApplication::RemoveWindow(uint32 _windowId)
{
	return Windows.erase(_windowId) > 0;
}
