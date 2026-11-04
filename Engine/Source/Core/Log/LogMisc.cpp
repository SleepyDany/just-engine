#include "LogMisc.h"

#include "Assertions/Assert.h"
#include "LogManager.h"

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

JE::FLogCategory::FLogCategory(const std::string& _name, ELogVerbosity _defaultVerbosity) noexcept
	: Name(_name)
	, DefaultVerbosity(_defaultVerbosity)
	, Verbosity(_defaultVerbosity)
{
	JE_PRIVATE_ASSERT_F(IsValid(), "Log category is invalid.");
}

const std::string& JE::FLogCategory::GetName() const
{
	return Name;
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
	return !Name.empty();
}

bool JE::FLogCategory::operator==(const FLogCategory& _logCategory) const
{
	return Name == _logCategory.Name;
}

bool JE::FLogCategory::operator!=(const FLogCategory& _logCategory) const
{
	return Name != _logCategory.Name;
}
