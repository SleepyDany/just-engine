#include "DelegateBase.h"

std::atomic<uint64> JE::FDelegateHandle::GlobalId = 1;

JE::FDelegateHandle::FDelegateHandle(EMode _mode)
{
	Id = _mode == EMode::Generate ? GenerateId() : InvalidId;
}

JE::FDelegateHandle::~FDelegateHandle()
{
	Reset();
}

JE::FDelegateHandle::FDelegateHandle(FDelegateHandle&& _handle) noexcept
{
	if (*this == _handle)
	{
		return;
	}

	Id = _handle.Id;
	_handle.Reset();
}

JE::FDelegateHandle& JE::FDelegateHandle::operator=(FDelegateHandle&& _handle) noexcept
{
	if (*this == _handle)
	{
		return *this;
	}

	Id = _handle.Id;
	_handle.Reset();

	return *this;
}

bool JE::FDelegateHandle::operator==(const FDelegateHandle& _handle) const
{
	return Id == _handle.Id;
}

bool JE::FDelegateHandle::operator!=(const FDelegateHandle& _handle) const
{
	return Id != _handle.Id;
}

void JE::FDelegateHandle::Reset()
{
	Id = InvalidId;
}

bool JE::FDelegateHandle::IsValid() const
{
	return Id != InvalidId;
}

uint64 JE::FDelegateHandle::GetId() const
{
	return Id;
}

uint64 JE::FDelegateHandle::GenerateId()
{
	uint64 result = GlobalId++;
	if (result == InvalidId)
	{
		result = GlobalId++;
	}

	return result;
}