#pragma once

#include "Types/Input.h"

namespace JE
{
	// Keyboard

	struct JE_API FKeyboardEventData
	{
		int32 Mods = (int32)EKeyMode::None;
		EKey Key = EKey::None;
	};

	inline std::string ToString(const FKeyboardEventData& _data)
	{
		std::string modsString;
		for (int32 mode = 1; mode <= (int32)EKeyMode::LAST;)
		{
			if (mode & _data.Mods)
			{
				modsString += std::format("{}{}", modsString.empty() ? "" : "/", JE::ToString(static_cast<EKeyMode>(mode)));
			}
			mode <<= 1;
		}
		return std::format("Mods: {}, Key: {}", modsString, JE::ToString(_data.Key));
	}

	// Mouse

	struct JE_API FMouseButtonEventData
	{
		int32 Mods = (int32)EKeyMode::None;
		EMouseKey Key = EMouseKey::Button1;
	};

	inline std::string ToString(const FMouseButtonEventData& _data)
	{
		std::string modsString;
		for (int32 mode = 1; mode <= (int32)EKeyMode::LAST;)
		{
			if (mode & _data.Mods)
			{
				modsString += std::format("{}{}", modsString.empty() ? "" : "/", JE::ToString(static_cast<EKeyMode>(mode)));
			}
			mode <<= 1;
		}
		return std::format("Mods: {}, Key: {}", modsString, JE::ToString(_data.Key));
	}

	// Cursor

	struct JE_API FCursorMovedEventData
	{
		uint32 WindowId = 0;
		double XCoord;
		double YCoord;
	};

	inline std::string ToString(const FCursorMovedEventData& _data)
	{
		return std::format("WindowId: {}, Coords: [{}; {}]", _data.WindowId, _data.XCoord, _data.YCoord);
	}

	struct JE_API FCursorEnteredEventData
	{
		uint32 WindowId = 0;
		bool bEntered = false;
	};

	inline std::string ToString(const FCursorEnteredEventData& _data)
	{
		return std::format("WindowId: {}, Entered: {}", _data.WindowId, _data.bEntered);
	}

	struct JE_API FScrollEventData
	{
		uint32 WindowId = 0;
		double XOffset = 0.0;
		double YOffset = 0.0;
	};

	inline std::string ToString(const FScrollEventData& _data)
	{
		return std::format("WindowId: {}, Offset: [{}; {}]", _data.WindowId, _data.XOffset, _data.YOffset);
	}

} // namespace JE
