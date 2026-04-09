#pragma once

#ifdef _MSC_VER
	#define JE_PLATFORM_BREAK() (__nop(), __debugbreak())
#else
	#include <signal.h>
	#define JE_PLATFORM_BREAK() raise(SIGTRAP)
#endif

// TODO: export/import, etc
#define JE_API