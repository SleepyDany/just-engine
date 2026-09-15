#pragma once

namespace JE
{
	/**
	 * US keyboard layout is used, re-arranged to map ASCII keys.\n
	 * For non-alphanumeric characters Unicode names are used.\n
	 * (COPY from GLFW_KEY_*)
	 */
	enum class JE_API EKey : int16
	{
		None = -1,

		// US keyboard

		SPACE = 32,
		/** ' */
		APOSTROPHE = 39,
		/** , */
		COMMA = 44,
		/** - */
		MINUS = 45,
		/** . */
		PERIOD = 46,
		/** / */
		SLASH = 47,
		/** 0 */
		_0 = 48,
		/** 1 */
		_1 = 49,
		/** 2 */
		_2 = 50,
		/** 3 */
		_3 = 51,
		/** 4 */
		_4 = 52,
		/** 5 */
		_5 = 53,
		/** 6 */
		_6 = 54,
		/** 7 */
		_7 = 55,
		/** 8 */
		_8 = 56,
		/** 9 */
		_9 = 57,
		/** ; */
		SEMICOLON = 59,
		/** = */
		EQUAL = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		/** [ */
		LEFT_BRACKET = 91,
		/** \ */
		BACKSLASH = 92,
		/** ] */
		RIGHT_BRACKET = 93,
		/** ` */
		GRAVE_ACCENT = 96,

		/** non-US #1 */
		WORLD_1 = 161,
		/** non-US #2 */
		WORLD_2 = 162,

		/* Function keys */
		ESCAPE = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DELETE = 261,
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		PAGE_UP = 266,
		PAGE_DOWN = 267,
		HOME = 268,
		END = 269,
		CAPS_LOCK = 280,
		SCROLL_LOCK = 281,
		NUM_LOCK = 282,
		PRINT_SCREEN = 283,
		PAUSE = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/** Numpad keys. */
		KP_0 = 320,
		KP_1 = 321,
		KP_2 = 322,
		KP_3 = 323,
		KP_4 = 324,
		KP_5 = 325,
		KP_6 = 326,
		KP_7 = 327,
		KP_8 = 328,
		KP_9 = 329,
		KP_DECIMAL = 330,
		KP_DIVIDE = 331,
		KP_MULTIPLY = 332,
		KP_SUBTRACT = 333,
		KP_ADD = 334,
		KP_ENTER = 335,
		KP_EQUAL = 336,

		/** Mod keys. */
		LEFT_SHIFT = 340,
		LEFT_CONTROL = 341,
		LEFT_ALT = 342,
		LEFT_SUPER = 343,
		RIGHT_SHIFT = 344,
		RIGHT_CONTROL = 345,
		RIGHT_ALT = 346,
		RIGHT_SUPER = 347,
		MENU = 348,
	};

