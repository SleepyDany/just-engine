#pragma once

#include "TimeBase.h"

namespace JE
{
	class FTimespan;

	/** Class representing specific time point. */
	class JE_API FDateTime : public FTimeBase
	{
		//- Variables --------------------
	protected:
		/** Whether system's time zone is used or not. */
		bool bZonedTime = false;

		//- Lifecycle --------------------
	public:
		FDateTime() = default;

		/** WARNING: _ticksSinceEpoch have to correspond with bZonedTime option. */
		explicit FDateTime(int64 _ticksSinceEpoch, bool _bZonedTime = false);

		/** WARNING: _timePoint will be converted to zoned time in case of bZonedTime = true. */
		template <class TClock, class TDuration>
		FDateTime(const std::chrono::time_point<TClock, TDuration>& _timePoint, bool _bZonedTime = false)
		{
			bZonedTime = _bZonedTime;
			if (bZonedTime)
			{
				const std::chrono::sys_time<TDuration> systemTime = std::chrono::clock_cast<std::chrono::system_clock>(_timePoint);
				const std::chrono::zoned_time zonedTime(std::chrono::current_zone(), systemTime);
				const std::chrono::system_clock::duration zonedDuration = zonedTime.get_local_time().time_since_epoch();

				Ticks = std::chrono::duration_cast<FTimeBase::TTickDuration>(zonedDuration).count();
			}
			else
			{
				const std::chrono::sys_time<TDuration> systemTime = std::chrono::clock_cast<std::chrono::system_clock>(_timePoint);
				Ticks = std::chrono::duration_cast<FTimeBase::TTickDuration>(systemTime.time_since_epoch()).count();
			}
		}

		virtual ~FDateTime() override = default;

		JE_API friend std::strong_ordering operator<=>(const FDateTime& _lDateTime, const FDateTime& _rDateTime);
		JE_API friend bool operator==(const FDateTime& _lDateTime, const FDateTime& _rDateTime);

		FDateTime(const FDateTime&) = default;
		FDateTime(FDateTime&&) = default;
		FDateTime& operator=(const FDateTime&) = default;
		FDateTime& operator=(FDateTime&&) = default;

		//- Methods ----------------------
	public:
		/** Whether system's time zone is used or not. */
		bool IsZoned() const;

		/** Get date-time considering system's time zone. */
		static FDateTime Now();

		/** Get universal non-zoned date-time. */
		static FDateTime UtcNow();

		FDateTime& operator-=(const FTimespan& _timespan);
		FDateTime& operator+=(const FTimespan& _timespan);

		JE_API friend FTimespan operator-(const FDateTime& _lDateTime, const FDateTime& _rDateTime);
		JE_API friend FDateTime operator+(const FDateTime& _dateTime, const FTimespan& _timespan);
		JE_API friend FDateTime operator-(const FDateTime& _dateTime, const FTimespan& _timespan);
	};

} // namespace JE

template <>
struct std::formatter<JE::FDateTime>
{
	constexpr auto parse(std::format_parse_context& _ctx)
	{
		return SystemTimeFormatter.parse(_ctx);
	}

	auto format(const JE::FDateTime& _obj, std::format_context& _ctx) const
	{
		return SystemTimeFormatter.format(std::chrono::system_clock::time_point(_obj.GetChronoDuration()), _ctx);
	}

private:
	std::formatter<std::chrono::system_clock::time_point> SystemTimeFormatter;
};
