#pragma once

namespace JE
{
	struct JE_API FWindowClosedEventData
	{
		uint32 Id = 0;
	};

	inline std::string ToString(const FWindowClosedEventData& _data)
	{
		return std::format("WindowId: {}", _data.Id);
	}

	struct JE_API FWindowResizedEventData
	{
		uint32 Id = 0;
		int32 Width = 0;
		int32 Height = 0;
	};

	inline std::string ToString(const FWindowResizedEventData& _data)
	{
		return std::format("WindowId: {}, Size: [{}; {}]", _data.Id, _data.Width, _data.Height);
	}

	struct JE_API FWindowFocusedEventData
	{
		uint32 Id = 0;
		bool bFocused = false;
	};

	inline std::string ToString(const FWindowFocusedEventData& _data)
	{
		return std::format("WindowId: {}, Focused: {}", _data.Id, _data.bFocused);
	}

	struct JE_API FWindowMovedEventData
	{
		uint32 Id = 0;
		int32 XCoord = 0;
		int32 YCoord = 0;
	};

	inline std::string ToString(const FWindowMovedEventData& _data)
	{
		return std::format("WindowId: {}, Coords: [{}; {}]", _data.Id, _data.XCoord, _data.YCoord);
	}

	struct JE_API FWindowMaximizedEventData
	{
		uint32 Id = 0;
		bool bMaximized = false;
	};

	inline std::string ToString(const FWindowMaximizedEventData& _data)
	{
		return std::format("WindowId: {}, Maximized: {}", _data.Id, _data.bMaximized);
	}

	struct JE_API FWindowIconifiedEventData
	{
		uint32 Id = 0;
		bool bIconified = false;
	};

	inline std::string ToString(const FWindowIconifiedEventData& _data)
	{
		return std::format("WindowId: {}, Iconified: {}", _data.Id, _data.bIconified);
	}

} // namespace JE