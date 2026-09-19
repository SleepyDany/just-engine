#include "GlfwWindow.h"

#include "Assertions/Assert.h"
#include "Engine.h"
#include "Events/Event.h"
#include "Events/InputEvents.h"
#include "Events/WindowEvents.h"
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

	glfwSetErrorCallback(
		[](int32 _errorCode, const char* _description)
		{
			JE_LOG(LogGLFW, Error, "GLFW Error {}: {}.", _errorCode, _description);
		});

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
	glfwSetWindowUserPointer(WindowInstance, this);

	// Window callbacks
	glfwSetFramebufferSizeCallback(WindowInstance,
		[](GLFWwindow* _window, int32 _width, int32 _height)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				FEvent windowResizedEvent{EEventType::WindowResized,
					EEventCategory::Window,
					FWindowResizedEventData{glfwWindow->Id, _width, _height}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(windowResizedEvent);
			}
		});

	glfwSetWindowPosCallback(WindowInstance,
		[](GLFWwindow* _window, int32 _xCoord, int32 _yCoord)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				FEvent windowMovedEvent{EEventType::WindowMoved,
					EEventCategory::Window,
					FWindowMovedEventData{glfwWindow->Id, _xCoord, _yCoord}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(windowMovedEvent);
			}
		});

	glfwSetWindowCloseCallback(WindowInstance,
		[](GLFWwindow* _window)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				FEvent windowClosedEvent{EEventType::WindowClosed, EEventCategory::Window, FWindowClosedEventData{glfwWindow->Id}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(windowClosedEvent);
			}
		});

	glfwSetWindowMaximizeCallback(WindowInstance,
		[](GLFWwindow* _window, int32 _bMaximized)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				FEvent windowMaximizedEvent{EEventType::WindowMaximized,
					EEventCategory::Window,
					FWindowMaximizedEventData{glfwWindow->Id, (bool)_bMaximized}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(windowMaximizedEvent);
			}
		});

	glfwSetWindowIconifyCallback(WindowInstance,
		[](GLFWwindow* _window, int32 _bIconified)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				FEvent windowIconifiedEvent{EEventType::WindowIconified,
					EEventCategory::Window,
					FWindowIconifiedEventData{glfwWindow->Id, (bool)_bIconified}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(windowIconifiedEvent);
			}
		});

	glfwSetWindowFocusCallback(WindowInstance,
		[](GLFWwindow* _window, int32 _bFocused)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				FEvent windowFocusedEvent{EEventType::WindowFocused,
					EEventCategory::Window,
					FWindowFocusedEventData{glfwWindow->Id, (bool)_bFocused}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(windowFocusedEvent);
			}
		});

	// Input callbacks
	glfwSetMouseButtonCallback(WindowInstance,
		[](GLFWwindow* _window, int _button, int _action, int _mods)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				int32 mouseButtonKeyMods = _mods & (int32)EKeyMode::MASK;
				EMouseKey mouseButtonKeyCode = (EMouseKey)_button;
				EEventType mouseButtonActionType = EEventType::None;
				switch (_action)
				{
				case GLFW_PRESS:
					mouseButtonActionType = EEventType::MouseBtnPressed;
					break;
				case GLFW_RELEASE:
					mouseButtonActionType = EEventType::MouseBtnReleased;
					break;
				case GLFW_REPEAT:
					mouseButtonActionType = EEventType::MouseBtnHovered;
					break;
				default:
					JE_CHECK_F(false, "Unhandled mouse button action type: {}.", _action);
					break;
				}

				FEvent mouseButtonEvent{mouseButtonActionType,
					EEventCategory::Input,
					FMouseButtonEventData{mouseButtonKeyMods, mouseButtonKeyCode}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(mouseButtonEvent);
			}
		});

	glfwSetCursorPosCallback(WindowInstance,
		[](GLFWwindow* _window, double _xPos, double _yPos)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				FEvent mouseMovedEvent{EEventType::CursorMoved, EEventCategory::Input, FCursorMovedEventData{glfwWindow->Id, _xPos, _yPos}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(mouseMovedEvent);
			}
		});

	glfwSetCursorEnterCallback(WindowInstance,
		[](GLFWwindow* _window, int32 _bEntered)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				FEvent mouseEnteredEvent{EEventType::CursorEntered,
					EEventCategory::Input,
					FCursorEnteredEventData{glfwWindow->Id, (bool)_bEntered}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(mouseEnteredEvent);
			}
		});

	glfwSetScrollCallback(WindowInstance,
		[](GLFWwindow* _window, double _xOffset, double _yOffset)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				FEvent scrollEvent{EEventType::Scroll, EEventCategory::Input, FScrollEventData{glfwWindow->Id, _xOffset, _yOffset}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(scrollEvent);
			}
		});

	glfwSetKeyCallback(WindowInstance,
		[](GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
		{
			if (FGlfwWindow* glfwWindow = static_cast<FGlfwWindow*>(glfwGetWindowUserPointer(_window)))
			{
				int32 keyMods = _mods & (int32)EKeyMode::MASK;
				EKey keyCode = (EKey)_key;
				EEventType keyActionType = EEventType::None;
				switch (_action)
				{
				case GLFW_PRESS:
					keyActionType = EEventType::KeyPressed;
					break;
				case GLFW_RELEASE:
					keyActionType = EEventType::KeyReleased;
					break;
				case GLFW_REPEAT:
					keyActionType = EEventType::KeyHovered;
					break;
				default:
					JE_CHECK_F(false, "Unhandled key action type: {}.", _action);
					break;
				}

				FEvent keyboardEvent{keyActionType, EEventCategory::Input, FKeyboardEventData{keyMods, keyCode}};

				JE_ASSERT(gEngine);
				gEngine->HandleEvent(keyboardEvent);
			}
		});

	// TODO: drop event?
}

