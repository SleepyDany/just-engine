#pragma once

#include "Application.h"

namespace JE
{
	/** Base windowed application class. */
	class JE_API FWindowApplication : public FApplication
	{
		//- Types ------------------------
		//- Events -----------------------
		//- Variables --------------------

		//- Lifecycle --------------------
	public:
		FWindowApplication();
		virtual ~FWindowApplication() override;

	protected:
		virtual void Initialize() override;

		virtual void OnEvent() override;
		virtual void OnUpdate(float _deltaTime) override;
		virtual void OnRender() override;

		virtual void Shutdown() override;

		//- Methods ----------------------
	};

} // namespace JE
