#pragma once

namespace JE
{
	class FEvent;

	// TODO: Move to some custom settings list later
#define JE_DEFAULTS_WINDOW_WIDTH 1280
#define JE_DEFAULTS_WINDOW_HEIGHT 720
#define JE_DEFAULTS_WINDOW_ANYSIZE -1

	struct JE_API FWindowProperties
	{
		std::string Title = "JustEngine";
		uint32 Width = JE_DEFAULTS_WINDOW_WIDTH;
		uint32 Height = JE_DEFAULTS_WINDOW_HEIGHT;

		bool bIsFocused = false;
	};

	class JE_API IWindow
	{
		//- Types ------------------------
		friend class FApplication;

		//- Events -----------------------

		/** Common window events handlers. */
	public:
		virtual void OnFocused(FEvent& _event) = 0;
		virtual void OnResized(FEvent& _event) = 0;
		virtual void OnMoved(FEvent& _event) = 0;
		virtual void OnMaximized(FEvent& _event) = 0;
		virtual void OnIconified(FEvent& _event) = 0;
		virtual void OnClosed(FEvent& _event) = 0;

		//- Variables --------------------
	protected:
		static uint32 GlobalWindowId;
		uint32 Id = 0;

		/** Common window properties. */
		FWindowProperties Properties;

		//- Lifecycle --------------------
	public:
		IWindow(const FWindowProperties& _windowProperties);
		virtual ~IWindow() = default;

		/** Common window interface. */
		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;

		virtual void SetFocus(bool bFocus) = 0;
		virtual void SetSizeLimits(int32 _minWidth = 0,
			int32 _minHeight = 0,
			int32 _maxWidth = JE_DEFAULTS_WINDOW_ANYSIZE,
			int32 _maxHeight = JE_DEFAULTS_WINDOW_ANYSIZE) = 0;
		virtual void Resize(int32 _width, int32 _height) = 0;
		virtual void Move(int32 _xCoord, int32 _yCoord) = 0;
		virtual void Maximize() = 0;
		virtual void Iconify() = 0;
		virtual void Close() = 0;

		//- Methods ----------------------
	public:
		/** Windows client implementation for specific IWindow class. */
		static IWindow* Create(const FWindowProperties& _windowProperties = {});

		uint32 GetId() const;
		FWindowProperties GetProperties() const;

		virtual bool ShouldClose() const;
		bool IsFocused() const;
	};
} // namespace JE
