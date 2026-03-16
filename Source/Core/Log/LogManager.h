#pragma once

#include "LogMisc.h"
#include "Logger.h"
#include "LoggerImpl.h"
#include "Types/NonCopyable.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace JE
{
	class FLogManager : public FNonCopyable
	{
		//- Variables --------------------
	protected:
		std::unordered_map<FLoggerImpl::ID, std::shared_ptr<FLoggerImpl>> LoggerImpls;

		std::unordered_map<FLogCategory::ID, FLogger> Loggers;

		/** Default logger's implementations. */
		std::vector<std::shared_ptr<FLoggerImpl>> DefaultLoggerImpls;

		/** Default logger's formatter */
		TLogFormatter DefaultFormatter;

		//- Lifecycle --------------------
	protected:
		FLogManager();
		virtual ~FLogManager() override;

		//- Methods ----------------------
	public:
		static FLogManager& Get();

		FLogger& GetLogger(const FLogCategory& _logCategory);

		std::shared_ptr<FLoggerImpl> GetLoggerImpl(const FLoggerImpl::ID& _implId) const;

		/** Transfer implementation's raw pointer control to manager. It will be handled and deleted properly here. */
		std::shared_ptr<FLoggerImpl> RegisterLoggerImpl(FLoggerImpl* _loggerImpl);
	};
} // namespace JE
