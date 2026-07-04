#pragma once

#ifdef JE_PLATFORM_LINUX
	#include "Utilities/StacktraceUtilities.h"

namespace JE
{
	// TODO: requires -rdynamic compile option ???
	// TODO: need to find out how to extract file/line

	class JE_API FLinuxStacktraceUtility : public IStacktraceUtility
	{
		//- Types ------------------------
		//- Events -----------------------
		//- Variables --------------------

		//- Lifecycle --------------------
	public:
		FLinuxStacktraceUtility();
		virtual ~FLinuxStacktraceUtility() override = default;

		//- Methods ----------------------
	public:
		virtual void CollectStacktrace(uint32 _stacktraceDepth, uint32 _skipFirstEntryCount) override;

	protected:
		virtual FStacktraceEntry ExtractStacktraceEntry(void* _address, const std::any& _symbol, int32 _index) const override;
	};
} // namespace JE

#endif // JE_PLATFORM_LINUX
