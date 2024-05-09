#include "Editor.h"

#include "Core/Log.h"
#include "Event/CameraEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "Layer/LayerStack.h"
#include "RenderCore/RenderCore.h"
#include "Scene/ECSWorld.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include "Layer/CameraControllerLayer.h"
#include "Layer/ImGuiLayer.h"
#include "Layer/SandboxLayer.h"

Editor::Editor(EditorInitor initor)
{
	sl::Log::Init();

	sl::RenderCore::Init(initor.m_backend);
	m_pWindow = new sl::Window(std::move(initor.title), initor.m_width, initor.m_height);
	m_pWindow->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));
	sl::ImGuiContext::Init(m_pWindow->GetNativeWindow());
	sl::Input::Init(m_pWindow->GetNativeWindow());

	sl::RenderCore::SetMainFrameBuffer(sl::FrameBuffer::Create(1280, 720));
	sl::RenderCore::SetDefaultState();

	auto mainCameraEntity = sl::ECSWorld::CreateEntity("Camera");
	mainCameraEntity.AddComponent<sl::CameraComponent>();
	mainCameraEntity.AddComponent<sl::CornerstoneComponent>("Currently we only support that only one camera in the scene.");
	sl::ECSWorld::SetMainCameraEntity(mainCameraEntity);

	for (int i = 0; i < 10; ++i)
	{
		sl::ECSWorld::CreateEntity(std::format("Test entity {}", i).c_str());
	}

	m_pCameraControllerLayer = new CameraControllerLayer;
	m_pSandboxLayer = new SandboxLayer;
	m_pImGuiLayer = new ImGuiLayer;
	m_pImGuiLayer->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));

	m_pLayerStack = new sl::LayerStack;
	m_pLayerStack->PushLayer(m_pSandboxLayer);
	m_pLayerStack->PushLayer(m_pCameraControllerLayer);
	m_pLayerStack->PushLayer(m_pImGuiLayer);

	m_timer.Tick();
}

Editor::~Editor()
{
	sl::ImGuiContext::Shutdown();

	delete m_pLayerStack;
	delete m_pWindow;
}

void Editor::Run()
{
	while (m_isRunning)
	{
		BegineFrame();

		if (!m_isMinimized)
		{
			Update();
			Render();
		}

		EndFrame();
	}
}

void Editor::BegineFrame()
{
	m_timer.Tick();

	for (sl::Layer *pLayer : *m_pLayerStack)
	{
		pLayer->BeginFrame();
	}
}

void Editor::Update()
{
	for (sl::Layer *pLayer : *m_pLayerStack)
	{
		pLayer->OnUpdate(m_timer.GetDeltatIme());
	}
}

void Editor::Render()
{
	for (sl::Layer *pLayer : *m_pLayerStack)
	{
		pLayer->OnRender();
	}
}

void Editor::EndFrame()
{
	for (sl::Layer *pLayer : *m_pLayerStack)
	{
		pLayer->EndFrame();
	}

	m_pWindow->EndFrame();
}

void Editor::OnEvent(sl::Event &event)
{
	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));
	dispatcher.Dispatch<sl::WindowResizeEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowResize));
	dispatcher.Dispatch<sl::CameraActivateEvent>(BIND_EVENT_CALLBACK(Editor::OnCameraActivate));
	dispatcher.Dispatch<sl::MouseButtonReleaseEvent>(BIND_EVENT_CALLBACK(Editor::OnMouseButtonRelease));

	// Iterate layers from end to begin / top to bottom.
	for (auto it = m_pLayerStack->rend(); it != m_pLayerStack->rbegin(); ++it)
	{
		if (event.GetIsHandled())
		{
			break;
		}
		(*it)->OnEvent(event);
	}
}

bool Editor::OnWindowClose(sl::WindowCloseEvent &event)
{
	SL_EDITOR_TRACE("Window closed");
	m_isRunning = false;

	return true;
}

bool Editor::OnWindowResize(sl::WindowResizeEvent &event)
{
	if (0 == event.GetWidth() || 0 == event.GetHeight())
	{
		SL_EDITOR_TRACE("Window minimized");
		m_isMinimized = true;
	}
	else
	{
		m_isMinimized = false;
	}

	return true;
}

// TODO: Move these to Window layer

bool Editor::OnCameraActivate(sl::CameraActivateEvent &event)
{
	m_pWindow->CaptureCursor();

	return false;
}

bool Editor::OnMouseButtonRelease(sl::MouseButtonReleaseEvent &event)
{
	m_pWindow->ReleaseCursor();

	return false;
}
