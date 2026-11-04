#pragma once

#include "LogMisc.h"
#include "Logger.h"
#include "Types/Time/Timespan.h"

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <thread>

namespace JE
{
	class JE_API FLogManager // TODO : public FNonCopyable
	{
		//- Variables --------------------
	protected:
		std::set<std::shared_ptr<FLogger>> Loggers;

		/** Default logger's formatter. */
		TLogFormatter DefaultFormatter;

	private:
		std::queue<FLogRecord> RecordsQueue;

		std::mutex QueueMutex;
		std::thread LogThread;

		FTimespan FlushPeriod;
		std::condition_variable QueueCV;
		std::atomic_bool bStop;

		//- Lifecycle --------------------
	protected:
		FLogManager();
		~FLogManager();

	public:
		void Initialize();
		void Shutdown();

		//- Methods ----------------------
	public:
		static FLogManager& Get();

		TLogFormatter GetDefaultFormatter() const;

		std::shared_ptr<FLogger> GetLogger(const std::string& _name);
		void AddLogger(const std::shared_ptr<FLogger>& _logger);
		bool RemoveLogger(const std::shared_ptr<FLogger>& _logger);

		void Log(const FLogRecord& _logRecord);
		void Flush();

	private:
		void ProcessRecords();
	};

} // namespace JE
