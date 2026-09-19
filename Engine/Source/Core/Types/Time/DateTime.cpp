#include "DateTime.h"

#include "Assertions/Check.h"
#include "Timespan.h"

JE::FDateTime::FDateTime(int64 _ticksSinceEpoch, bool _bZonedTime)
	: FTimeBase(_ticksSinceEpoch)
	, bZonedTime(_bZonedTime)
{
}

std::strong_ordering JE::operator<=>(const FDateTime& _lDateTime, const FDateTime& _rDateTime)
{
	JE_CHECK_CF(_lDateTime.IsZoned() == _rDateTime.IsZoned(),
		LogEngine,
		"Undefined behaviour: Trying to compare zoned and non-zoned time.");
	return _lDateTime.Ticks <=> _rDateTime.Ticks;
}

bool JE::operator==(const FDateTime& _lDateTime, const FDateTime& _rDateTime)
{
	return _lDateTime.Ticks == _rDateTime.Ticks && _lDateTime.bZonedTime == _rDateTime.bZonedTime;
}

bool JE::FDateTime::IsZoned() const
{
	return bZonedTime;
}

JE::FDateTime JE::FDateTime::Now()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::zoned_time zonedTime(std::chrono::current_zone(), now);
	std::chrono::system_clock::duration zonedDuration = zonedTime.get_local_time().time_since_epoch();

	return FDateTime(std::chrono::duration_cast<FTimeBase::TTickDuration>(zonedDuration).count(), true);
}

JE::FDateTime JE::FDateTime::UtcNow()
{
	return FDateTime(std::chrono::duration_cast<FTimeBase::TTickDuration>(std::chrono::system_clock::now().time_since_epoch()).count(),
		false);
}

JE::FDateTime& JE::FDateTime::operator-=(const FTimespan& _timespan)
{
	Ticks -= _timespan.GetTicks();
	return *this;
}

JE::FDateTime& JE::FDateTime::operator+=(const FTimespan& _timespan)
{
	Ticks += _timespan.GetTicks();
	return *this;
}

JE::FTimespan JE::operator-(const FDateTime& _lDateTime, const FDateTime& _rDateTime)
{
	return FTimespan(_lDateTime.GetTicks() - _rDateTime.GetTicks());
}

JE::FDateTime JE::operator+(const FDateTime& _dateTime, const FTimespan& _timespan)
{
	return FDateTime(_dateTime.GetTicks() + _timespan.GetTicks(), _dateTime.IsZoned());
}

JE::FDateTime JE::operator-(const FDateTime& _dateTime, const FTimespan& _timespan)
{
	return FDateTime(_dateTime.GetTicks() - _timespan.GetTicks(), _dateTime.IsZoned());
}