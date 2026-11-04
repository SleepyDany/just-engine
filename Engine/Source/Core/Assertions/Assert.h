#pragma once

#include "CoreDefines.h"

#ifdef JE_USE_ASSERTS

	#include "Assertions/CheckManager.h"
	#include "Log/Log.h"

	#include <cstdlib>
	#include <format>
	#include <iostream>

	/** Standard assertion with formatted logging. */
	#define JE_ASSERT_F(Expr, Message)                                                                                                   \
		do                                                                                                                               \
		{                                                                                                                                \
			if (!(Expr))                                                                                                                 \
			{                                                                                                                            \
				JE::FCheckManager& checkManager = JE::FCheckManager::Get();                                                              \
				JE_LOG(LogAssert, Fatal, "Assertion '{}' failed: {}\nStacktrace:\n{}\n", #Expr, Message, checkManager.GetStacktrace(2)); \
				std::abort();                                                                                                            \
			}                                                                                                                            \
		} while (false)

	/** Standard assertion with logging. */
	#define JE_ASSERT(Expr) JE_ASSERT_F(Expr, "Abort.")

	/** For INTERNAL usage only. Use JE_ASSERT_F() instead. */
	#define JE_PRIVATE_ASSERT_F(Expr, Message)                                                                                        \
		do                                                                                                                            \
		{                                                                                                                             \
			if (!(Expr))                                                                                                              \
			{                                                                                                                         \
				JE::FCheckManager& checkManager = JE::FCheckManager::Get();                                                           \
				std::cerr << std::format("Assert '{}' failed: {}\nStacktrace:\n{}\n", #Expr, Message, checkManager.GetStacktrace(2)); \
				/** TODO: do we need it here? */                                                                                      \
				JE_PLATFORM_BREAK();                                                                                                  \
				std::abort();                                                                                                         \
			}                                                                                                                         \
		} while (false)

	/** For INTERNAL usage only. Use JE_ASSERT() instead. */
	#define JE_PRIVATE_ASSERT(Expr) JE_PRIVATE_ASSERT_F(Expr, "Abort.")

#else
	#define JE_ASSERT_F(Expr, Message) void(0)
	#define JE_ASSERT(Expr) void(0)
	#define JE_PRIVATE_ASSERT_F(Expr, Message) void(0)
	#define JE_PRIVATE_ASSERT(Expr) void(0)
#endif