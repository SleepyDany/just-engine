#pragma once

#include "Application/Application.h"
#include "Delegates/DelegateMacros.h"
#include "Events/Event.h"
#include "Types/NonCopyable.h"
#include "Types/Time/DateTime.h"

#include <memory>

namespace JE
{
	/**
	 * Global Engine instance.
	 * TODO: Be aware of exporting to dll unique_ptr!
	 */
	class FEngine;
	JE_API extern std::unique_ptr<JE::FEngine> gEngine;

	JE_API FEngine& GetEngine();

	class JE_API FEngine : public FNonCopyable
	{
		//- Events -----------------------
	protected:
		JE_DEFINE_MULTICAST_DELEGATE(FGlobalEventDelegate, FEvent&);

		//- Variables --------------------
	protected:
		/** TODO: Commandline parser + console manager? */
		std::string Commandline;
		/** Parsed commandline parameters. */
		std::vector<std::string> CommandlineParameters;

		/** Client's application. */
		std::shared_ptr<FApplication> Application;
		/** Global delegates. */
		std::unordered_map<EEventType, FGlobalEventDelegate> GlobalDelegates;

		/** Frame time. */
		FDateTime EngineStartTime;
		double DeltaTime = -1.0f;
		FDateTime PrevFrameTime;

		uint64 FrameCounter = 0;
		int32 ExitCode = EXIT_SUCCESS;
		bool bIsRunning = false;

		//- Lifecycle --------------------
	public:
		FEngine(int32 _argCount, char** _argString);
		virtual ~FEngine() override;

		/** Initialize all engine systems. */
		bool Initialize();
		/** Run main loop. */
		void Run();
		/** Global application events handler. Manually binding is required. */
		void HandleEvent(FEvent& _event);
		/** Shutdown all engine systems. */
		int32 Shutdown();

	protected:
		/** Pre-Rendering logic update. */
		virtual void Update(double _deltaTime);
		/** Rendering logic handler. */
		virtual void Render();
		/** Post-Rendering logic completion. */
		virtual void EndFrame();

		//- Methods ----------------------
	public:
		/** Setup clients application. */
		void SetApplication(FApplication* _application);

		/** Get clients application. */
		std::weak_ptr<FApplication> GetApplication();

		FGlobalEventDelegate& GetGlobalDelegate(EEventType _type);
	};

} // namespace JE
