#ifdef JE_PLATFORM_LINUX

	#include "LinuxStacktrace.h"

	#include "Assertions/Assert.h"

	#include <cxxabi.h>
	#include <dlfcn.h>
	#include <execinfo.h>

	#include <cstdio>
	#include <cstring>

JE::FLinuxStacktraceUtility::FLinuxStacktraceUtility()
	: IStacktraceUtility()
{
	// TODO: need to find out how to extract file/line
	DefaultFormatter = [](const FStacktraceEntry& _entry)
	{
		if (_entry.File.empty())
		{
			return std::format("[{}] {} [{}]", _entry.Index, _entry.Function, _entry.Address);
		}

		return std::format("[{}] {}: {} [{}]", _entry.Index, _entry.File, _entry.Function, _entry.Address);
	};
}

void JE::FLinuxStacktraceUtility::CollectStacktrace(uint32 _stacktraceDepth, uint32 _skipFirstEntryCount)
{
	Stacktrace.clear();

	std::vector<void*> bufferVector(_stacktraceDepth + _skipFirstEntryCount);
	void** buffer = bufferVector.data();
	int32 frameCount = backtrace(buffer, _stacktraceDepth + _skipFirstEntryCount);
	char** symbols = backtrace_symbols(buffer, frameCount);

	if (!symbols)
	{
		return;
	}

	Stacktrace.reserve(frameCount);
	for (uint32 frameIndex = _skipFirstEntryCount; frameIndex < frameCount; ++frameIndex)
	{
		Stacktrace.emplace_back(ExtractStacktraceEntry(buffer[frameIndex], symbols[frameIndex], frameIndex - _skipFirstEntryCount));
	}

	std::free(symbols);
}

JE::FStacktraceEntry JE::FLinuxStacktraceUtility::ExtractStacktraceEntry(void* _address, const std::any& _symbol, int32 _index) const
{
	JE_PRIVATE_ASSERT(_address);
	JE_PRIVATE_ASSERT(_symbol.has_value());

	// _symbol common view: path/exe(func+offset) [address]
	std::string mangledEntry(std::any_cast<char*>(_symbol));

	FStacktraceEntry stacktraceEntry;
	stacktraceEntry.Index = _index;

	Dl_info dlInfo;
	if (dladdr(_address, &dlInfo) && dlInfo.dli_sname)
	{
		int32 status = 0;
		std::unique_ptr<char, decltype(&std::free)> demangled(abi::__cxa_demangle(dlInfo.dli_sname, nullptr, nullptr, &status), std::free);

		stacktraceEntry.Function = status == 0 ? demangled.get() : dlInfo.dli_sname;
		// std::ptrdiff_t offset = (char*)_address - (char*)dlInfo.dli_saddr; ?

		if (dlInfo.dli_fname)
		{
			const char* fileName = std::strrchr(dlInfo.dli_fname, '/');
			stacktraceEntry.File = fileName ? fileName + 1 : dlInfo.dli_fname;
		}

		size_t openBracketIndex = mangledEntry.find_last_of('[');
		size_t closeBracketIndex = mangledEntry.find_last_of(']');
		stacktraceEntry.Address = mangledEntry.substr(openBracketIndex + 1, closeBracketIndex - openBracketIndex - 1);
	}
	else
	{
		stacktraceEntry.Function = mangledEntry;
	}

	// size_t openIndex = mangledEntry.find_last_of('(');
	// size_t plusIndex = mangledEntry.find_last_of('+');
	//
	// if (openIndex != std::string::npos && plusIndex != std::string::npos && openIndex < plusIndex)
	// {
	// 	std::string mangledFunction = mangledEntry.substr(openIndex + 1, plusIndex - openIndex - 1);
	// 	int32 status = 0;
	// 	char* demangledFunction = abi::__cxa_demangle(mangledFunction.c_str(), nullptr, nullptr, &status);
	//
	// 	if (status == 0)
	// 	{
	// 		stacktraceEntry.Function = demangledFunction;
	// 	}
	// 	else
	// 	{
	// 		stacktraceEntry.Function = mangledEntry;
	// 	}
	//
	// 	size_t openBracketIndex = mangledEntry.find_last_of('[');
	// 	size_t closeBracketIndex = mangledEntry.find_last_of(']');
	// 	stacktraceEntry.Address = mangledEntry.substr(openBracketIndex + 1, closeBracketIndex - openBracketIndex - 1);
	//
	// 	std::free(demangledFunction);
	// }
	// else
	// {
	// 	stacktraceEntry.Function = mangledEntry;
	// }

	return stacktraceEntry;
}

#endif // JE_PLATFORM_LINUX