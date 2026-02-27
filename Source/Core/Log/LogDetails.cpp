#include "LogDetails.h"

namespace JE
{
	FLogCategory::ID FLogCategory::GlobalId = 0;

	FLogCategory::FLogCategory()
	{
		Id = ++FLogCategory::GlobalId;
	}

	FLogCategory::ID FLogCategory::GetId() const
	{
		return Id;
	}

	std::string FLogCategory::toString() const
	{
		return "";
	}

	bool FLogCategory::operator==(const FLogCategory& _logCategory) const
	{
		return Id == _logCategory.Id;
	}

	bool FLogCategory::operator!=(const FLogCategory& _logCategory) const
	{
		return Id != _logCategory.Id;
	}
} // namespace JE