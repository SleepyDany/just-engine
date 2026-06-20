#include "LogMisc.h"

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

JE::FLogCategory::FLogCategory(const ID& _id, ELogVerbosity _defaultVerbosity)
	: Id(_id)
	, DefaultVerbosity(_defaultVerbosity)
	, Verbosity(_defaultVerbosity)
{
}

const JE::FLogCategory::ID& JE::FLogCategory::GetId() const
{
	return Id;
}

JE::ELogVerbosity JE::FLogCategory::GetDefaultVerbosity() const
{
	return DefaultVerbosity;
}

void JE::FLogCategory::ResetVerbosity()
{
	SetVerbosity(DefaultVerbosity);
}

JE::ELogVerbosity JE::FLogCategory::GetVerbosity() const
{
	return Verbosity;
}

void JE::FLogCategory::SetVerbosity(ELogVerbosity _newVerbosity)
{
	// TODO: validate verbosity
	if (Verbosity != _newVerbosity)
	{
		Verbosity = _newVerbosity;
	}
}

bool JE::FLogCategory::IsValid() const
{
	return !Id.empty();
}

bool JE::FLogCategory::operator==(const FLogCategory& _logCategory) const
{
	return Id == _logCategory.Id;
}

bool JE::FLogCategory::operator!=(const FLogCategory& _logCategory) const
{
	return Id != _logCategory.Id;
}
