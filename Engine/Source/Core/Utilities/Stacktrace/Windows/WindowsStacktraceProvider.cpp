#ifdef JE_PLATFORM_WINDOWS
	#include "WindowsStacktraceProvider.h"

	#include "Assertions/Assert.h"

// clang-format off
	// include order is important
	#include <windows.h>
	#include <dbghelp.h>
// clang-format on

void JE::FWindowsStacktraceProvider::CollectStacktrace(uint32 _stacktraceDepth, uint32 _skipFirstEntryCount)
{
	Stacktrace.clear();

	// TODO: Initialize step?
	HANDLE process = GetCurrentProcess();
	HANDLE thread = GetCurrentThread();

	// initialize symbol handler
	SymInitialize(process, NULL, TRUE);
	SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
	// end initialization

	// capture stacktrace
	CONTEXT context;
	RtlCaptureContext(&context);

	// prepare stackframe
	STACKFRAME64 stackFrame;
	memset(&stackFrame, 0, sizeof(STACKFRAME64));

	stackFrame.AddrPC.Offset = context.Rip;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Offset = context.Rbp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = context.Rsp;
	stackFrame.AddrStack.Mode = AddrModeFlat;

	int32 frameIndex = 0;
	uint32 frameCapturedCount = 0;
	std::ostringstream stacktraceResult;

	while (frameCapturedCount < _stacktraceDepth)
	{
		if (!StackWalk64(IMAGE_FILE_MACHINE_AMD64,
				process,
				thread,
				&stackFrame,
				&context,
				NULL,
				SymFunctionTableAccess64,
				SymGetModuleBase64,
				NULL))
		{
			break;
		}

		// TODO: ?
		if (stackFrame.AddrPC.Offset == 0)
		{
			break;
		}

		if (frameIndex < _skipFirstEntryCount)
		{
			++frameIndex;
			continue;
		}

		Stacktrace.emplace_back(ExtractStacktraceEntry(process, stackFrame, frameIndex - _skipFirstEntryCount));
		++frameCapturedCount;
		++frameIndex;
	}

	// in pair with SymInitialize()
	SymCleanup(process);
}

JE::FStacktraceEntry JE::FWindowsStacktraceProvider::ExtractStacktraceEntry(void* _address, const std::any& _symbol, int32 _index) const
{
	JE_PRIVATE_ASSERT(_address);
	JE_PRIVATE_ASSERT(_symbol.has_value());

	FStacktraceEntry stacktraceEntry;
	stacktraceEntry.Index = _index;

	HANDLE process = _address;
	STACKFRAME64 stackFrame = std::any_cast<STACKFRAME64>(_symbol);
	// get module/file name
	char moduleName[MAX_PATH] = {0};
	HMODULE module = NULL;

	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCTSTR)stackFrame.AddrPC.Offset,
		&module);
	if (module)
	{
		GetModuleFileNameA(module, moduleName, MAX_PATH);
		char* fileName = strrchr(moduleName, '\\');
		if (fileName)
		{
			fileName++;
		}
		else
		{
			fileName = moduleName;
		}

		// TODO: ???
		stacktraceEntry.File = fileName;
	}

	// extract symbol info
	DWORD64 offset = 0;
	char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
	PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = MAX_SYM_NAME;

	if (SymFromAddr(process, stackFrame.AddrPC.Offset, &offset, symbol))
	{
		stacktraceEntry.Function = symbol->Name;
		stacktraceEntry.Address = std::format("0x{}", symbol->Address);

		// get line info
		IMAGEHLP_LINE64 line{};
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
		DWORD lineOffset = 0;

		if (SymGetLineFromAddr64(process, stackFrame.AddrPC.Offset, &lineOffset, &line))
		{
			stacktraceEntry.File = line.FileName;
			stacktraceEntry.Line = line.LineNumber;
		}
	}
	else
	{
		stacktraceEntry.Address = std::format("0x{}", stackFrame.AddrPC.Offset);
	}

	return stacktraceEntry;
}

#endif // JE_PLATFORM_WINDOWS