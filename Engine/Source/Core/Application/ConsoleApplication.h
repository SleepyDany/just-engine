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

	protected:
		virtual bool Initialize() override;
		virtual void PollEvents() override;

		virtual void OnEvent(FEvent& _event) override;
		virtual void OnUpdate(double _deltaTime) override;
		virtual void OnRender() override;
		virtual void OnEndFrame() override;

		virtual void Shutdown() override;

		//- Methods ----------------------
	public:
		virtual bool IsWindowed() const override;
		/** One-shot update of the application by default. */
		virtual bool IsRunning() const override;
	};

} // namespace JE
