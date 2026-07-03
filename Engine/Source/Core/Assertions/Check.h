#pragma once

// TODO: how to define JE_USE_CHECKS? not only debug?
#define JE_USE_CHECKS

#ifdef JE_USE_CHECKS

	#include "CheckManager.h"
	#include "CorePch.h"
	#include "Log/Log.h"

JE_DEFINE_LOG_CATEGORY(LogCheck, Error);

	/** INTERNAL checks implementation. Use JE_CHECK_* instead. */
	#define JE_PRIVATE_CHECK_CF_IMPL(Expr, Category, Message, ...)                                                    \
		(!!(Expr) ||                                                                                                  \
			[&]()                                                                                                     \
			{                                                                                                         \
				JE::FCheckManager& checkManager = JE::FCheckManager::Get();                                           \
				std::string_view messageView(Message);                                                                \
				if (checkManager.ShouldPrintToLog() && !messageView.empty())                                          \
				{                                                                                                     \
					/** TODO: How to remove <lambda>::operator() from function name? */                               \
					JE_LOG(Category, Error, Message, __VA_ARGS__);                                                    \
				}                                                                                                     \
                                                                                                                      \
				if (checkManager.ShouldPrintToScreen())                                                               \
				{                                                                                                     \
					/** TODO: Implement FCheckManager::PrintToScreen() */                                             \
					JE_ASSERT(false, "FCheckManager::PrintToScreen is not implemented.");                             \
				}                                                                                                     \
				/** TODO: How to remove <lambda>::operator() from function name? */                                   \
				JE_LOG(LogCheck, Error, "Check '{}' failed. Stacktrace:\n{}\n", #Expr, checkManager.GetStacktrace()); \
                                                                                                                      \
				if (checkManager.ShouldBreak())                                                                       \
				{                                                                                                     \
					JE_PLATFORM_BREAK();                                                                              \
				}                                                                                                     \
                                                                                                                      \
				return false;                                                                                         \
			}())

// Simple checks

	/** Check with log category and formatted message. */
	#define JE_CHECK_CF(Expr, Category, Message, ...) JE_PRIVATE_CHECK_CF_IMPL(Expr, Category, Message, __VA_ARGS__)

	/** Check with log category. */
	#define JE_CHECK_C(Expr, Category) JE_CHECK_CF(Expr, Category, "")

	/** Check with formatted message. */
	#define JE_CHECK_F(Expr, Message, ...) JE_CHECK_CF(Expr, LogCheck, Message, __VA_ARGS__)

	/** Check. */
	#define JE_CHECK(Expr) JE_CHECK_CF(Expr, LogCheck, "")

// Returning checks

	/** Returning check with log category and formatted message. */
	#define JE_CHECK_RETURN_CF(Expr, ReturnValue, Category, Message, ...)        \
		if (!!(!JE_PRIVATE_CHECK_CF_IMPL(Expr, Category, Message, __VA_ARGS__))) \
			return ReturnValue;                                                  \
		else                                                                     \
			volatile auto __stub = ((void*)0)

	/** Returning check with log category. */
	#define JE_CHECK_RETURN_C(Expr, ReturnValue, Category) JE_CHECK_RETURN_CF(Expr, ReturnValue, Category, "")

	/** Returning check with formatted message. */
	#define JE_CHECK_RETURN_F(Expr, ReturnValue, Message, ...) JE_CHECK_RETURN_CF(Expr, ReturnValue, LogCheck, Message, __VA_ARGS__)

	/** Returning check. */
	#define JE_CHECK_RETURN(Expr) JE_CHECK_RETURN_CF(Expr, , LogCheck, "")

// Loop-continue checks

	/** Loop-continue check with log category and formatted message. */
	#define JE_CHECK_CONTINUE_CF(Expr, Category, Message, ...)                   \
		if (!!(!JE_PRIVATE_CHECK_CF_IMPL(Expr, Category, Message, __VA_ARGS__))) \
			continue;                                                            \
		else                                                                     \
			volatile auto __stub = ((void*)0)

	/** Loop-continue check with log category. */
	#define JE_CHECK_CONTINUE_C(Expr, Category) JE_CHECK_CONTINUE_CF(Expr, Category, "")

	/** Loop-continue check with formatted message. */
	#define JE_CHECK_CONTINUE_F(Expr, Message, ...) JE_CHECK_CONTINUE_CF(Expr, LogCheck, Message, __VA_ARGS__)

	/** Loop-continue check. */
	#define JE_CHECK_CONTINUE(Expr) JE_CHECK_CONTINUE_CF(Expr, LogCheck, "")

// Loop-break checks

	/** Loop-break check with log category and formatted message. */
	#define JE_CHECK_BREAK_CF(Expr, Category, Message, ...)                      \
		if (!!(!JE_PRIVATE_CHECK_CF_IMPL(Expr, Category, Message, __VA_ARGS__))) \
			break;                                                               \
		else                                                                     \
			volatile auto __stub = ((void*)0)

	/** Loop-break check with log category. */
	#define JE_CHECK_BREAK_C(Expr, Category) JE_CHECK_BREAK_CF(Expr, Category, "")

	/** Loop-break check with formatted message. */
	#define JE_CHECK_BREAK_F(Expr, Message, ...) JE_CHECK_BREAK_CF(Expr, LogCheck, Message, __VA_ARGS__)

	/** Loop-break check. */
	#define JE_CHECK_BREAK(Expr) JE_CHECK_BREAK_CF(Expr, LogCheck, "")

#else
	#define JE_PRIVATE_CHECK_CF(Expr, Category, Message, ...)

	#define JE_CHECK_CF(Expr, Category, Message, ...)
	#define JE_CHECK_C(Expr, Category)
	#define JE_CHECK_F(Expr, Message, ...)
	#define JE_CHECK(Expr)

	#define JE_CHECK_RETURN_CF(Expr, ReturnValue, Category, Message, ...)
	#define JE_CHECK_RETURN_C(Expr, ReturnValue, Category)
	#define JE_CHECK_RETURN_F(Expr, ReturnValue, Message, ...)
	#define JE_CHECK_RETURN(Expr)

	#define JE_CHECK_CONTINUE_CF(Expr, Category, Message, ...)
	#define JE_CHECK_CONTINUE_C(Expr, Category)
	#define JE_CHECK_CONTINUE_F(Expr, Message, ...)
	#define JE_CHECK_CONTINUE(Expr)

	#define JE_CHECK_BREAK_CF(Expr, Category, Message, ...)
	#define JE_CHECK_BREAK_C(Expr, Category)
	#define JE_CHECK_BREAK_F(Expr, Message, ...)
	#define JE_CHECK_BREAK(Expr)
#endif // JE_USE_CHECKS