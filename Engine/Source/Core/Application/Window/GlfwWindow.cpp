#include "GlfwWindow.h"

#include "Log/LogMacros.h"

#include <GLFW/glfw3.h>

JE_DEFINE_LOG_CATEGORY(LogGLFW, Log);

bool JE::FGlfwWindow::bGlfwInitialized = false;

bool JE::FGlfwWindow::GlfwInitialize()
{
	if (FGlfwWindow::bGlfwInitialized)
	{
		return true;
	}

	glfwSetErrorCallback(FGlfwWindow::OnGlfwError);

	// TODO: how to choose specific window implementation? (per-platform?)
	if (!glfwInit())
	{
		// TODO: Log
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	FGlfwWindow::bGlfwInitialized = true;
	return true;
}

void JE::FGlfwWindow::OnGlfwError(int32 _errorCode, const char* _description)
{
	JE_LOG(LogGLFW, Error, "GLFW Error {}: {}.", _errorCode, _description);
}

void JE::FGlfwWindow::OnGlfwResize(GLFWwindow* _window, int32 _width, int32 _height)
{
	if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
	{
		JE_LOG(LogGLFW,
			Warning,
			"GLFW Framebuffer resize for window '{}'. New size: {}x{}.",
			glfwWindow->Properties.Title,
			_width,
			_height);
	}
}

void JE::FGlfwWindow::OnGlfwWindowClose(GLFWwindow* _window)
{
	if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
	{
		JE_LOG(LogGLFW, Warning, "GLFW close window '{}'.", glfwWindow->Properties.Title);
		glfwWindow->Close();
	}
}

JE::IWindow* JE::IWindow::Create(const JE::FWindowProperties& _windowProperties)
{
	FGlfwWindow::GlfwInitialize();

	IWindow* window = nullptr;
	GLFWwindow* glfwWindow =
		glfwCreateWindow(_windowProperties.Width, _windowProperties.Height, _windowProperties.Title.c_str(), nullptr, nullptr);
	if (glfwWindow)
	{
		window = new FGlfwWindow(_windowProperties, glfwWindow);
	}

	return window;
}

JE::FGlfwWindow::FGlfwWindow(const FWindowProperties& _windowProperties, GLFWwindow* _glfwWindow)
	: IWindow(_windowProperties)
	, WindowInstance(_glfwWindow)
{
	glfwSetFramebufferSizeCallback(WindowInstance, FGlfwWindow::OnGlfwResize);
	glfwSetWindowUserPointer(WindowInstance, this);
	glfwSetWindowCloseCallback(WindowInstance, FGlfwWindow::OnGlfwWindowClose);
}

JE::FGlfwWindow::~FGlfwWindow()
{
	glfwDestroyWindow(WindowInstance);

	// TODO: where to put?
	glfwTerminate();
}

void JE::FGlfwWindow::PollEvents()
{
	glfwPollEvents();
}

void JE::FGlfwWindow::SwapBuffers()
{
	glfwSwapBuffers(WindowInstance);
}

void JE::FGlfwWindow::Close()
{
	glfwSetWindowShouldClose(WindowInstance, GLFW_TRUE);
}

bool JE::FGlfwWindow::ShouldClose() const
{
	return glfwWindowShouldClose(WindowInstance);
}

void JE::FGlfwWindow::SetActive(bool _bIsActive)
{
	if (_bIsActive && bIsActive != _bIsActive)
	{
		// TODO: OpenGL specific???
		glfwMakeContextCurrent(WindowInstance);
	}
	bIsActive = _bIsActive;
}