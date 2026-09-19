#pragma once

#include "CoreDefines.h"
#include "Memory/Memory.h"

namespace JE
{
	// TODO: add memory perf scopes
	template <uint64 StackSize>
	class JE_API FInlineAllocator
	{
		//- Variables --------------------
	protected:
		/** Current amount of memory in bytes. */
		uint64 Size;

		/** Using buffer on stack in case Size <= StackSize or dynamic allocation on heap otherwise. */
		union
		{
			uint8 Buffer[StackSize];
			void* Ptr = nullptr;
		};

		//- Lifecycle --------------------
	public:
		explicit FInlineAllocator() noexcept
			: Size(0)
		{
			static_assert(StackSize > sizeof(void*), "StackSize shouldn't be smaller than the size of the pointer.");
		}

		~FInlineAllocator() noexcept
		{
			Free();
		}

		explicit FInlineAllocator(const FInlineAllocator& _allocator) noexcept
		{
			Allocate(_allocator.GetSize());
			if (!_allocator.IsEmpty())
			{
				Memory::Copy(GetData(), _allocator.GetData(), GetSize());
			}
		}

		FInlineAllocator& operator=(const FInlineAllocator& _allocator) noexcept
		{
			if (*this == _allocator)
			{
				return *this;
			}

			Allocate(_allocator.GetSize());
			if (!_allocator.IsEmpty())
			{
				Memory::Copy(GetData(), _allocator.GetData(), GetSize());
			}

			return *this;
		}

		explicit FInlineAllocator(FInlineAllocator&& _allocator) noexcept
		{
			if (_allocator.IsStacked())
			{
				*this = _allocator;
			}
			else
			{
				Ptr = _allocator.GetData();
				Size = _allocator.GetSize();
			}

			_allocator.Size = 0;
			_allocator.Ptr = nullptr;
		}

		FInlineAllocator& operator=(FInlineAllocator&& _allocator) noexcept
		{
			if (*this == _allocator)
			{
				return *this;
			}

			if (_allocator.IsStacked())
			{
				*this = _allocator;
			}
			else
			{
				Ptr = _allocator.GetData();
				Size = _allocator.GetSize();
			}

			_allocator.Size = 0;
			_allocator.Ptr = nullptr;

			return *this;
		}

		bool operator==(const FInlineAllocator& _allocator) const noexcept
		{
			return Size == _allocator.Size && std::memcmp(GetData(), _allocator.GetData(), Size) == 0;
		}

		//- Methods ----------------------
	public:
		void* Allocate(uint64 _size)
		{
			if (Size != _size)
			{
				Free();
				Size = _size;
				if (Size > StackSize)
				{
					Ptr = Memory::Allocate(Size);
					return Ptr;
				}
			}
			return static_cast<void*>(Buffer);
		}

		void Free()
		{
			if (Size > StackSize)
			{
				Memory::Free(Ptr);
			}
			Size = 0;
			Ptr = nullptr;
		}

		void* GetData()
		{
			if (IsEmpty())
			{
				return nullptr;
			}

			return IsStacked() ? static_cast<void*>(Buffer) : Ptr;
		}

		const void* GetData() const
		{
			if (IsEmpty())
			{
				return nullptr;
			}

			return IsStacked() ? static_cast<const void*>(Buffer) : Ptr;
		}

		template <class TType = void>
		TType* Cast()
		{
			return static_cast<TType*>(GetData());
		}

		template <class TType = void>
		const TType* Cast() const
		{
			return static_cast<const TType*>(GetData());
		}

		inline uint64 GetSize() const
		{
			return Size;
		}

		inline uint64 GetStackSize() const
		{
			return StackSize;
		}

		inline bool IsStacked() const
		{
			return Size <= StackSize;
		}

		inline bool IsEmpty() const
		{
			return Size == 0;
		}
	};

} // namespace JE
