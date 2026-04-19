#pragma once

// Platforms-specific definitions
#if defined(_WIN32) || defined(_WIN64)
	#define JE_PLATFORM_WINDOWS
#elif defined(__linux__)
	#define JE_PLATFORM_LINUX
#else
	#error Platform is unsupported.
#endif

// Compiler-specific definitions
#if defined(_MSC_VER)
	#define JE_COMPILER_MSVC
#elif defined(__clang__)
	#define JE_COMPILER_CLANG
#elif defined(__GNUC__)
	#define JE_COMPILER_GCC
#else
	#error C++ compiler is unsupported.
#endif

#ifdef JE_COMPILER_MSVC
	#define JE_PLATFORM_BREAK() (__nop(), __debugbreak())
#else
	#include <signal.h>
	#define JE_PLATFORM_BREAK() raise(SIGTRAP)
#endif

// Import/Export definitions
#if defined(JE_SHARED_LIBRARY)
	#if defined(JE_EXPORT)
		#if defined(JE_COMPILER_MSVC)
			#define JE_API __declspec(dllexport)
		#elif defined(JE_COMPILER_GCC) || defined(JE_COMPILER_CLANG)
			#define JE_API __atribute__((visibility("default")))
		#endif
	#else
		#if defined(JE_COMPILER_MSVC)
			#define JE_API __declspec(dllimport)
		#elif defined(JE_COMPILER_GCC) || defined(JE_COMPILER_CLANG)
			#define JE_API __atribute__((visibility("default")))
		#endif
	#endif
#else
	#define JE_STATIC_LIBRARY
	#define JE_API
#endif

// Deprecated
#if defined(JE_COMPILER_MSVC)
	#define JE_DEPRECATED(Message) __declspec(deprecated(Message))
#elif defined(JE_COMPILER_GCC) || defined(JE_COMPILER_CLANG)
	#define JE_DEPRECATED(Message) __attribute__((deprecated(Message)))
#endif
