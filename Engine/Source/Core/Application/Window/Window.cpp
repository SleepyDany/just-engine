#include "Window.h"

#include "Engine.h"

uint32 JE::IWindow::GlobalWindowId = 0;

JE::IWindow::IWindow(const FWindowProperties& _windowProperties)
	: Id(GlobalWindowId++)
	, Properties(_windowProperties)
{
}

uint32 JE::IWindow::GetId() const
{
	return Id;
}

JE::FWindowProperties JE::IWindow::GetProperties() const
{
	return Properties;
}

bool JE::IWindow::ShouldClose() const
{
	return false;
}

bool JE::IWindow::IsFocused() const
{
	return Properties.bIsFocused;
}
