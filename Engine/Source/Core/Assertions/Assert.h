#pragma once

// TODO: how to define JE_USE_ASSERTS? Debug only?
#if _DEBUG && !defined(JE_USE_ASSERTS)
	#define JE_USE_ASSERTS
#endif

#ifdef JE_USE_ASSERTS

	#include "CorePch.h"
	#include "Log/Log.h"

JE_DEFINE_LOG_CATEGORY(LogAssert, Error);

	/** Standard assertion with logging. */
	#define JE_ASSERT(Expr, Message)                                                \
		do                                                                          \
		{                                                                           \
			if (!(Expr))                                                            \
			{                                                                       \
				JE_LOG(LogAssert, Fatal, "Assert '{}' failed: {}", #Expr, Message); \
				std::abort();                                                       \
			}                                                                       \
		} while (false)

	/** For INTERNAL usage only. Use JE_ASSERT() instead. */
	#define JE_PRIVATE_ASSERT(Expr, Message)                                        \
		do                                                                          \
		{                                                                           \
			if (!(Expr))                                                            \
			{                                                                       \
				std::cerr << std::format("Assert '{}' failed: {}", #Expr, Message); \
				/** TODO: do we need it here? */                                    \
				JE_PLATFORM_BREAK();                                                \
				std::abort();                                                       \
			}                                                                       \
		} while (false)

#else
	#define JE_ASSERT(Expr, Message) void(0)
	#define JE_PRIVATE_ASSERT(Expr, Message) void(0)
#endif