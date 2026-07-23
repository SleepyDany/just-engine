#pragma once

#include "Application/Application.h"
#include "Log/LogMacros.h"
#include "Types/NonCopyable.h"
#include "Types/Time/DateTime.h"

namespace JE
{
	class IWindow;

	class JE_API FEngine : public FNonCopyable
	{
		//- Types ------------------------
		//- Events -----------------------

		//- Variables --------------------
	protected:
		/** TODO: Commandline parser + console manager? */
		std::string Commandline;

		/** Parsed commandline parameters. */
		std::vector<std::string> CommandlineParameters;

		/** Client's application. */
		std::shared_ptr<FApplication> Application;

		// TODO: rework with WindowManager
		/** Main application window. */
		std::shared_ptr<IWindow> Window;

		int32 ExitCode = EXIT_SUCCESS;
		bool bIsRunning = false;

		/** Frame time. */
		FDateTime EngineStartTime;
		float DeltaTime = -1.0f;
		FDateTime PrevFrameTime;

		uint64 FrameCounter = 0;

		//- Lifecycle --------------------
	public:
		FEngine(int32 _argCount, char** _argString);
		virtual ~FEngine() override;

		/** Initialize all engine systems. */
		virtual bool Initialize();

		/** Run main loop. */
		void Run();

		/** Shutdown all engine systems. */
		virtual int32 Shutdown();

	protected:
		// TODO:
		/** Process available events. */
		virtual void PollEvents();
		virtual void OnEvent();

		virtual void Update(float _deltaTime);
		virtual void Render();

		virtual void EndFrame();

		//- Methods ----------------------
	public:
		/** Setup clients application. */
		void SetApplication(FApplication* _application);

		/** Get clients application. */
		std::weak_ptr<FApplication> GetApplication();

		// TODO: Support multiple application windows & move to WindowManager?
		/** Create new window with specific properties. */
		// const IWindow* AddWindow(const FWindowProperties& _windowProperties, bool _bSetActive = false);

		/** Remove specific window. */
		// void RemoveWindow(IWindow* _window);

		/** Get current active window. */
		std::weak_ptr<IWindow> GetActiveWindow();
	};

	/** Global Engine instance. */
	extern std::unique_ptr<FEngine> gEngine;

} // namespace JE
