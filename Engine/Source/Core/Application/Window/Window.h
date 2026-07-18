#pragma once

namespace JE
{
	struct JE_API FWindowProperties
	{
		std::string Title = "JustEngine";
		uint32 Width = 1280;
		uint32 Height = 720;
	};

	class JE_API IWindow
	{
		//- Types ------------------------
		//- Events -----------------------
		//- Variables --------------------
	protected:
		/** Common window properties. */
		FWindowProperties Properties;

		/** Does window is active and has a current OpenGL context. */
		bool bIsActive = false;

		//- Lifecycle --------------------
	public:
		IWindow(const FWindowProperties& _windowProperties);
		virtual ~IWindow() = default;

		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;
		virtual void Close() = 0;

		//- Methods ----------------------
	public:
		/** Windows client implementation for specific IWindow class. */
		static IWindow* Create(const FWindowProperties& _windowProperties = {});

		virtual bool ShouldClose() const;

		virtual void SetActive(bool _bIsActive);
		bool IsActive() const;
	};
} // namespace JE
