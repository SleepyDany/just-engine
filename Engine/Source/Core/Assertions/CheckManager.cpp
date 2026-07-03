#include "CheckManager.h"

#include "Assert.h"
#include "Utilities/StacktraceUtilities.h"

JE::FCheckManager& JE::FCheckManager::Get()
{
	// TODO: define how to setup/configure check manager
	static JE::FCheckManager checkManager;
	return checkManager;
}

std::string JE::FCheckManager::GetStacktrace(uint32 _skipFirstEntryCount) const
{
	if (StacktraceDepth == 0)
	{
		return "[WARNING] Allowed stacktrace depth is 0.";
	}

	// TODO: skip or not?
	// skip first 2 stacktrace entries, because it will be GetStacktrace() + lambda() in CHECK macro
	IStacktraceUtility* stacktraceUtility = IStacktraceUtility::Get();
	stacktraceUtility->CollectStacktrace(StacktraceDepth, _skipFirstEntryCount);

	std::stringstream outputStacktrace;
	outputStacktrace << stacktraceUtility->GetFormattedStacktrace();

	return outputStacktrace.str();
}

bool JE::FCheckManager::ShouldPrintToLog() const
{
	return bShouldPrintToLog;
}

bool JE::FCheckManager::ShouldPrintToScreen() const
{
	return bShouldPrintToScreen;
}

bool JE::FCheckManager::ShouldBreak() const
{
	return bShouldBreak;
}