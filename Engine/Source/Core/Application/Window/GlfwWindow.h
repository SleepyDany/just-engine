#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>

namespace JE
{
	class FGlfwWindow : public IWindow
	{
		//- Types ------------------------
		//- Events -----------------------
	public:
		virtual void OnClosed(FEvent& _event) override;
		virtual void OnResized(FEvent& _event) override;
		virtual void OnMoved(FEvent& _event) override;
		virtual void OnFocused(FEvent& _event) override;
		virtual void OnMaximized(FEvent& _event) override;
		virtual void OnIconified(FEvent& _event) override;

		//- Variables --------------------
	private:
		/** Shared GLFW initialization state. */
		static bool bGlfwInitialized;

		GLFWwindow* WindowInstance = nullptr;

		//- Lifecycle --------------------
	public:
		FGlfwWindow(const FWindowProperties& _windowProperties, GLFWwindow* _glfwWindow);
		virtual ~FGlfwWindow() override;

		virtual void PollEvents() override;
		virtual void SwapBuffers() override;

		virtual void SetSizeLimits(int32 _minWidth = 0,
			int32 _minHeight = 0,
			int32 _maxWidth = JE_DEFAULTS_WINDOW_ANYSIZE,
			int32 _maxHeight = JE_DEFAULTS_WINDOW_ANYSIZE) override;
		virtual void Resize(int32 _width, int32 _height) override;
		virtual void SetFocus(bool bFocus) override;
		virtual void Move(int32 _xCoord, int32 _yCoord) override;
		virtual void Maximize() override;
		virtual void Iconify() override;
		virtual void Close() override;

		//- Methods ----------------------
	public:
		/** Static GLFW initialization. */
		static bool GlfwInitialize();

		virtual bool ShouldClose() const override;
	};

} // namespace JE
