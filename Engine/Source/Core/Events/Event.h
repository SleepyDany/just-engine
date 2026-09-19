#pragma once

#include "CorePch.h"
#include "InputEvents.h"
#include "WindowEvents.h"

namespace JE
{
	// TODO: rework as namespace EEventType { constexpr uint32 Event = StaticValue++; }? A bit cumbersome, but allows to add custom event
	// types without editing sources.

	enum class JE_API EEventType : uint8
	{
		None = 0,

		/** Application events. */
		FrameBegun,
		FrameUpdated,
		FrameEnded,

		/** Window events. */
		WindowClosed,
		WindowResized,
		WindowFocused,
		WindowMoved,
		WindowIconified,
		WindowMaximized,

		/** Keyboard events. */
		KeyPressed,
		KeyReleased,
		KeyHovered,

		/** Mouse events. */
		MouseBtnPressed,
		MouseBtnReleased,
		MouseBtnHovered,
		// MouseBtnClicked, // TODO: How to handle?

		/** Cursor events. */
		CursorMoved,
		CursorEntered,
		Scroll,

		NUM
	};

	std::string ToString(EEventType _type);

	enum class EEventCategory : uint8
	{
		None = 0,

		/** Application events category. */
		Application = 1 << 0,

		/** Window events category. */
		Window = 1 << 1,

		/** Device input events category. */
		Input = 1 << 2
	};

	inline std::underlying_type_t<EEventCategory> operator|(EEventCategory _lCategory, EEventCategory _rCategory)
	{
		using T = std::underlying_type_t<EEventCategory>;
		return static_cast<T>(_lCategory) | static_cast<T>(_rCategory);
	}

	std::string ToString(EEventCategory _category);

	std::string ToString(const std::monostate& _obj);

	// clang-format off
	using JE_API FEventData = std::variant<std::monostate,
		// Window events
		FWindowClosedEventData,
		FWindowResizedEventData,
		FWindowFocusedEventData,
		FWindowMovedEventData,
		FWindowMaximizedEventData,
		FWindowIconifiedEventData,

		// Input events
		FMouseButtonEventData,
		FKeyboardEventData,

		FCursorMovedEventData,
		FCursorEnteredEventData,
		FScrollEventData
	>;
	// clang-format on

	class JE_API FEvent
	{
		//- Types ------------------------
		//- Events -----------------------

		//- Variables --------------------
	protected:
		FEventData Data;

		/** Specific type of the event. */
		EEventType Type = EEventType::None;

		/** Category of the event. */
		EEventCategory Category = EEventCategory::None;

		/** Whether event is handled or not. */
		bool bHandled = false;

		//- Lifecycle --------------------
	public:
		explicit FEvent(EEventType _type = EEventType::None, EEventCategory _category = EEventCategory::None, const FEventData& _data = {});
		virtual ~FEvent() = default;

		FEvent(const FEvent& _event) = default;
		FEvent& operator=(const FEvent& _event) = default;
		FEvent(FEvent&& _event) = default;
		FEvent& operator=(FEvent&& _event) = default;

		//- Methods ----------------------
	public:
		EEventType GetType() const;
		EEventCategory GetCategory() const;
		bool IsInCategory(EEventCategory _category) const;

		bool IsHandled() const;
		void SetHandled(bool _bHandled);

		std::string ToString() const;
	};

} // namespace JE
