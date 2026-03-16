#include "FileLoggerImpl.h"

#include "Log/LogMacros.h"

#include <cassert>

using namespace JE;

FFileLoggerImpl::FFileLoggerImpl(const ID& _id, const std::filesystem::path& _directory, const std::string& _name)
	: FLoggerImpl(_id)
	, Directory(_directory)
	, FileName(_name)
{
	Open();
}

FFileLoggerImpl::~FFileLoggerImpl()
{
	Close();
	FLoggerImpl::~FLoggerImpl();
}

void FFileLoggerImpl::SetFileName(const std::string& _newFileName)
{
	Close();
	FileName = _newFileName;
	Open();
}

void FFileLoggerImpl::SetLimitSize(uint64 _newLimitSize)
{
	LimitSize = _newLimitSize;
	if (GetCurrentSize() < LimitSize)
	{
		Close();
		Open();
	}
}

void FFileLoggerImpl::SetFileDirectory(const std::filesystem::path& _newDirectory)
{
	Close();
	Directory = _newDirectory;
	Open();
}

const std::string& FFileLoggerImpl::GetFileName() const
{
	return FileName;
}

uint64 FFileLoggerImpl::GetCurrentSize() const
{
	return std::filesystem::file_size(Directory / FileName);
}

uint64 FFileLoggerImpl::GetLimitSize() const
{
	return LimitSize;
}

const std::filesystem::path& FFileLoggerImpl::GetDirectory() const
{
	return Directory;
}

void FFileLoggerImpl::Open()
{
	assert(!FileName.empty() && !Directory.empty());
	// TODO: check directory exists?
	File.open(Directory / FileName, std::ios::out | std::ios::trunc);
}

void FFileLoggerImpl::Close()
{
	File.close();

	// update file's name
	const auto now = std::chrono::system_clock::now();
	const std::string fullFileName = std::format("{}-{:%d.%m.%Y-%H.%M.%OS}.log", FileName, now);

	// rename with overwriting if fullFileName already exists
	std::filesystem::rename(Directory / FileName, Directory / fullFileName);
}

void FFileLoggerImpl::Log(const std::string& _message)
{
	if (File.is_open() && std::filesystem::file_size(Directory) < LimitSize)
	{
		File << _message;
	}
}