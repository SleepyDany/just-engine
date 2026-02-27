#pragma once

#include <Core/CoreDefines.h>
#include <Core/Log/LogDetails.h>
#include <Core/Types/BaseTypes.h>

#include <filesystem>
#include <fstream>
#include <string>

namespace JE
{
	struct FLogDetails;

	class JE_API FLoggerImpl
	{
		//- Variables --------------------
	protected:
		TLogFormatter Formatter = nullptr;

		//- Lifecycle --------------------
	public:
		FLoggerImpl(TLogFormatter&& _logFormatter = nullptr);
		virtual ~FLoggerImpl() = default;

		const TLogFormatter& GetFormatter() const;

		//- Methods ----------------------
	public:
		virtual void Log(const FLogDetails& _logDetails) = 0;
		virtual void Log(const std::string& _message) = 0;
	};

	class JE_API FConsoleLoggerImpl : public FLoggerImpl
	{
		//- Methods ----------------------
	public:
		virtual void Log(const FLogDetails& _logDetails) override;
		virtual void Log(const std::string& _message) override;
	};

	class JE_API FFileLoggerImpl : public FLoggerImpl
	{
		//- Variables --------------------
	protected:
		std::filesystem::path Path;

		std::ofstream File;

		/** 1 Mb */
		uint32 LimitSize = 1024 * 1024;

		//- Lifecycle --------------------
	public:
		FFileLoggerImpl(const std::filesystem::path& _path);
		virtual ~FFileLoggerImpl() override;

		//- Methods ----------------------
	public:
		virtual void Log(const FLogDetails& _logDetails) override;
		virtual void Log(const std::string& _message) override;
	};
} // namespace JE
