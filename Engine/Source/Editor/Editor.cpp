#include "Editor.h"

#include "Core/Log.h"
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
#include "Layer/RendererLayer.h"
#include "Layer/SandboxLayer.h"
#include "Layer/WindowLayer.h"

Editor::Editor(EditorInitor initor)
{
	sl::Log::Init();

	sl::RenderCore::SetBackend(initor.m_backend);
	sl::Window *pWindow = new sl::Window(std::move(initor.title), initor.m_width, initor.m_height);
	pWindow->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));

	sl::Input::Init(pWindow->GetNativeWindow());
	sl::ImGuiContext::Init(pWindow->GetNativeWindow());

	sl::RenderCore::Init();
	sl::RenderCore::SetDefaultState();

	sl::RenderCore::SetMainFramebuffer(sl::FrameBuffer::Create({
		// Size is meaningless here.
		sl::Texture2D::Create(1, 1, false, sl::TextureFormat::RGBA8, SL_SAMPLER_CLAMP | SL_SAMPLER_BILINEAR),
		sl::Texture2D::Create(1, 1, false, sl::TextureFormat::D32, SL_SAMPLER_CLAMP | SL_SAMPLER_BILINEAR),
	}));
	sl::RenderCore::SetEntityIDFramebuffer(sl::FrameBuffer::Create({
		// Size is meaningless here.
		sl::Texture2D::Create(1, 1, false, sl::TextureFormat::R32I, SL_SAMPLER_CLAMP | SL_SAMPLER_NEAREST),
		sl::Texture2D::Create(1, 1, false, sl::TextureFormat::D32, SL_SAMPLER_CLAMP | SL_SAMPLER_BILINEAR),
	}));

	auto mainCameraEntity = sl::ECSWorld::CreateEntity("Editor Camera");
	mainCameraEntity.AddComponent<sl::CameraComponent>();
	mainCameraEntity.AddComponent<sl::CornerstoneComponent>("Currently we only support that only one camera in the scene.");
	sl::ECSWorld::SetEditorCameraEntity(mainCameraEntity);

	m_pWindowLayer = new WindowLayer;
	m_pSandboxLayer = new SandboxLayer;
	m_pRendererLayer = new RendererLayer;
	m_pCameraControllerLayer = new CameraControllerLayer;
	m_pImGuiLayer = new ImGuiLayer;

	m_pWindowLayer->SetWindow(pWindow);
	m_pWindowLayer->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));
	m_pImGuiLayer->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));

	m_pLayerStack = new sl::LayerStack;
	m_pLayerStack->PushLayer(m_pWindowLayer);
	m_pLayerStack->PushLayer(m_pSandboxLayer);
	m_pLayerStack->PushLayer(m_pRendererLayer);
	m_pLayerStack->PushLayer(m_pCameraControllerLayer);
	m_pLayerStack->PushLayer(m_pImGuiLayer);

	m_timer.Tick();
}

Editor::~Editor()
{
	sl::ImGuiContext::Shutdown();

	delete m_pLayerStack;
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
}

void Editor::OnEvent(sl::Event &event)
{
	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));
	dispatcher.Dispatch<sl::WindowResizeEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowResize));

	// Iterate layers from top to bottom / from end to begin.
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
	SL_EDITOR_TRACE("Window close");
	m_isRunning = false;

	return true;
}

bool Editor::OnWindowResize(sl::WindowResizeEvent &event)
{
	if (0 == event.GetWidth() || 0 == event.GetHeight())
	{
		SL_EDITOR_TRACE("Window minimise");
		m_isMinimized = true;
	}
	else
	{
		m_isMinimized = false;
	}

	return true;
}
