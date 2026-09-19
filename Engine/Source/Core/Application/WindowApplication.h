#pragma once

#include "Application.h"

namespace JE
{
	class IWindow;

	/** Base windowed application class. */
	class JE_API FWindowApplication : public FApplication
	{
		//- Types ------------------------
		//- Events -----------------------

		//- Variables --------------------
	protected:
		// TODO: rework with WindowManager
		std::unordered_map<uint32, std::shared_ptr<IWindow>> Windows;

		uint32 CurrentWindowId = 0;

		//- Lifecycle --------------------
	public:
		FWindowApplication();
		virtual ~FWindowApplication() override;

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
		virtual bool IsRunning() const override;

		// TODO: Support multiple application windows & move to WindowManager?
		std::shared_ptr<IWindow> GetCurrentWindow() const;

		/** Create new window with specific properties. */
		void AddWindow(const std::shared_ptr<IWindow>& _window);

		/** Remove specific window. */
		bool RemoveWindow(uint32 _windowId);
	};

} // namespace JE
