#pragma once

#include <any>
#include <mutex>

namespace JE
{
	/**
	 * TODO: C++23 <stacktrace> could be used instead.
	 * This interface handles situations when <stacktrace> is not supported.
	 */

	/** Single entry from the stacktrace. Not all properties could be filled, platform-specific. */
	struct JE_API FStacktraceEntry
	{
		std::string File{};
		std::string Function{};

		// hex-formatted address
		std::string Address{};

		uint64 Line{};
		uint32 Index{};
	};

	/** Stacktrace entry formatter. */
	using TStacktraceFormatter = std::function<std::string(const FStacktraceEntry&)>;

	/** Utility for collecting stacktrace. Platform-specific. */
	class JE_API IStacktraceUtility
	{
		//- Types ------------------------
		//- Events -----------------------

		//- Variables --------------------
	protected:
		std::mutex Mutex;

		std::vector<FStacktraceEntry> Stacktrace;

		TStacktraceFormatter DefaultFormatter;

		//- Lifecycle --------------------
	public:
		IStacktraceUtility();
		virtual ~IStacktraceUtility() = default;

		IStacktraceUtility(const IStacktraceUtility&) = delete;
		IStacktraceUtility& operator=(const IStacktraceUtility&) = delete;
		IStacktraceUtility(IStacktraceUtility&&) = delete;
		IStacktraceUtility& operator=(IStacktraceUtility&&) = delete;

		//- Methods ----------------------
	public:
		static IStacktraceUtility* Get();

		/** Collect @_stacktraceDepth stacktrace entries after skipping first @_skipFirstEntryCount. Could be EXPENSIVE. */
		virtual void CollectStacktrace(uint32 _stacktraceDepth, uint32 _skipFirstEntryCount = 1) = 0;

		/** Return last collected stacktrace entries. */
		const std::vector<FStacktraceEntry>& GetStacktraceEntries() const;

		/** Compile all stacktrace entries into single string using formatter. If no formatter is given, DefaultFormatter will be used. */
		std::string GetFormattedStacktrace(const TStacktraceFormatter& _formatter = nullptr) const;

	protected:
		/** Extract stacktrace entry by @_address and given @_symbol object. */
		virtual FStacktraceEntry ExtractStacktraceEntry(void* _address, const std::any& _symbol, int32 _index) const = 0;
	};
} // namespace JE
