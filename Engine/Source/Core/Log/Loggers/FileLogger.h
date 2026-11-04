#pragma once

#include "Log/Logger.h"

#include "Types/BaseTypes.h"
#include "Types/Time/Timespan.h"

#include <filesystem>
#include <fstream>

namespace JE
{
	class JE_API FFileLogger : public FLogger
	{
		//- Types ------------------------
	private:
		using Super = FLogger;

		//- Variables --------------------
	protected:
		/** Working file path for this logger. */
		std::filesystem::path FilePath;

		/** File's stream. */
		std::ofstream File;

		/** Max size of the current file in bytes. */
		uint64 MaxSize = 10ull * 1024 * 1024; // 10Mb

		/** Current file size in bytes. */
		uint64 CurrentSize = 0ull;

		/** Max number of files after which logger starts to rewrite older files. */
		uint32 RotationNumber = 10;

		/** Time period to flush ordinary messages. */
		FTimespan FlushPeriod;

		/** Timepoint for previous flush. */
		FDateTime LastFlushTime;

		//- Lifecycle --------------------
	public:
		FFileLogger(const std::string& _id, TLogFormatter _formatter, const std::filesystem::path& _filePath);
		virtual ~FFileLogger() override;

		//- Methods ----------------------
	public:
		const std::filesystem::path& GetFilePath() const;
		void SetFilePath(const std::filesystem::path& _newFilePath);

		uint64 GetMaxSize() const;
		void SetMaxSize(uint64 _newMaxSize);

		uint64 GetCurrentSize() const;

	protected:
		virtual void Write(std::string_view _message, bool _bForceFlush) override;

	protected:
		void Open();
		void Close();
	};
} // namespace JE
