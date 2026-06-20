#include "Engine.h"
#include "EntryPoint.h"

JE::FApplication* JE::CreateApplication(int32 _argCount, char** _argString)
{
	return new JE::FEngine(_argCount, _argString);
}
