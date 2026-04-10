#include "Log/LogMacros.h"

JE_DEFINE_LOG_CATEGORY(LogCore, Log);

int main()
{
	JE_LOG(LogCore, Log, "Hello, World!\tIt's JustEngine.");

	return 0;
}