#include "FileLoggerImpl.h"

#include "Log/LogMacros.h"

#include <cassert>
#include <filesystem>
#include <map>
#include <set>

using namespace JE;

FFileLoggerImpl::FFileLoggerImpl(const ID& _id, const std::filesystem::path& _filePath)
	: FLoggerImpl(_id)
	, FilePath(_filePath)
{
	assert(!_filePath.empty());
	assert(_filePath.has_filename());

	FilePath = _filePath;

	Open();
}

FFileLoggerImpl::~FFileLoggerImpl()
{
	Close();
	FLoggerImpl::~FLoggerImpl();
}

const std::filesystem::path& FFileLoggerImpl::GetFilePath() const
{
	return FilePath;
}

void FFileLoggerImpl::SetFilePath(const std::filesystem::path& _newFilePath)
{
	Close();

	assert(!_newFilePath.empty());
	assert(_newFilePath.has_filename());
	FilePath = _newFilePath;

	Open();
}

uint64 FFileLoggerImpl::GetMaxSize() const
{
	return MaxSize;
}

void FFileLoggerImpl::SetMaxSize(uint64 _newMaxSize)
{
	MaxSize = _newMaxSize;
	if (GetCurrentSize() >= MaxSize)
	{
		Close();
		Open();
	}
}

uint64 FFileLoggerImpl::GetCurrentSize() const
{
	return std::filesystem::file_size(FilePath);
}

void FFileLoggerImpl::Log(const std::string& _message)
{
	if (File.is_open())
	{
		if (GetCurrentSize() >= MaxSize)
		{
			Close();
			Open();
		}

		File << _message;
		// TODO: optimize
		File.flush();
	}
}

void FFileLoggerImpl::Open()
{
	std::filesystem::create_directory(FilePath.parent_path());

	// rename existing File to cached format
	if (std::filesystem::exists(FilePath))
	{
		// update file's name
		const std::string closedFileName = std::format("{}-{:%d.%m.%Y-%H.%M.%OS}{}",
			FilePath.stem().generic_string(),
			std::chrono::system_clock::now(),
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

void FFileLoggerImpl::Close()
{
	File.close();
}