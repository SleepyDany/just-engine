#include "FileLogger.h"

#include "Assertions/Assert.h"

#include <map>

JE::FFileLogger::FFileLogger(const std::string& _id, TLogFormatter _formatter, const std::filesystem::path& _filePath)
	: Super(_id, std::move(_formatter))
	, FilePath(_filePath)
{
	JE_PRIVATE_ASSERT_F(!_filePath.empty(), "FFileLogger needs filled path.");
	JE_PRIVATE_ASSERT_F(_filePath.has_filename(), "FFileLogger doesn't need a filename in path.");

	FilePath = _filePath;

	using namespace std::chrono_literals;
	FlushPeriod = 1s;
	LastFlushTime = FDateTime::Now();

	Open();
}

JE::FFileLogger::~FFileLogger()
{
	Close();
}

const std::filesystem::path& JE::FFileLogger::GetFilePath() const
{
	return FilePath;
}

void JE::FFileLogger::SetFilePath(const std::filesystem::path& _newFilePath)
{
	Close();

	JE_PRIVATE_ASSERT_F(!_newFilePath.empty(), "FFileLogger needs filled path.");
	JE_PRIVATE_ASSERT_F(_newFilePath.has_filename(), "FFileLogger doesn't need a filename in path.");
	FilePath = _newFilePath;

	Open();
}

uint64 JE::FFileLogger::GetMaxSize() const
{
	return MaxSize;
}

void JE::FFileLogger::SetMaxSize(uint64 _newMaxSize)
{
	MaxSize = _newMaxSize;
	if (CurrentSize >= MaxSize)
	{
		Close();
		Open();
	}
}

uint64 JE::FFileLogger::GetCurrentSize() const
{
	return CurrentSize;
}

void JE::FFileLogger::Write(std::string_view _message, bool _bForceFlush)
{
	if (File.is_open())
	{
		if (CurrentSize >= MaxSize)
		{
			Close();
			Open();
		}

		// TODO: add messages/time/size limits for flush as well?
		File << _message;
		CurrentSize += _message.size();

		const FDateTime now = FDateTime::Now();
		if (_bForceFlush || ((now - LastFlushTime) >= FlushPeriod))
		{
			File.flush();
			LastFlushTime = now;
		}
	}
}

void JE::FFileLogger::Open()
{
	std::filesystem::create_directory(FilePath.parent_path());

	// rename existing File to cached format
	if (std::filesystem::exists(FilePath))
	{
		// update file's name
		const FDateTime lastWriteTime(std::filesystem::last_write_time(FilePath), true);
		const std::string closedFileName = std::format("{}-{:%d.%m.%Y-%H.%M.%OS}{}",
			FilePath.stem().generic_string(),
			lastWriteTime,
			FilePath.extension().generic_string());

		// rename with overwriting if fullFileName already exists
		std::filesystem::rename(FilePath, FilePath.parent_path() / closedFileName);
	}

	if (RotationNumber > 1)
	{
		const std::string fileName = FilePath.stem().generic_string();
		std::map<std::filesystem::file_time_type, std::filesystem::path> logFileEntries;

		for (const auto& fileEntry : std::filesystem::directory_iterator(FilePath.parent_path()))
		{
			if (fileEntry.is_regular_file() && fileEntry.path().filename().generic_string().contains(fileName))
			{
				logFileEntries.try_emplace(fileEntry.last_write_time(), fileEntry.path());
			}
		}

		uint64 curNumber = logFileEntries.size();
		auto logFileIter = logFileEntries.begin();
		while (curNumber >= RotationNumber)
		{
			std::filesystem::remove(logFileIter->second);
			++logFileIter;
			--curNumber;
		}
	}

	File.open(FilePath, std::ios::out | std::ios::trunc);
	CurrentSize = 0;
}

void JE::FFileLogger::Close()
{
	File.close();
}