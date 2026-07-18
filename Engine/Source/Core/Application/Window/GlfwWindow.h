#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>

namespace JE
{
	class FGlfwWindow : public IWindow
	{
		//- Types ------------------------
		//- Events -----------------------

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
		virtual void Close() override;

		//- Methods ----------------------
	public:
		virtual bool ShouldClose() const override;

		virtual void SetActive(bool _bIsActive) override;

	public:
		/** Static GLFW initialization. */
		static bool GlfwInitialize();

		/** Static GLFW Error handler. */
		static void OnGlfwError(int32 _errorCode, const char* _description);

		/** Static GLFW Resize event handler. */
		static void OnGlfwResize(GLFWwindow* _window, int32 _width, int32 _height);

		static void OnGlfwWindowClose(GLFWwindow* _window);
	};

} // namespace JE
