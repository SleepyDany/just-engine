#pragma once

#include "Application.h"

namespace JE
{
	/** Base console application class (without visual part). */
	class FConsoleApplication : public FApplication
	{
		//- Types ------------------------
		//- Events -----------------------
		//- Variables --------------------

		//- Lifecycle --------------------
	public:
		FConsoleApplication();
		virtual ~FConsoleApplication() override;

		//- Methods ----------------------
	public:
		virtual bool IsWindowed() const override;
	};

} // namespace JE
