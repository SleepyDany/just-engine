#include "CheckManager.h"

#include "Assert.h"

#include <stacktrace>

JE::FCheckManager& JE::FCheckManager::Get()
{
	// TODO: define how to setup/configure check manager
	static JE::FCheckManager checkManager;
	return checkManager;
}

std::string JE::FCheckManager::GetStacktrace() const
{
	if (StacktraceDepth == 0)
	{
		return "[WARNING] Allowed stacktrace depth is 0.";
	}

	// TODO: skip or not?
	// skip first 2 stacktrace entries, because it will be GetStacktrace() + lambda() in CHECK macro
	std::stringstream outputStacktrace{};
	auto stacktrace = std::stacktrace::current();
	for (uint32 i = 2; i < StacktraceDepth + 2 && i < stacktrace.size(); ++i)
	{
		outputStacktrace << std::to_string(stacktrace[i]) << (((StacktraceDepth + 1) == i) ? "" : "\n");
	}

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