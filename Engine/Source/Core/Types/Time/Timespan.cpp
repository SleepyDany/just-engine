#include "Timespan.h"

JE::FTimespan::FTimespan(int64 _ticks)
	: FTimeBase(_ticks)
{
}

std::strong_ordering JE::operator<=>(const FTimespan& _lTimespan, const FTimespan& _rTimespan)
{
	return _lTimespan.GetTicks() <=> _rTimespan.GetTicks();
}

bool JE::operator==(const FTimespan& _lTimespan, const FTimespan& _rTimespan)
{
	return _lTimespan.GetTicks() == _rTimespan.GetTicks();
}

JE::FTimespan& JE::FTimespan::operator+=(const FTimespan& _timespan)
{
	Ticks += _timespan.GetTicks();
	return *this;
}

JE::FTimespan& JE::FTimespan::operator-=(const FTimespan& _timespan)
{
	Ticks -= _timespan.GetTicks();
	return *this;
}

JE::FTimespan JE::operator+(const FTimespan& _lTimespan, const FTimespan& _rTimespan)
{
	return FTimespan(_lTimespan.GetTicks() + _rTimespan.GetTicks());
}

JE::FTimespan JE::operator-(const FTimespan& _lTimespan, const FTimespan& _rTimespan)
{
	return FTimespan(_lTimespan.GetTicks() - _rTimespan.GetTicks());
}

double JE::operator/(const FTimespan& _lTimespan, const FTimespan& _rTimespan)
{
	return static_cast<double>(_lTimespan.GetTicks()) / static_cast<double>(_rTimespan.GetTicks());
}