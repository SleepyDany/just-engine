#include "LogMisc.h"

using namespace JE;

std::string JE::ToString(ELogVerbosity Verbosity)
{
	switch (Verbosity)
	{
	case None:
		return "None";
	case Fatal:
		return "Fatal";
	case Error:
		return "Error";
	case Warning:
		return "Warning";
	case Log:
		return "Log";
	case Verbose:
		return "Verbose";
	case MAX:
		break;
	}

	return "";
}

FLogCategory::FLogCategory(const ID& _id, ELogVerbosity _defaultVerbosity)
	: Id(_id)
	, DefaultVerbosity(_defaultVerbosity)
	, Verbosity(_defaultVerbosity)
{
}

const FLogCategory::ID& FLogCategory::GetId() const
{
	return Id;
}

ELogVerbosity FLogCategory::GetDefaultVerbosity() const
{
	return DefaultVerbosity;
}

void FLogCategory::ResetVerbosity()
{
	SetVerbosity(DefaultVerbosity);
}

ELogVerbosity FLogCategory::GetVerbosity() const
{
	return Verbosity;
}

void FLogCategory::SetVerbosity(ELogVerbosity _newVerbosity)
{
	// TODO: validate verbosity
	if (Verbosity != _newVerbosity)
	{
		Verbosity = _newVerbosity;
	}
}

bool FLogCategory::IsValid() const
{
	return !Id.empty();
}

bool FLogCategory::operator==(const FLogCategory& _logCategory) const
{
	return Id == _logCategory.Id;
}

bool FLogCategory::operator!=(const FLogCategory& _logCategory) const
{
	return Id != _logCategory.Id;
}
