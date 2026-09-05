#pragma once

#include "Log/Log.h"

JE_DEFINE_LOG_CATEGORY(LogMemory, Log);

namespace JE
{
	namespace Memory
	{
		static inline JE_API void* Allocate(uint64 _size)
		{
			void* pointer = std::malloc(_size);
			return pointer;
		}

		static inline JE_API void Free(void* _pointer)
		{
			std::free(_pointer);
		}

		static inline JE_API void Copy(void* _destPtr, void const* _sourcePtr, uint64 _size)
		{
			std::memcpy(_destPtr, _sourcePtr, _size);
		}
	} // namespace Memory

}; // namespace JE