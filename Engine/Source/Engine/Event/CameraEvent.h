#pragma once

#include "Core/Defines.h"
#include "Event/Event.h"

#include <nameof/nameof.hpp>

#include <sstream>

namespace sl
{

class CameraActivateEvent : public Event
{
public:
	CameraActivateEvent(CameraControllerMode mode) : m_mode(mode) {}

	static EventType GetStaticEventType()
	{
		return EventType::CameraActivate;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "CameraActivate, mode: " << nameof::nameof_enum(m_mode);
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

class CameraDeactivateEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::CameraDeactivate;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "CameraDeactivate";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_CAMERA;
	}
};

} // namespace sl
