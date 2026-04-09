#pragma once

#include <Core/CoreDefines.h>

namespace JE
{
	class JE_API FNonCopyable
	{
	protected:
		FNonCopyable() = default;
		~FNonCopyable() = default;

	private:
		FNonCopyable(const FNonCopyable&) = delete;
		FNonCopyable& operator=(const FNonCopyable&) = delete;
	};

} // namespace JE
