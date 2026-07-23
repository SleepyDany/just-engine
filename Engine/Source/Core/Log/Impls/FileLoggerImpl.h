#pragma once

#include "Log/LoggerImpl.h"

#include "Types/BaseTypes.h"
#include "Types/Time/Timespan.h"

#include <filesystem>
#include <fstream>

namespace JE
{
	class JE_API FFileLoggerImpl : public FLoggerImpl
	{
		//- Types ------------------------
	private:
		using Super = FLoggerImpl;

		//- Variables --------------------
	protected:
		/** Working file path for this logger. */
		std::filesystem::path FilePath;

		/** File's stream. */
		std::ofstream File;

		/** Max size of the current file in bytes. */
		uint64 MaxSize = 10 * 1024 * 1024; // 10Mb

		/** Max number of files after which logger starts to rewrite older files. */
		uint32 RotationNumber = 10;

		/** Time period to flush ordinary messages. */
		FTimespan FlushPeriod;

		/** Timepoint for previous flush. */
		FDateTime PrevFlushTime;

		//- Lifecycle --------------------
	public:
		FFileLoggerImpl(const ID& _id, const TLogFormatter& _formatter, const std::filesystem::path& _filePath);
		virtual ~FFileLoggerImpl() override;

		//- Methods ----------------------
	public:
		virtual void Log(const std::string& _message, bool _bForceFlush) override;

		const std::filesystem::path& GetFilePath() const;
		void SetFilePath(const std::filesystem::path& _newFilePath);

		uint64 GetMaxSize() const;
		void SetMaxSize(uint64 _newMaxSize);

		uint64 GetCurrentSize() const;

	protected:
		void Open();
		void Close();
	};
} // namespace JE
