#pragma once

#include "Event.h"
#include "Window/KeyCode.h"

#include <sstream>

namespace sl
{

class KeyPressEvent : public Event
{
public:
	KeyPressEvent(int keycode, bool isRepeat = false)
		: m_key(keycode), m_isRepeat(isRepeat) {}

	static EventType GetStaticEventType()
	{
		return EventType::KeyPress;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPress: " << m_key << (m_isRepeat ? ", repeat" : "");
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_KEYBOARD;
	}

	int GetKey() const { return m_key; }
	bool IsRepeat() const { return m_isRepeat; }

private:
	int m_key;
	bool m_isRepeat;
};

class KeyReleaseEvent : public Event
{
public:
	KeyReleaseEvent(int keycode) : m_key(keycode) {}

	static EventType GetStaticEventType()
	{
		return EventType::KeyRelease;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyRelease: " << m_key;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_KEYBOARD;
	}

	int GetKey() const { return m_key; }

private:
	int m_key;
};

class KeyTypeEvent : public Event
{
public:
	KeyTypeEvent(int keycode) : m_key(keycode) {}

	static EventType GetStaticEventType()
	{
		return EventType::KeyType;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyType: " << m_key;
		return ss.str();
	}
	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_KEYBOARD;
	}

	int GetKey() const { return m_key; }

private:
	int m_key;
};

} // namespace sl
