#pragma once

#include "Event.h"

#include <sstream>

namespace sl
{

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

	static EventType GetStaticEventType()
	{
		return EventType::WindowResize;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_width << ", " << m_height;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return EventCategoryWindow;
	}

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }

private:
	uint32_t m_width, m_height;
};

class WindowCloseEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::WindowClose;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "WindowCloseEvent";
	}

	virtual uint8_t GetCategories() const override
	{
		return EventCategoryWindow;
	}
};

class WindowFocusEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::WindowFocus;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "WindowFocusEvent";
	}

	virtual uint8_t GetCategories() const override
	{
		return EventCategoryWindow;
	}
};

class WindowLostFocusEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::WindowLostFocus;
	}

	WindowLostFocusEvent() = default;

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "WindowLostFocusEvent";
	}

	virtual uint8_t GetCategories() const override
	{
		return EventCategoryWindow;
	}
};

} // namespace sl
