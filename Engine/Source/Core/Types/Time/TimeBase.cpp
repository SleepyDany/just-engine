#include "TimeBase.h"

JE::FTimeBase::FTimeBase(int64 _ticks)
	: Ticks(_ticks)
{
}

int64 JE::FTimeBase::GetTicks() const
{
	return Ticks;
}

int64 JE::FTimeBase::GetSeconds() const
{
	return GetDuration<std::chrono::seconds>();
}

double JE::FTimeBase::GetTotalSeconds() const
{
	return GetDuration<std::chrono::duration<double>>();
}

int64 JE::FTimeBase::GetMilliseconds() const
{
	return GetDuration<std::chrono::milliseconds>();
}

double JE::FTimeBase::GetTotalMilliseconds() const
{
	return GetDuration<std::chrono::duration<double, std::milli>>();
}

int64 JE::FTimeBase::GetMicroseconds() const
{
	return GetDuration<std::chrono::microseconds>();
}

double JE::FTimeBase::GetTotalMicroseconds() const
{
	return GetDuration<std::chrono::duration<double, std::micro>>();
}

int64 JE::FTimeBase::GetNanoseconds() const
{
	return GetDuration<std::chrono::nanoseconds>();
}

double JE::FTimeBase::GetTotalNanoseconds() const
{
	return GetDuration<std::chrono::duration<double, std::nano>>();
}
