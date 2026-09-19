#pragma once

#ifdef JE_PLATFORM_LINUX
	#include "Utilities/Stacktrace/StacktraceProvider.h"

namespace JE
{
	// TODO: requires -rdynamic compile option ???
	// TODO: need to find out how to extract file/line

	class JE_API FLinuxStacktraceProvider : public IStacktraceProvider
	{
		//- Types ------------------------
		//- Events -----------------------
		//- Variables --------------------

		//- Lifecycle --------------------
	public:
		FLinuxStacktraceProvider();
		virtual ~FLinuxStacktraceProvider() override = default;

		//- Methods ----------------------
	public:
		virtual void CollectStacktrace(uint32 _stacktraceDepth, uint32 _skipFirstEntryCount) override;

	protected:
		virtual FStacktraceEntry ExtractStacktraceEntry(void* _address, const std::any& _symbol, int32 _index) const override;
	};
} // namespace JE

#endif // JE_PLATFORM_LINUX