	inline std::string ToString(EKey _key)
	{
		static std::unordered_map<EKey, std::string> keyNames{{EKey::None, "None"},
			{EKey::SPACE, "Space"},
			{EKey::APOSTROPHE, "'"},
			{EKey::COMMA, ","},
			{EKey::MINUS, "-"},
			{EKey::PERIOD, "."},
			{EKey::SLASH, "/"},
			{EKey::_0, "0"},
			{EKey::_1, "1"},
			{EKey::_2, "2"},
			{EKey::_3, "3"},
			{EKey::_4, "4"},
			{EKey::_5, "5"},
			{EKey::_6, "6"},
			{EKey::_7, "7"},
			{EKey::_8, "8"},
			{EKey::_9, "9"},
			{EKey::SEMICOLON, ";"},
			{EKey::EQUAL, "="},
			{EKey::A, "A"},
			{EKey::B, "B"},
			{EKey::C, "C"},
			{EKey::D, "D"},
			{EKey::E, "E"},
			{EKey::F, "F"},
			{EKey::G, "G"},
			{EKey::H, "H"},
			{EKey::I, "I"},
			{EKey::J, "J"},
			{EKey::K, "K"},
			{EKey::L, "L"},
			{EKey::M, "M"},
			{EKey::N, "N"},
			{EKey::O, "O"},
			{EKey::P, "P"},
			{EKey::Q, "Q"},
			{EKey::R, "R"},
			{EKey::S, "S"},
			{EKey::T, "T"},
			{EKey::U, "U"},
			{EKey::V, "V"},
			{EKey::W, "W"},
			{EKey::X, "X"},
			{EKey::Y, "Y"},
			{EKey::Z, "Z"},
			{EKey::LEFT_BRACKET, "["},
			{EKey::BACKSLASH, "\\"},
			{EKey::RIGHT_BRACKET, "/"},
			{EKey::GRAVE_ACCENT, "`"},
			{EKey::WORLD_1, "World1"},
			{EKey::WORLD_2, "World2"},
			{EKey::ESCAPE, "Esc"},
			{EKey::ENTER, "Enter"},
			{EKey::TAB, "Tab"},
			{EKey::BACKSPACE, "Backspace"},
			{EKey::INSERT, "Insert"},
			{EKey::DELETE, "Delete"},
			{EKey::RIGHT, "Right"},
			{EKey::LEFT, "Left"},
			{EKey::DOWN, "Down"},
			{EKey::UP, "Up"},
			{EKey::PAGE_UP, "Page Up"},
			{EKey::PAGE_DOWN, "Page Down"},
			{EKey::HOME, "Home"},
			{EKey::END, "End"},
			{EKey::CAPS_LOCK, "CapsLock"},
			{EKey::SCROLL_LOCK, "ScrollLock"},
			{EKey::NUM_LOCK, "NumLock"},
			{EKey::PRINT_SCREEN, "PrintScreen"},
			{EKey::PAUSE, "Pause"},
			{EKey::F1, "F1"},
			{EKey::F2, "F2"},
			{EKey::F3, "F3"},
			{EKey::F4, "F4"},
			{EKey::F5, "F5"},
			{EKey::F6, "F6"},
			{EKey::F7, "F7"},
			{EKey::F8, "F8"},
			{EKey::F9, "F9"},
			{EKey::F10, "F10"},
			{EKey::F11, "F11"},
			{EKey::F12, "F12"},
			{EKey::F13, "F13"},
			{EKey::F14, "F14"},
			{EKey::F15, "F15"},
			{EKey::F16, "F16"},
			{EKey::F17, "F17"},
			{EKey::F18, "F18"},
			{EKey::F19, "F19"},
			{EKey::F20, "F20"},
			{EKey::F21, "F21"},
			{EKey::F22, "F22"},
			{EKey::F23, "F23"},
			{EKey::F24, "F24"},
			{EKey::F25, "F25"},
			{EKey::KP_0, "Numpad 0"},
			{EKey::KP_1, "Numpad 1"},
			{EKey::KP_2, "Numpad 2"},
			{EKey::KP_3, "Numpad 3"},
			{EKey::KP_4, "Numpad 4"},
			{EKey::KP_5, "Numpad 5"},
			{EKey::KP_6, "Numpad 6"},
			{EKey::KP_7, "Numpad 7"},
			{EKey::KP_8, "Numpad 8"},
			{EKey::KP_9, "Numpad 9"},
			{EKey::KP_DECIMAL, "Numpad ."},
			{EKey::KP_DIVIDE, "Numpad /"},
			{EKey::KP_MULTIPLY, "Numpad *"},
			{EKey::KP_SUBTRACT, "Numpad -"},
			{EKey::KP_ADD, "Numpad +"},
			{EKey::KP_ENTER, "Numpad Enter"},
			{EKey::KP_EQUAL, "Numpad ="},
			{EKey::LEFT_SHIFT, "Left Shift"},
			{EKey::LEFT_CONTROL, "Left Control"},
			{EKey::LEFT_ALT, "Left Alt"},
			{EKey::LEFT_SUPER, "Left Super"},
			{EKey::RIGHT_SHIFT, "Right Shift"},
			{EKey::RIGHT_CONTROL, "Right Control"},
			{EKey::RIGHT_ALT, "Right Alt"},
			{EKey::RIGHT_SUPER, "Right Super"},
			{EKey::MENU, "Menu"}};

		return keyNames.contains(_key) ? keyNames[_key] : "None";
	}

	enum class JE_API EKeyMode : uint8
	{
		None = 0,
		Shift = 1 << 0,
		Control = 1 << 1,
		Alt = 1 << 2,
		Super = 1 << 3,
		CapsLock = 1 << 4,
		NumLock = 1 << 5,

		LAST = NumLock,
		MASK = Shift | Control | Alt | Super | CapsLock | NumLock,
	};

	inline std::string ToString(EKeyMode _mode)
	{
		static std::unordered_map<EKeyMode, std::string> modeNames{
			{EKeyMode::None, "None"},
			{EKeyMode::Shift, "Shift"},
			{EKeyMode::Control, "Control"},
			{EKeyMode::Alt, "Alt"},
			{EKeyMode::Super, "Super"},
			{EKeyMode::CapsLock, "CapsLock"},
			{EKeyMode::NumLock, "NumLock"},
		};

		return modeNames.contains(_mode) ? modeNames[_mode] : "None";
	}

	// TODO: wrap into the macro?
	inline std::underlying_type_t<EKeyMode> operator|(EKeyMode _lMode, EKeyMode _rMode)
	{
		using T = std::underlying_type_t<EKeyMode>;
		return static_cast<T>(_lMode) | static_cast<T>(_rMode);
	}

	enum JE_API EMouseKey : uint8
	{
		Button1 = 0,
		Button2,
		Button3,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8,

		LeftButton = Button1,
		RightButton = Button2,
		MiddleButton = Button3,

		LAST = Button8
	};

	inline std::string ToString(EMouseKey _mouseKey)
	{
		static std::unordered_map<EMouseKey, std::string> keyNames{
			{EMouseKey::Button1, "Button1"},
			{EMouseKey::Button2, "Button2"},
			{EMouseKey::Button3, "Button3"},
			{EMouseKey::Button4, "Button4"},
			{EMouseKey::Button5, "Button5"},
			{EMouseKey::Button6, "Button6"},
			{EMouseKey::Button7, "Button7"},
			{EMouseKey::Button8, "Button8"},
		};

		return keyNames.contains(_mouseKey) ? keyNames[_mouseKey] : "None";
	}

} // namespace JE