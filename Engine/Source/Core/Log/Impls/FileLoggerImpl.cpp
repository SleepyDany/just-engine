#include "FileLoggerImpl.h"

#include "Assertions/Assert.h"
#include "Log/LogMacros.h"

JE::FFileLoggerImpl::FFileLoggerImpl(const ID& _id, const TLogFormatter& _formatter, const std::filesystem::path& _filePath)
	: Super(_id, _formatter)
	, FilePath(_filePath)
{
	JE_PRIVATE_ASSERT_F(!_filePath.empty(), "FFileLoggerImpl needs filled path.");
	JE_PRIVATE_ASSERT_F(_filePath.has_filename(), "FFileLoggerImpl doesn't need a filename in path.");

	FilePath = _filePath;

	using namespace std::chrono_literals;
	FlushPeriod = 1s;
	PrevFlushTime = FDateTime::Now();

	Open();
}

JE::FFileLoggerImpl::~FFileLoggerImpl()
{
	Close();
	Super::~FLoggerImpl();
}

const std::filesystem::path& JE::FFileLoggerImpl::GetFilePath() const
{
	return FilePath;
}

void JE::FFileLoggerImpl::SetFilePath(const std::filesystem::path& _newFilePath)
{
	Close();

	JE_PRIVATE_ASSERT_F(!_newFilePath.empty(), "FFileLoggerImpl needs filled path.");
	JE_PRIVATE_ASSERT_F(_newFilePath.has_filename(), "FFileLoggerImpl doesn't need a filename in path.");
	FilePath = _newFilePath;

	Open();
}

uint64 JE::FFileLoggerImpl::GetMaxSize() const
{
	return MaxSize;
}

void JE::FFileLoggerImpl::SetMaxSize(uint64 _newMaxSize)
{
	MaxSize = _newMaxSize;
	if (GetCurrentSize() >= MaxSize)
	{
		Close();
		Open();
	}
}

uint64 JE::FFileLoggerImpl::GetCurrentSize() const
{
	return std::filesystem::file_size(FilePath);
}

void JE::FFileLoggerImpl::Log(const std::string& _message, bool _bForceFlush)
{
	if (File.is_open())
	{
		if (GetCurrentSize() >= MaxSize)
		{
			Close();
			Open();
		}

		// TODO: add messages/time/size limits for flush as well?
		File << _message;
		FDateTime now = FDateTime::Now();
		if (_bForceFlush || (now - PrevFlushTime) >= FlushPeriod)
		{
			File.flush();
			PrevFlushTime = now;
		}
	}
}

void JE::FFileLoggerImpl::Open()
{
	std::filesystem::create_directory(FilePath.parent_path());

	// rename existing File to cached format
	if (std::filesystem::exists(FilePath))
	{
		// update file's name
		const std::string closedFileName = std::format("{}-{:%d.%m.%Y-%H.%M.%OS}{}",
			FilePath.stem().generic_string(),
			FDateTime::Now(),
			FilePath.extension().generic_string());

		// rename with overwriting if fullFileName already exists
		std::filesystem::rename(FilePath, FilePath.parent_path() / closedFileName);
	}

	if (RotationNumber > 1)
	{
		const std::string fileName = FilePath.stem().generic_string();
		std::map<std::filesystem::file_time_type, std::filesystem::path> logFileEntries;

		for (const auto& file_entry : std::filesystem::directory_iterator(FilePath.parent_path()))
		{
			if (file_entry.is_regular_file() && file_entry.path().filename().generic_string().contains(fileName))
			{
				logFileEntries.try_emplace(file_entry.last_write_time(), file_entry.path());
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
}

void JE::FFileLoggerImpl::Close()
{
	File.close();
}