#pragma once

#include "Event/Event.h"

#include <nameof/nameof.hpp>

#include <sstream>

namespace sl
{

enum class CameraControllerMode : uint8_t
{
	None,
	FPS,
	Editor,
};

class CameraControllerEvent : public Event
{
public:
	CameraControllerEvent(CameraControllerMode mode) : m_mode(mode) {}

	static EventType GetStaticEventType()
	{
		return EventType::CameraController;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "CameraControllerMode: " << nameof::nameof_enum(m_mode);
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_CAMERA;
	}

	CameraControllerMode GetMode() const { return m_mode; }

private:
	CameraControllerMode m_mode;
};

} // namespace sl
