#include "Editor.h"

#include "Event/KeyEvent.h"
#include "Log/Log.h"

void Editor::Init()
{
	sl::Log::Init();

	auto lambda = [](sl::Event &event)
	{
		SL_EDITOR_DEBUG(event);
		return true;
	};

	sl::KeyPressedEvent event{ 42 };
	sl::EventDispatcher dis{ event };
	dis.Dispatch<sl::KeyPressedEvent>(lambda);
	dis.Dispatch<sl::KeyReleasedEvent>(lambda);
}

void Editor::Update()
{

}

void Editor::Render()
{

}

void Editor::Shutdown()
{

}
