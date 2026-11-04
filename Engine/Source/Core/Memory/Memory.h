#pragma once

#include "Log/Log.h"
#include "Types/BaseTypes.h"

#include <cstring>

namespace JE
{
	namespace Memory
	{
		inline JE_API void* Allocate(uint64 _size)
		{
			void* pointer = std::malloc(_size);
			return pointer;
		}

		inline JE_API void Free(void* _pointer)
		{
			std::free(_pointer);
		}

		inline JE_API void Copy(void* _destPtr, void const* _sourcePtr, uint64 _size)
		{
			std::memcpy(_destPtr, _sourcePtr, _size);
		}
	} // namespace Memory

}; // namespace JE