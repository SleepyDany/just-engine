#pragma once

#include "TimeBase.h"

namespace JE
{
	// TODO: for performance tests it's better to use std::chrono::steady_clock. How to deal with it?

	/** Class representing timespan. */
	class JE_API FTimespan : public FTimeBase
	{
		//- Types ------------------------
		//- Events -----------------------
		//- Variables --------------------

		//- Lifecycle --------------------
	public:
		FTimespan() = default;
		~FTimespan() = default;

		explicit FTimespan(int64 _ticks);

		template <class TRep, class TPeriod>
		FTimespan(const std::chrono::duration<TRep, TPeriod>& _duration)
		{
			Ticks = std::chrono::duration_cast<FTimeBase::TTickDuration>(_duration).count();
		}

		JE_API friend std::strong_ordering operator<=>(const FTimespan& _lTimespan, const FTimespan& _rTimespan);
		JE_API friend bool operator==(const FTimespan& _lTimespan, const FTimespan& _rTimespan);

		FTimespan(const FTimespan&) = default;
		FTimespan(FTimespan&&) = default;
		FTimespan& operator=(const FTimespan&) = default;
		FTimespan& operator=(FTimespan&&) = default;

		//- Methods ----------------------
	public:
		FTimespan& operator+=(const FTimespan& _timespan);
		FTimespan& operator-=(const FTimespan& _timespan);

		JE_API friend FTimespan operator+(const FTimespan& _lTimespan, const FTimespan& _rTimespan);
		JE_API friend FTimespan operator-(const FTimespan& _lTimespan, const FTimespan& _rTimespan);
		JE_API friend double operator/(const FTimespan& _lTimespan, const FTimespan& _rTimespan);

		template <TArithmetic TValue>
		FTimespan& operator*=(TValue _value)
		{
			Ticks *= _value;
			return *this;
		}

		template <TArithmetic TValue>
		FTimespan& operator/=(TValue _value)
		{
			JE_ASSERT(_value == 0);

			Ticks /= _value;
			return *this;
		}

		template <TArithmetic TValue>
		JE_API friend FTimespan operator*(FTimespan _timespan, TValue _value)
		{
			return _timespan *= _value;
		}

		template <TArithmetic TValue>
		JE_API friend FTimespan operator/(FTimespan _timespan, TValue _value)
		{
			return _timespan /= _value;
		}
	};

} // namespace JE

template <>
struct std::formatter<JE::FTimespan>
{
	constexpr auto parse(std::format_parse_context& _ctx)
	{
		return DurationFormatter.parse(_ctx);
	}

	auto format(const JE::FTimespan& _obj, std::format_context& _ctx) const
	{
		return DurationFormatter.format(_obj.GetChronoDuration(), _ctx);
	}

private:
	std::formatter<JE::FTimeBase::TTickDuration> DurationFormatter;
};
