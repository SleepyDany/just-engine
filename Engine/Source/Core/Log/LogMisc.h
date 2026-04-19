#pragma once

#include "CoreDefines.h"
#include "Types/BaseTypes.h"

#include <chrono>
#include <functional>
#include <string>
#include <string_view>

namespace JE
{
	// TODO: Add colors

	enum ELogVerbosity : uint8
	{
		/** Ignore logging. */
		None = 0,

		/** Fatal status: debug break and aborts. Red. */
		Fatal,

		/** Error status: pay attention. Red. */
		Error,

		/** Warning status: be aware of that. Yellow. */
		Warning,

		/** Verbose status: Gray. */
		Log,

		/** Verbose status: Gray. */
		Verbose,

		/** Max amount of log states. */
		MAX = Verbose + 1
	};

	JE_API std::string ToString(ELogVerbosity Verbosity);

	struct JE_API FLogCategory
	{
		//- Types ------------------------
	public:
		using ID = std::string;

		//- Variables --------------------
	protected:
		/** Unique identifier for log category. */
		ID Id;

		/** Default verbosity for log category. */
		ELogVerbosity DefaultVerbosity;

		/** Actual verbosity for log category. */
		ELogVerbosity Verbosity;

		/** Should we break on each log. */
		bool bDebugBreak = false;

		//- Lifecycle --------------------
	public:
		explicit FLogCategory(const ID& _id, ELogVerbosity _defaultVerbosity);
		FLogCategory(const FLogCategory&) = default;
		FLogCategory& operator=(const FLogCategory&) = default;
		~FLogCategory() = default;

		//- Methods ----------------------
	public:
		const ID& GetId() const;

		ELogVerbosity GetDefaultVerbosity() const;
		void ResetVerbosity();

		ELogVerbosity GetVerbosity() const;
		void SetVerbosity(ELogVerbosity _newVerbosity);

		bool IsValid() const;

		bool operator==(const FLogCategory& _logCategory) const;
		bool operator!=(const FLogCategory& _logCategory) const;
	};

	struct JE_API FLogRecord
	{
		const FLogCategory& LogCategory;
		ELogVerbosity Verbosity;
		std::string Message;
		std::string_view File;
		std::string_view Function;
		uint32 Line;
		std::chrono::system_clock::time_point TimePoint;
	};

	using TLogFormatter = std::function<std::string(const FLogRecord&)>;

} // namespace JE
