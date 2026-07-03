#include "StacktraceUtilities.h"

#include "CoreDefines.h"

#ifdef JE_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsStacktrace.h"
#elifdef JE_PLATFORM_LINUX
	#include "Platform/Linux/LinuxStacktrace.h"
#endif

JE::IStacktraceUtility::IStacktraceUtility()
{
	DefaultFormatter = [](const FStacktraceEntry& _entry)
	{
		return std::format("[{}] {}({}): {}", _entry.Index, _entry.File, _entry.Line, _entry.Function);
	};
}

JE::IStacktraceUtility* JE::IStacktraceUtility::Get()
{
#ifdef JE_PLATFORM_WINDOWS
	static std::unique_ptr<IStacktraceUtility> stacktraceUtility = std::make_unique<FWindowsStacktraceUtility>();
#elifdef JE_PLATFORM_LINUX
	static std::unique_ptr<IStacktraceUtility> stacktraceUtility = std::make_unique<FLinuxStacktraceUtility>();
#endif
	return stacktraceUtility.get();
}

const std::vector<JE::FStacktraceEntry>& JE::IStacktraceUtility::GetStacktraceEntries() const
{
	return Stacktrace;
}

std::string JE::IStacktraceUtility::GetFormattedStacktrace(const TStacktraceFormatter& _formatter) const
{
	const TStacktraceFormatter& formatter = _formatter ? _formatter : DefaultFormatter;

	uint32 index = 1;
	std::ostringstream stacktraceRecord;
	for (const FStacktraceEntry& entry : Stacktrace)
	{
		std::string_view endOfLine = index == Stacktrace.size() ? "" : "\n";
		stacktraceRecord << formatter(entry) << endOfLine;
		++index;
	}

	return stacktraceRecord.str();
}