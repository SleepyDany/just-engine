#include "Window.h"

JE::IWindow::IWindow(const FWindowProperties& _windowProperties)
	: Properties(_windowProperties)
{
}

bool JE::IWindow::ShouldClose() const
{
	return false;
}

void JE::IWindow::SetActive(bool _bIsActive)
{
	bIsActive = _bIsActive;
}

bool JE::IWindow::IsActive() const
{
	return bIsActive;
}
