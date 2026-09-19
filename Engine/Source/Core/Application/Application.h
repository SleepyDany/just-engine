#pragma once

#include "CoreDefines.h"
#include "Log/Log.h"
#include "Types/BaseTypes.h"

JE_DEFINE_LOG_CATEGORY(LogApplication, Log);

namespace JE
{
	class FEvent;

	/** Base class for clients logic implementation. */
	class JE_API FApplication
	{
		//- Types ------------------------
		friend class FEngine;

		//- Events -----------------------
		//- Variables --------------------

		//- Lifecycle --------------------
	public:
		FApplication() = default;
		virtual ~FApplication() = default;

	protected:
		/** Initialize all application systems. */
		virtual bool Initialize() = 0;

		// TODO:
		/** Process available events. */
		virtual void PollEvents() = 0;

		/** Handlers during engines Run(). */
		virtual void OnEvent(FEvent& _event) = 0;
		virtual void OnUpdate(double _deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnEndFrame() = 0;

		/** Shutdown all application systems. */
		virtual void Shutdown() = 0;

		//- Methods ----------------------
	public:
		/** Does the application have windows. */
		virtual bool IsWindowed() const = 0;
		/** Does the application still running. */
		virtual bool IsRunning() const = 0;
	};

	/** Clients custom app implementation. */
	FApplication* CreateApplication();

} // namespace JE
