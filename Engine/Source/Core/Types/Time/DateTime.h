#pragma once

#include "TimeBase.h"

namespace JE
{
	class FTimespan;

	// TODO: now we use time zone for time. Should we separate absolute UTC and zoned?

	/** Class representing specific time point. */
	class JE_API FDateTime : public FTimeBase
	{
		//- Types ------------------------
		//- Events -----------------------

		//- Variables --------------------
	protected:
		/** Whether system's time zone is used or not. */
		bool bZonedTime = false;

		//- Lifecycle --------------------
	public:
		FDateTime() = default;
		~FDateTime() = default;

		explicit FDateTime(int64 _ticksSinceEpoch, bool _bZonedTime = false);

		template <class TClock, class TDuration>
		FDateTime(const std::chrono::time_point<TClock, TDuration>& _timePoint, bool _bZonedTime = false)
		{
			Ticks = std::chrono::duration_cast<FTimeBase::TTickDuration>(_timePoint.time_since_epoch()).count();
			bZonedTime = _bZonedTime;
		}

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
