#pragma once

#include <cstdint>
#include <format>
#include <functional>

namespace sl
{

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowGetFocus, WindowLostFocus, WindowDrop,
	KeyPress, KeyRelease, KeyType,
	MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll,
	SceneViewportResize, SceneViewportGetFocus, SceneViewportLostFocus, SceneViewportHover,
	CameraActivate, CameraDeactivate,
};

#define SL_EVENT_CATEGORY_NONE           UINT8_C(0x00)
#define SL_EVENT_CATEGORY_WINDOW         UINT8_C(0x01)
#define SL_EVENT_CATEGORY_INPUT          UINT8_C(0x02)
#define SL_EVENT_CATEGORY_KEYBOARD       UINT8_C(0x04)
#define SL_EVENT_CATEGORY_MOUSE          UINT8_C(0x08)
#define SL_EVENT_CATEGORY_SCENE_VIEWPORT UINT8_C(0x10)
#define SL_EVENT_CATEGORY_CAMERA         UINT8_C(0x12)

#define BIND_EVENT_CALLBACK(fun) std::bind(&fun, this, std::placeholders::_1)

class Event
{
public:
	virtual ~Event() = default;

	virtual EventType GetEventType() const = 0;
	virtual std::string ToString() const = 0;

	virtual uint8_t GetCategories() const = 0;
	bool IsInCategory(uint8_t category) const
	{
		return category & GetCategories();
	}

	void SetHandled(bool handled) { m_handled = handled; }
	bool &GetIsHandled() { return m_handled; }
	bool GetIsHandled() const { return m_handled; }

private:
	bool m_handled = false;
};

class EventDispatcher
{
public:
	EventDispatcher(Event &event) : m_event(event) {}

	template<typename T>
	bool Dispatch(std::function<bool(T &)> func)
	{
		static_assert(std::is_base_of_v<Event, T>);

		// Call func if type of m_event is T.
		if (T::GetStaticEventType() == m_event.GetEventType())
		{
			m_event.GetIsHandled() |= func(static_cast<T &>(m_event));
			return true;
		}

		return false;
	}

private:
	Event &m_event;
};

using EventCallback = std::function<void(Event &)>;

} // namespace sl

template<>
struct std::formatter<sl::Event> : std::formatter<std::string>
{
	auto format(const sl::Event &event, std::format_context &context) const
	{
		return formatter<string>::format(event.ToString(), context);
	}
};
