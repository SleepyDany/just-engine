#pragma once

#include <chrono>

namespace JE
{
	// TODO: conversion to hours/days/weeks/months/years
	// TODO: how to support seconds in current minute/round total seconds/total seconds; etc
	// TODO: validate/test conversions/arithmetic/operations

	/** Base class representing time. */
	class JE_API FTimeBase
	{
		//- Types ------------------------
	public:
		/** 1 tick equals 100 nanoseconds. */
		using TTickDuration = std::chrono::duration<int64, std::ratio<100, 1'000'000'000>>;

		//- Events -----------------------

		//- Variables --------------------
	protected:
		int64 Ticks = 0;

		//- Lifecycle --------------------
	public:
		/** Delete compare operator because it's children may be incomparable. */
		friend std::strong_ordering operator<=>(const FTimeBase&, const FTimeBase&) = delete;

	protected:
		/** Use children classes instead. */
		FTimeBase() = default;
		explicit FTimeBase(int64 _ticks);

		//- Methods ----------------------
	public:
		int64 GetTicks() const;

		/** Get count from custom duration. */
		template <class TCustomDuration = TTickDuration>
		auto GetDuration() const
		{
			return GetChronoDuration<TCustomDuration>().count();
		}

		/** Get time as custom std::chrono::duration. */
		template <class TCustomDuration = TTickDuration>
		auto GetChronoDuration() const
		{
			return std::chrono::duration_cast<TCustomDuration>(TTickDuration(Ticks));
		}

		int64 GetSeconds() const;
		double GetTotalSeconds() const;

		int64 GetMilliseconds() const;
		double GetTotalMilliseconds() const;

		int64 GetMicroseconds() const;
		double GetTotalMicroseconds() const;

		int64 GetNanoseconds() const;
		double GetTotalNanoseconds() const;
	};

} // namespace JE
