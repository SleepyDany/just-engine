#pragma once

#include "CoreDefines.h"
#include "Types/BaseTypes.h"

namespace JE
{
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
		virtual void Initialize();

		/** Handlers during engine run. */
		virtual void OnEvent();
		virtual void OnUpdate(float _deltaTime);
		virtual void OnRender();

		/** Shutdown all application systems. */
		virtual void Shutdown();

		//- Methods ----------------------
	public:
		/** Does application have windows. */
		virtual bool IsWindowed() const;
	};

	/** Clients custom app implementation. */
	FApplication* CreateApplication();

} // namespace JE
