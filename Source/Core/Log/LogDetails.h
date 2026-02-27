#pragma once

#include <Core/CoreDefines.h>
#include <Core/Types/BaseTypes.h>

#include <functional>
#include <source_location>
#include <string>

namespace JE
{
	enum ELogVerbosity : uint8
	{
		None = 0,
		Verbose,
		Log,
		Warning,
		Error,
		Fatal,

		//
		MAX
	};

	struct JE_API FLogCategory
	{
		//- Types ------------------------
	public:
		using ID = uint32;

		//- Variables --------------------
	protected:
		static ID GlobalId;
		ID Id = 0;

		//- Lifecycle --------------------
	public:
		FLogCategory();
		FLogCategory(const FLogCategory&) = default;
		FLogCategory& operator=(const FLogCategory&) = default;
		~FLogCategory() = default;

		//- Methods ----------------------
	public:
		inline ID GetId() const;
		inline std::string toString() const;

		bool operator==(const FLogCategory& _logCategory) const;
		bool operator!=(const FLogCategory& _logCategory) const;
	};

	struct JE_API FLogDetails
	{
		ELogVerbosity Verbosity;
		std::string Message;
		std::source_location SourceLocation;
	};

	using TLogFormatter = std::function<std::string(const FLogDetails&)>;

} // namespace JE
