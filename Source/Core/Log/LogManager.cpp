#include "LogManager.h"

#include "Impls/ConsoleLoggerImpl.h"
#include "Impls/FileLoggerImpl.h"

#include <cassert>

using namespace JE;

FLogManager::FLogManager()
{
	FConsoleLoggerImpl* consoleLoggerImpl = new FConsoleLoggerImpl("DefaultConsoleLogger");
	std::shared_ptr<FLoggerImpl> newLoggerImpl = RegisterLoggerImpl(consoleLoggerImpl);
	DefaultLoggerImpls.emplace_back(newLoggerImpl);

	// TODO: manage how to setup default engine root path
	// TODO: setup logs path from settings
	FFileLoggerImpl* fileLoggerImpl =
		new FFileLoggerImpl("DefaultFileLogger", std::filesystem::current_path() / "../Saved/Logs/JustEngine.log");
	newLoggerImpl = RegisterLoggerImpl(fileLoggerImpl);
	DefaultLoggerImpls.emplace_back(newLoggerImpl);

	DefaultFormatter = [](const FLogRecord& _logRecord)
	{
		// TODO: fix time shift (local time)
		// TODO: fix long file-name (full path)
		return std::format("[{:%d.%m.%Y %H:%M:%S}]   {:<7}   {:<20}   {}   [{} {}:{}]\n",
			_logRecord.TimePoint,
			JE::ToString(_logRecord.Verbosity),
			_logRecord.LogCategory.GetId(),
			_logRecord.Message,
			_logRecord.File,
			_logRecord.Function,
			_logRecord.Line);
	};
}

FLogManager::~FLogManager()
{
	Loggers.clear();
	DefaultLoggerImpls.clear();

	for (auto& [implId, loggerImpl] : LoggerImpls)
	{
		// TODO: someone is still using LoggerImpls (besides Loggers)
		assert(loggerImpl.use_count() == 1);
	}
	LoggerImpls.clear();
}

FLogManager& FLogManager::Get()
{
	static FLogManager logManager;
	return logManager;
}

FLogger& FLogManager::GetLogger(const FLogCategory& _logCategory)
{
	const auto loggerIter = Loggers.find(_logCategory.GetId());
	if (loggerIter != Loggers.end())
	{
		return loggerIter->second;
	}

	assert(_logCategory.IsValid());

	Loggers.try_emplace(_logCategory.GetId(), _logCategory, DefaultFormatter, DefaultLoggerImpls);

	return Loggers.at(_logCategory.GetId());
}

std::shared_ptr<FLoggerImpl> FLogManager::GetLoggerImpl(const FLoggerImpl::ID& _implId) const
{
	const auto loggerImplIter = LoggerImpls.find(_implId);
	return loggerImplIter != LoggerImpls.end() ? loggerImplIter->second : nullptr;
}

std::shared_ptr<FLoggerImpl> FLogManager::RegisterLoggerImpl(FLoggerImpl* _loggerImpl)
{
	if (!_loggerImpl)
	{
		return nullptr;
	}

	if (const auto loggerImpl = GetLoggerImpl(_loggerImpl->GetId()))
	{
		// TODO: assert(loggerImpl.get() == _loggerImpl)
		return loggerImpl;
	}

	LoggerImpls.emplace(_loggerImpl->GetId(), std::shared_ptr<FLoggerImpl>(_loggerImpl));
	return LoggerImpls[_loggerImpl->GetId()];
}