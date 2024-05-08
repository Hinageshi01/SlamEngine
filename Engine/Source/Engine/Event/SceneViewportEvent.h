#pragma once

#include "Event.h"

#include <sstream>

namespace sl
{

class SceneViewportResizeEvent : public Event
{
public:
	SceneViewportResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

	static EventType GetStaticEventType()
	{
		return EventType::SceneViewportResize;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "SceneViewportResize: " << m_width << ", " << m_height;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_SCENE_VIEWPORT;
	}

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }

private:
	uint32_t m_width, m_height;
};

class SceneViewportGetFocusEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::SceneViewportGetFocus;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "SceneViewportFocus";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_SCENE_VIEWPORT;
	}
};

class SceneViewportLostFocusEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::SceneViewportLostFocus;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "SceneViewportLostFocus";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_SCENE_VIEWPORT;
	}
};

class SceneViewportHoverEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::SceneViewportHover;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "SceneViewportHover";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_SCENE_VIEWPORT;
	}
};

} // namespace sl
