#include "StacktraceProvider.h"

#include "CoreDefines.h"

#ifdef JE_PLATFORM_WINDOWS
	#include "Windows/WindowsStacktraceProvider.h"
#elifdef JE_PLATFORM_LINUX
	#include "Linux/LinuxStacktrace.h"
#endif

JE::IStacktraceProvider::IStacktraceProvider()
{
	DefaultFormatter = [](const FStacktraceEntry& _entry)
	{
		return std::format("[{}] {}({}): {}", _entry.Index, _entry.File, _entry.Line, _entry.Function);
	};
}

JE::IStacktraceProvider* JE::IStacktraceProvider::Get()
{
#ifdef JE_PLATFORM_WINDOWS
	static std::unique_ptr<IStacktraceProvider> stacktraceProvider = std::make_unique<FWindowsStacktraceProvider>();
#elifdef JE_PLATFORM_LINUX
	static std::unique_ptr<IStacktraceProvider> stacktraceProvider = std::make_unique<FLinuxStacktraceProvider>();
#endif
	return stacktraceProvider.get();
}

const std::vector<JE::FStacktraceEntry>& JE::IStacktraceProvider::GetStacktraceEntries() const
{
	return Stacktrace;
}

std::string JE::IStacktraceProvider::GetFormattedStacktrace(const TStacktraceFormatter& _formatter) const
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