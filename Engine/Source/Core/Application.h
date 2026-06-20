#pragma once

#include "CoreDefines.h"
#include "Types/BaseTypes.h"

namespace JE
{
	class JE_API FApplication
	{
		//- Types ------------------------
		//- Events -----------------------
		//- Variables --------------------

		//- Lifecycle --------------------
	public:
		FApplication() = default;
		virtual ~FApplication() = default;

		/** Initialize all application systems. */
		virtual bool Initialize();

		/** Main application loop. */
		virtual void Run();

		/** Shutdown all application systems and return exit code. */
		virtual int32 Shutdown();

		//- Methods ----------------------
	};

	/** Clients custom app implementation. */
	FApplication* CreateApplication(int32 _argCount, char** _argString);

} // namespace JE
