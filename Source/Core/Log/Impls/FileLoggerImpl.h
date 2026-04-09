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
		/** Working file path for this logger. */
		std::filesystem::path FilePath;

		/** File's stream. */
		std::ofstream File;

		/** Max size of the current file in bytes. */
		uint64 MaxSize = 1024 * 1024; // 1Mb

		/** Max number of files after which logger starts to rewrite older files. */
		uint32 RotationNumber = 10;

		//- Lifecycle --------------------
	public:
		FFileLoggerImpl(const ID& _id, const std::filesystem::path& _filePath);
		virtual ~FFileLoggerImpl() override;

		//- Methods ----------------------
	public:
		virtual void Log(const std::string& _message) override;

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
