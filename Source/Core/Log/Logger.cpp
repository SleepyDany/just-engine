#include "Logger.h"

#include "LoggerImpl.h"

#include <Log/LogDetails.h>

namespace JE
{
	template <class TLogCategory>
	FLogger<TLogCategory>::FLogger(ELogVerbosity _verbosity, TLogFormatter&& _formatter)
		: Verbosity(_verbosity)
		, DefaultFormatter(std::move(_formatter))
	{
	}

	template <class TLogCategory>
	FLogger<TLogCategory>& FLogger<TLogCategory>::GetInstance()
	{
		static FLogger<TLogCategory> instance;
		return instance;
	}

	template <class TLogCategory>
	ELogVerbosity FLogger<TLogCategory>::GetVerbosity() const
	{
		return Verbosity;
	}

	template <class TLogCategory>
	void FLogger<TLogCategory>::SetVerbosity(ELogVerbosity _verbosity)
	{
		Verbosity = _verbosity;
	}

	template <class TLogCategory>
	FLogCategory::ID FLogger<TLogCategory>::GetCategoryId() const
	{
		return Category.GetId();
	}

	template <class TLogCategory>
	const TLogFormatter& FLogger<TLogCategory>::GetDefaultFormatter() const
	{
		return DefaultFormatter;
	}

	template <class TLogCategory>
	void FLogger<TLogCategory>::SetDefaultFormatter(TLogFormatter&& _defaultFormatter)
	{
		DefaultFormatter = std::move(_defaultFormatter);
	}

	template <class TLogCategory>
	void FLogger<TLogCategory>::Log(const FLogDetails& _logDetails)
	{
		if (_logDetails.Verbosity < Verbosity)
		{
			return;
		}

		for (auto& impl : LoggerImpls)
		{
			if (!impl)
			{
				continue;
			}

			const TLogFormatter& formatter = impl->GetFormatter() ? impl->GetFormatter() : DefaultFormatter;
			impl->Log(formatter(_logDetails));
		}

		if (Verbosity == ELogVerbosity::Fatal || bAlwaysBreakOnLog)
		{
			JE_PLATFORM_BREAK();
		}
	}

	template <class TLogCategory>
	void FLogger<TLogCategory>::RegisterImpl(const std::shared_ptr<FLoggerImpl>& _logImpl)
	{
		LoggerImpls.emplace_back(_logImpl);
	}

	template <class TLogCategory>
	void FLogger<TLogCategory>::UnregisterImpl(const std::shared_ptr<FLoggerImpl>& _logImpl)
	{
		LoggerImpls.erase(std::remove(LoggerImpls.begin(), LoggerImpls.end(), _logImpl), LoggerImpls.end());
	}
} // namespace JE