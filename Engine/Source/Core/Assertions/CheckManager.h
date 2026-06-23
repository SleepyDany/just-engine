#pragma once

namespace JE
{
	class JE_API FCheckManager
	{
		//- Types ------------------------
		//- Events -----------------------

		//- Variables --------------------
	protected:
		/** Enable/disable printing check info to log. */
		bool bShouldPrintToLog = true;

		// TODO: Implement and setup
		/** Enable/disable printing check info to screen. */
		bool bShouldPrintToScreen = false;

		/** Enable/disable breakpoint on check. */
		bool bShouldBreak = false;

		/** Depth of the captured callstack. */
		uint32 StacktraceDepth = 10;

		//- Lifecycle --------------------
	public:
		FCheckManager() = default;
		~FCheckManager() = default;

		//- Methods ----------------------
	public:
		/** Get static manager instance. */
		static FCheckManager& Get();

		/** Get callstack before check was triggered. */
		std::string GetStacktrace() const;

		/** Does printing to log enabled? */
		bool ShouldPrintToLog() const;

		/** Does printing to screen enabled? */
		bool ShouldPrintToScreen() const;

		/** Is breakpoint enabled? */
		bool ShouldBreak() const;
	};
} // namespace JE
