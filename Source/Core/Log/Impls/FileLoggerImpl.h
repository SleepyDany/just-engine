#pragma once

#include "Log/LoggerImpl.h"
#include "Types/BaseTypes.h"

#include <filesystem>
#include <fstream>

namespace JE
{
	class JE_API FFileLoggerImpl : public FLoggerImpl
	{
		//- Variables --------------------
	protected:
		/** Working directory for this file logger. */
		std::filesystem::path Directory;

		/** Name of the file. Saved file will be renamed to 'FileName-Timepoint' at the end. */
		std::string FileName{"JustEngine"};

		/** File's stream. */
		std::ofstream File;

		/** Max size of the file in bytes. */
		uint64 LimitSize = 1024 * 1024; // 1Mb

		//- Lifecycle --------------------
	public:
		FFileLoggerImpl(const ID& _id, const std::filesystem::path& _directory, const std::string& _name = "JustEngine");
		virtual ~FFileLoggerImpl() override;

		//- Methods ----------------------
	public:
		void SetFileName(const std::string& _newFileName);
		void SetLimitSize(uint64 _newLimitSize);
		void SetFileDirectory(const std::filesystem::path& _newDirectory);

		const std::string& GetFileName() const;
		uint64 GetLimitSize() const;
		uint64 GetCurrentSize() const;
		const std::filesystem::path& GetDirectory() const;

		virtual void Log(const std::string& _message) override;

	protected:
		void Open();
		void Close();
	};
} // namespace JE
