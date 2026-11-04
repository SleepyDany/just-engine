#pragma once

#include "CoreDefines.h"
#include "Types/BaseTypes.h"
#include "Types/Time/DateTime.h"

#include <chrono>
#include <functional>
#include <string>
#include <string_view>

namespace JE
{
	// TODO: Add colors

	enum JE_API ELogVerbosity : uint8
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
		//- Variables --------------------
	protected:
		/** Unique identifier for log category. */
		std::string Name;

		/** Default verbosity for log category. */
		ELogVerbosity DefaultVerbosity;

		/** Actual verbosity for log category. */
		ELogVerbosity Verbosity;

		//- Lifecycle --------------------
	public:
		FLogCategory(const std::string& _name, ELogVerbosity _defaultVerbosity = ELogVerbosity::None) noexcept;
		~FLogCategory() = default;

		FLogCategory(const FLogCategory&) = default;
		FLogCategory& operator=(const FLogCategory&) = default;

		//- Methods ----------------------
	public:
		const std::string& GetName() const;

		ELogVerbosity GetDefaultVerbosity() const;

		/** Reset current verbosity to default. */
		void ResetVerbosity();
		ELogVerbosity GetVerbosity() const;
		void SetVerbosity(ELogVerbosity _newVerbosity);

		bool IsValid() const;

		bool operator==(const FLogCategory& _logCategory) const;
		bool operator!=(const FLogCategory& _logCategory) const;
	};

	using TLogFormatter = std::function<std::string(const struct FLogRecord&)>;

	struct JE_API FLogRecord
	{
		FLogCategory* Category;
		std::string Message;
		std::string_view File;
		std::string_view Function;
		FDateTime Time;
		uint32 Line;
		ELogVerbosity Verbosity;
	};

} // namespace JE
