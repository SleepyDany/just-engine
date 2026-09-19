#include "Application/WindowApplication.h"
#include "EntryPoint.h"

JE::FApplication* JE::CreateApplication()
{
	return new JE::FWindowApplication();
}
