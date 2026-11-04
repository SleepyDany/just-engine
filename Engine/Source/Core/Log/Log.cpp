#include "Log.h"

// Application layer
JE_DEFINE_LOG_CATEGORY(LogEngine, Log);
JE_DEFINE_LOG_CATEGORY(LogApplication, Log);

// Utilities
JE_DEFINE_LOG_CATEGORY(LogAssert, Error);
JE_DEFINE_LOG_CATEGORY(LogCheck, Error);
JE_DEFINE_LOG_CATEGORY(LogMemory, Log);
