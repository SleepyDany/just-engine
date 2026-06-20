#pragma once

#include "Application.h"
#include "CorePch.h"

namespace JE
{
	class JE_API FEngine : public FApplication
	{
		//- Types ------------------------
		//- Events -----------------------

		//- Variables --------------------
	protected:
		std::vector<std::string> LaunchParameters;

		//- Lifecycle --------------------
	public:
		FEngine(int32 _argCount, char** _argString);
		virtual ~FEngine() override;

		virtual bool Initialize() override;
		virtual void Run() override;
		virtual int32 Shutdown() override;

		//- Methods ----------------------
	};

} // namespace JE
