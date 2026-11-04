#include "LogManager.h"

#include "Assertions/Assert.h"
#include "Loggers/FileLogger.h"
#include "Loggers/StdoutLogger.h"

#include <filesystem>
#include <format>

JE::FLogManager::FLogManager()
{
	DefaultFormatter = [](const FLogRecord& _logRecord)
	{
		// TODO: add colors for diff verbosities
		return std::format("[{:%d.%m.%Y %H:%M:%S}]   {:<7}   {:<20}   {}   [{}({}): {}]\n",
			_logRecord.Time,
			JE::ToString(_logRecord.Verbosity),
			_logRecord.Category->GetName(),
			_logRecord.Message,
			_logRecord.File,
			_logRecord.Line,
			_logRecord.Function);
	};

	bStop = false;
}

JE::FLogManager::~FLogManager()
{
}

void JE::FLogManager::Initialize()
{
	// TODO: setup logs path from settings
	AddLogger(std::make_shared<FStdoutLogger>("DefaultStdoutLogger", DefaultFormatter));
	AddLogger(std::make_shared<FFileLogger>("DefaultFileLogger",
		DefaultFormatter,
		std::filesystem::current_path() / "../../Saved/Logs/JustEngine.log"));

	// TODO: setup flush period from settings and support immediate logging?
	FlushPeriod = std::chrono::milliseconds(100);
	LogThread = std::thread(&FLogManager::ProcessRecords, this);
}

void JE::FLogManager::Shutdown()
{
	bStop = true;
	QueueCV.notify_all();

	if (LogThread.joinable())
	{
		LogThread.join();
	}

	for (const auto& logger : Loggers)
	{
		JE_PRIVATE_ASSERT_F(logger.use_count() == 1, std::format("Current logger '{}' has multiple users.", logger->GetName()));
	}
	Loggers.clear();
}

JE::FLogManager& JE::FLogManager::Get()
{
	static FLogManager logManager;
	return logManager;
}

JE::TLogFormatter JE::FLogManager::GetDefaultFormatter() const
{
	return DefaultFormatter;
}

void JE::FLogManager::Log(const FLogRecord& _logRecord)
{
	// TODO: probably should batch multiple records if possible
	std::scoped_lock<std::mutex> lock(QueueMutex);
	RecordsQueue.push(_logRecord);

	// QueueCV.notify_one();
}

void JE::FLogManager::Flush()
{
	QueueCV.notify_one();
}

void JE::FLogManager::AddLogger(const std::shared_ptr<FLogger>& _logger)
{
	if (_logger)
	{
		Loggers.emplace(_logger);
	}
}

bool JE::FLogManager::RemoveLogger(const std::shared_ptr<FLogger>& _logger)
{
	if (_logger)
	{
		return Loggers.erase(_logger) > 0;
	}

	return false;
}

std::shared_ptr<JE::FLogger> JE::FLogManager::GetLogger(const std::string& _name)
{
	const auto loggerIter = std::ranges::find_if(Loggers,
		[&_name](const std::shared_ptr<FLogger>& _logger)
		{
			return _logger && _logger->GetName() == _name;
		});
	return loggerIter == Loggers.end() ? nullptr : *loggerIter;
}

void JE::FLogManager::ProcessRecords()
{
	while (!bStop)
	{
		std::unique_lock<std::mutex> lock(QueueMutex);
		QueueCV.wait_for(lock,
			FlushPeriod.GetChronoDuration(),
			[&]()
			{
				return bStop || !RecordsQueue.empty();
			});

		if (bStop && RecordsQueue.empty())
		{
			return;
		}

		std::queue<FLogRecord> localRecords;
		localRecords.swap(RecordsQueue);
		lock.unlock();

		while (!localRecords.empty())
		{
			const FLogRecord& logRecord = localRecords.front();
			for (const auto& logger : Loggers)
			{
				if (logger && logger->IsSupported(*logRecord.Category))
				{
					logger->Write(logRecord);
				}
			}
			localRecords.pop();
		}
	}
}