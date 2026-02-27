#include "LoggerImpl.h"

#include <iostream>
#include <ostream>

namespace JE
{
	// ConsoleLogSink

	FLoggerImpl::FLoggerImpl(TLogFormatter&& _logFormatter)
		: Formatter(std::move(_logFormatter))
	{
	}

	const TLogFormatter& FLoggerImpl::GetFormatter() const
	{
		return Formatter;
	}

	void FConsoleLoggerImpl::Log(const FLogDetails& _logDetails)
	{
	}

	void FConsoleLoggerImpl::Log(const std::string& _message)
	{
		std::cout << _message << std::endl;
	}

	// FileLogSink

	FFileLoggerImpl::FFileLoggerImpl(const std::filesystem::path& _path)
	{
		Path = _path;
		File.open(Path, std::ios::out | std::ios::trunc);
	}

	FFileLoggerImpl::~FFileLoggerImpl()
	{
		File.close();

		FLoggerImpl::~FLoggerImpl();
	}

	void FFileLoggerImpl::Log(const FLogDetails& _logDetails)
	{
	}

	void FFileLoggerImpl::Log(const std::string& _message)
	{
		if (File.is_open() && std::filesystem::file_size(Path) < LimitSize)
		{
			File << _message;
		}
	}
} // namespace JE