JE::FGlfwWindow::~FGlfwWindow()
{
	glfwDestroyWindow(WindowInstance);

	// TODO: put it into the WindowManager
	glfwTerminate();
}

void JE::FGlfwWindow::PollEvents()
{
	glfwPollEvents();
}

void JE::FGlfwWindow::OnClosed(FEvent& _event)
{
	Close();
}

void JE::FGlfwWindow::OnResized(FEvent& _event)
{
}

void JE::FGlfwWindow::OnMoved(FEvent& _event)
{
}

void JE::FGlfwWindow::OnFocused(FEvent& _event)
{
}

void JE::FGlfwWindow::OnMaximized(FEvent& _event)
{
}

void JE::FGlfwWindow::OnIconified(FEvent& _event)
{
}

void JE::FGlfwWindow::SwapBuffers()
{
	glfwSwapBuffers(WindowInstance);
}

void JE::FGlfwWindow::SetSizeLimits(int32 _minWidth, int32 _minHeight, int32 _maxWidth, int32 _maxHeight)
{
	glfwSetWindowSizeLimits(WindowInstance, _minWidth, _minHeight, _maxWidth, _maxHeight);
}

void JE::FGlfwWindow::Resize(int32 _width, int32 _height)
{
	glfwSetWindowSize(WindowInstance, _width, _height);
}

void JE::FGlfwWindow::SetFocus(bool bFocus)
{
	// TODO: review it later
	glfwFocusWindow(WindowInstance);
	// OR glfwSetCurrentContext(WindowInstance); // OpenGL specific?
}

void JE::FGlfwWindow::Move(int32 _xCoord, int32 _yCoord)
{
	glfwSetWindowPos(WindowInstance, _xCoord, _yCoord);
}

void JE::FGlfwWindow::Maximize()
{
	glfwMaximizeWindow(WindowInstance);
}

void JE::FGlfwWindow::Iconify()
{
	glfwIconifyWindow(WindowInstance);
}

void JE::FGlfwWindow::Close()
{
	glfwSetWindowShouldClose(WindowInstance, GLFW_TRUE);
}

bool JE::FGlfwWindow::ShouldClose() const
{
	return glfwWindowShouldClose(WindowInstance);
}
