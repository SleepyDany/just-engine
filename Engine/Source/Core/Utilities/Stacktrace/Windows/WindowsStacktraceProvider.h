#pragma once

#ifdef JE_PLATFORM_WINDOWS
	#include "Utilities/Stacktrace/StacktraceProvider.h"

namespace JE
{
	// TODO: link <dbghelp>
	class JE_API FWindowsStacktraceProvider : public IStacktraceProvider
	{
		//- Types ------------------------
		//- Events -----------------------
		//- Variables --------------------

		//- Lifecycle --------------------
	public:
		FWindowsStacktraceProvider() = default;
		virtual ~FWindowsStacktraceProvider() override = default;

		//- Methods ----------------------
	public:
		virtual void CollectStacktrace(uint32 _stacktraceDepth, uint32 _skipFirstEntryCount) override;

	protected:
		virtual FStacktraceEntry ExtractStacktraceEntry(void* _address, const std::any& _symbol, int32 _index) const override;
	};
} // namespace JE

#endif // JE_PLATFORM_WINDOWS
