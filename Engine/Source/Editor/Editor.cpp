#include "Editor.h"

#include "Core/Core.h"
#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Layer/LayerStack.h"
#include "RenderCore/RenderCore.h"
#include "Window/Input.h"
#include "Window/Window.h"

// TEMPLATE
#include "Core/Path.hpp"
#include <glm/gtc/matrix_transform.hpp>

Editor::Editor(EditorInitor initor)
{
	Init(std::move(initor));
}

Editor::~Editor()
{
	Shutdown();
}

void Editor::Init(EditorInitor initor)
{
	sl::Log::Init();

	sl::RenderCore::GetInstance().SetBackend(initor.m_backend);

	m_pWindow = new sl::Window{ std::move(initor.title), initor.m_width, initor.m_height, true };
	m_pWindow->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));
	sl::Input::GetInstance().SetWindow(m_pWindow);
	sl::RenderCore::GetInstance().SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// PENDING: Use pointer of every layer directly instead of layer stack.
	m_layerStack.PushLayer(new sl::ImGuiLayer{ m_pWindow });

	// TEMPORARY
	{
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		uint32_t indices[] = { 0, 1, 3, 1, 2, 3 };

		std::string vsSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec2 a_uv;
			out vec2 v_uv;

			uniform mat4 u_ModelViewProjection;

			void main()
			{
				v_uv = a_uv;
				gl_Position = u_ModelViewProjection * vec4(a_position, 1.0);
			}
		)";

		std::string fsSrc = R"(
			#version 330 core
			in vec2 v_uv;
			out vec4 o_color;

			uniform sampler2D u_texture;

			void main()
			{
				o_color = vec4(texture(u_texture, v_uv).xyz, 1.0);
			}
		)";

		sl::IndexBuffer *pIndexBuffer = sl::IndexBuffer::Create(indices, sizeof(indices));
		sl::VertexBuffer *pVertexBuffer = sl::VertexBuffer::Create(vertices, sizeof(vertices));
		pVertexBuffer->SetLayout(sl::VertexLayout
		{
			{"Position", sl::AttribType::Float, 3},
			{"UV", sl::AttribType::Float, 2},
		});

		m_pVertexArray = sl::VertexArray::Create();
		m_pVertexArray->SetVertexBuffer(pVertexBuffer);
		m_pVertexArray->SetIndexBuffer(pIndexBuffer);

		m_pShader = sl::Shader::Creat("Test Shader", std::move(vsSrc), std::move(fsSrc));

		m_pTexture = sl::Texture2D::Create(sl::Path::FromeAsset("Texture/jc.png"));

		m_camera.SetWindow(m_pWindow);
		m_camera.GetData().SetPosition(glm::vec3{ 0.0f, 0.0f, 5.0f });
		m_camera.GetData().SetRotationDegrees(glm::vec3{ 0.0f, -90.0f, 0.0f });
	}
}

void Editor::Shutdown()
{
	delete m_pVertexArray;
	delete m_pShader;

	m_layerStack.Shutdown();
	delete m_pWindow;
}

void Editor::Update()
{
	while (m_isRunning)
	{
		BegineFrame();

		for (sl::Layer *pLayer : m_layerStack)
		{
			pLayer->OnUpdate();
		}
		m_camera.Update(m_timmer.GetDeltatIme());

		Render();
		EndFrame();
	}
}

void Editor::BegineFrame()
{
	m_timmer.Update();

	sl::RenderCore::GetInstance().Clear(SL_CLEAR_COLOR);

	for (sl::Layer *pLayer : m_layerStack)
	{
		pLayer->BeginFrame();
	}
}

void Editor::Render()
{
	// TEMPORARY
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			glm::vec3 trans{ i, j, 0.0f };
			glm::mat4 modelMat = glm::translate(glm::mat4{ 1.0f }, trans * 1.1f);
			glm::mat4 mvp = m_camera.GetData().GetViewProjection() * modelMat;

			m_pShader->Bind();
			m_pTexture->Bind(0);
			m_pShader->UploadUniform("u_texture", (int)0);
			m_pShader->UploadUniform("u_ModelViewProjection", std::move(mvp));
			m_pShader->Unbind();
			sl::RenderCore::GetInstance().Submit(m_pVertexArray, m_pShader);
		}
	}

	for (sl::Layer *pLayer : m_layerStack)
	{
		pLayer->OnRender();
	}
}

void Editor::EndFrame()
{
	m_pWindow->EndFrame();
	for (sl::Layer *pLayer : m_layerStack)
	{
		pLayer->EndFrame();
	}
}

void Editor::OnEvent(sl::Event &event)
{
	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));

	// Iterate layers from end to begin.
	for (auto it = std::make_reverse_iterator(m_layerStack.end());
		it != std::make_reverse_iterator(m_layerStack.begin());
		++it)
	{
		(*it)->OnEvent(event);
		if (event.GetIsHandled())
		{
			break;
		}
	}
}

bool Editor::OnWindowClose(sl::Event &event)
{
	m_isRunning = false;
	return true;
}
