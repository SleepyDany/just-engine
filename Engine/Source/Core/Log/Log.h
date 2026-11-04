#pragma once

#include "Log/LogMacros.h"
#include "Log/LogManager.h"
#include "Log/LogMisc.h"
#include "Log/Logger.h"

/** Common engine's log categories. */
JE_DECLARE_LOG_CATEGORY(LogEngine);
JE_DECLARE_LOG_CATEGORY(LogApplication);

JE_DECLARE_LOG_CATEGORY(LogAssert);
JE_DECLARE_LOG_CATEGORY(LogCheck);
JE_DECLARE_LOG_CATEGORY(LogMemory);