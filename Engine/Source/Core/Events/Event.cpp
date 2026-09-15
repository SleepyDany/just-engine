#include "Event.h"

std::string JE::ToString(EEventType _type)
{
	switch (_type)
	{
	case EEventType::None:
		return "EEventType::None";

	// Application
	case EEventType::FrameBegun:
		return "EEventType::FrameBegun";
	case EEventType::FrameUpdated:
		return "EEventType::FrameUpdated";
	case EEventType::FrameEnded:
		return "EEventType::FrameEnded";

	// Window
	case EEventType::WindowClosed:
		return "EEventType::WindowClosed";
	case EEventType::WindowResized:
		return "EEventType::WindowResized";
	case EEventType::WindowFocused:
		return "EEventType::WindowFocused";
	case EEventType::WindowMoved:
		return "EEventType::WindowMoved";
	case EEventType::WindowIconified:
		return "EEventType::WindowIconified";
	case EEventType::WindowMaximized:
		return "EEventType::WindowMaximized";

	// Input
	case EEventType::KeyPressed:
		return "EEventType::KeyPressed";
	case EEventType::KeyReleased:
		return "EEventType::KeyReleased";
	case EEventType::KeyHovered:
		return "EEventType::KeyHovered";
	case EEventType::MouseBtnPressed:
		return "EEventType::MouseBtnPressed";
	case EEventType::MouseBtnReleased:
		return "EEventType::MouseBtnReleased";
	case EEventType::MouseBtnHovered:
		return "EEventType::MouseBtnHovered";
	case EEventType::CursorMoved:
		return "EEventType::CursorMoved";
	case EEventType::CursorEntered:
		return "EEventType::CursorEntered";
	case EEventType::Scroll:
		return "EEventType::Scroll";

	default:
		return "";
	}
}

std::string JE::ToString(EEventCategory _category)
{
	switch (_category)
	{
	case EEventCategory::None:
		return "EEventCategory::None";
	case EEventCategory::Application:
		return "EEventCategory::Application";
	case EEventCategory::Window:
		return "EEventCategory::Window";
	case EEventCategory::Input:
		return "EEventCategory::Input";
	default:
		return "";
	}
}

std::string JE::ToString(const std::monostate& _obj)
{
	return "";
}

JE::FEvent::FEvent(EEventType _type, EEventCategory _category, const FEventData& _data)
	: Type(_type)
	, Category(_category)
	, Data(_data)
	, bHandled(false)
{
}

JE::EEventType JE::FEvent::GetType() const
{
	return Type;
}

JE::EEventCategory JE::FEvent::GetCategory() const
{
	return Category;
}

bool JE::FEvent::IsHandled() const
{
	return bHandled;
}

void JE::FEvent::SetHandled(bool _bHandled)
{
	bHandled = _bHandled;
}

std::string JE::FEvent::ToString() const
{
	std::string dataString = std::visit(
		[](const auto& _data)
		{
			return JE::ToString(_data);
		},
		Data);
	return std::format("Type: {}, Category: {}, Data: {}.", JE::ToString(Type), JE::ToString(Category), dataString);
}

bool JE::FEvent::IsInCategory(EEventCategory _category) const
{
	return Category == _category;
}