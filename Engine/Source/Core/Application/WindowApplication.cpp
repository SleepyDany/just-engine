#include "WindowApplication.h"

JE::FWindowApplication::FWindowApplication()
{
}

JE::FWindowApplication::~FWindowApplication()
{
	FApplication::~FApplication();
}

void JE::FWindowApplication::Initialize()
{
}

void JE::FWindowApplication::OnEvent()
{
}

void JE::FWindowApplication::OnUpdate(float _deltaTime)
{
}

void JE::FWindowApplication::OnRender()
{
}

void JE::FWindowApplication::Shutdown()
{
	return FApplication::Shutdown();
}
