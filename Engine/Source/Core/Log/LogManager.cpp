#include "LogManager.h"

#include "Assertions/Assert.h"
#include "Impls/ConsoleLoggerImpl.h"
#include "Impls/FileLoggerImpl.h"

#include <cassert>
#include <filesystem>
#include <format>

JE::FLogManager::FLogManager()
{
	DefaultFormatter = [](const FLogRecord& _logRecord)
	{
		// TODO: add colors for diff verbosities
		// TODO: fix time shift (local time)
		return std::format("[{:%d.%m.%Y %H:%M:%S}]   {:<7}   {:<20}   {}   [{}({}): {}]\n",
			_logRecord.TimePoint,
			JE::ToString(_logRecord.Verbosity),
			_logRecord.LogCategory.GetId(),
			_logRecord.Message,
			_logRecord.File,
			_logRecord.Line,
			_logRecord.Function);
	};

	FConsoleLoggerImpl* consoleLoggerImpl = new FConsoleLoggerImpl("DefaultConsoleLogger", DefaultFormatter);
	std::shared_ptr<FLoggerImpl> newLoggerImpl = RegisterLoggerImpl(consoleLoggerImpl);
	DefaultLoggerImpls.emplace_back(newLoggerImpl);

	// TODO: setup logs path from settings
	FFileLoggerImpl* fileLoggerImpl =
		new FFileLoggerImpl("DefaultFileLogger", DefaultFormatter, std::filesystem::current_path() / "../../Saved/Logs/JustEngine.log");
	newLoggerImpl = RegisterLoggerImpl(fileLoggerImpl);
	DefaultLoggerImpls.emplace_back(newLoggerImpl);
}

JE::FLogManager::~FLogManager()
{
	Loggers.clear();
	DefaultLoggerImpls.clear();

	for (auto& [implId, loggerImpl] : LoggerImpls)
	{
		// TODO: Is it possible to be used not only by Loggers?
		JE_PRIVATE_ASSERT_F(loggerImpl.use_count() == 1,
			std::format("Current logger implementation {} has multiple users.", loggerImpl->GetId()));
	}
	LoggerImpls.clear();
}

JE::FLogManager& JE::FLogManager::Get()
{
	static JE::FLogManager logManager;
	return logManager;
}

JE::FLogger& JE::FLogManager::GetLogger(const FLogCategory& _logCategory)
{
	const auto loggerIter = Loggers.find(_logCategory.GetId());
	if (loggerIter != Loggers.end())
	{
		return loggerIter->second;
	}

	JE_PRIVATE_ASSERT_F(_logCategory.IsValid(), "Invalid log category.");

	Loggers.try_emplace(_logCategory.GetId(), _logCategory, DefaultFormatter, DefaultLoggerImpls);

	return Loggers.at(_logCategory.GetId());
}

std::shared_ptr<JE::FLoggerImpl> JE::FLogManager::GetLoggerImpl(const FLoggerImpl::ID& _implId) const
{
	const auto loggerImplIter = LoggerImpls.find(_implId);
	return loggerImplIter != LoggerImpls.end() ? loggerImplIter->second : nullptr;
}

std::shared_ptr<JE::FLoggerImpl> JE::FLogManager::RegisterLoggerImpl(FLoggerImpl* _loggerImpl)
{
	if (!_loggerImpl)
	{
		return nullptr;
	}

	if (const auto loggerImpl = GetLoggerImpl(_loggerImpl->GetId()))
	{
		// TODO (?): JE_PRIVATE_ASSERT(loggerImpl.get() == _loggerImpl,
		// 	std::format("Logger implementation {} already exists during registration.", _loggerImpl->GetId()));
		return loggerImpl;
	}

	LoggerImpls.emplace(_loggerImpl->GetId(), std::shared_ptr<FLoggerImpl>(_loggerImpl));
	return LoggerImpls[_loggerImpl->GetId()];
}