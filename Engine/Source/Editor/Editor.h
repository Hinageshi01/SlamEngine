#pragma once

#include "Core/Core.h"
#include "Core/Defines.h"
#include "Core/Timer.h"

namespace sl
{

class Event;
class WindowCloseEvent;
class WindowResizeEvent;
class CameraActivateEvent;
class MouseButtonReleaseEvent;
class Window;
class LayerStack;

}

class CameraControllerLayer;
class ImGuiLayer;
class SandboxLayer;

struct EditorInitor
{
	std::string title;
	uint32_t m_width;
	uint32_t m_height;
	sl::GraphicsBackend m_backend;
};

class Editor final
{
public:
	Editor() = delete;
	Editor(const Editor &) = delete;
	Editor &operator=(const Editor &) = delete;
	Editor(Editor &&) = delete;
	Editor &operator=(Editor &&) = delete;

	Editor(EditorInitor initor);
	~Editor();

	void Run();

	sl::Window *GetWindow() const { return m_pWindow; }

private:
	void BegineFrame();
	void Update();
	void Render();
	void EndFrame();

	void OnEvent(sl::Event &event);
	bool OnWindowClose(sl::WindowCloseEvent &event);
	bool OnWindowResize(sl::WindowResizeEvent &event);
	bool OnCameraActivate(sl::CameraActivateEvent &event);
	bool OnMouseButtonRelease(sl::MouseButtonReleaseEvent &event);

	bool m_isRunning = true;
	bool m_isMinimized = false;

	sl::Timer m_timer;
	sl::Window *m_pWindow = nullptr;
	sl::LayerStack *m_pLayerStack = nullptr;
	
	CameraControllerLayer *m_pCameraControllerLayer = nullptr;
	ImGuiLayer *m_pImGuiLayer = nullptr;
	SandboxLayer *m_pSandboxLayer = nullptr;
};
