#pragma once

#include "Core/CoreDefines.h"

namespace JE
{
	class JE_API FNonCopyable
	{
	public:
		FNonCopyable() = default;
		virtual ~FNonCopyable() = default;

	protected:
		FNonCopyable(const FNonCopyable&) = delete;
		FNonCopyable& operator=(const FNonCopyable&) = delete;
	};

} // namespace JE
