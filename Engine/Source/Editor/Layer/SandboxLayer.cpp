#include "SandboxLayer.h"

#include "Core/Path.hpp"
#include "RenderCore/RenderCore.h"
#include "Resource/TextureResource.h"
#include "Scene/ECSWorld.h"

#include <glm/gtc/matrix_transform.hpp>

SandboxLayer::SandboxLayer()
{
	auto &transform = sl::ECSWorld::GetEditorCameraEntity().GetComponent<sl::TransformComponent>();
	transform.m_position = glm::vec3{ 0.0f, 0.0f, 5.0f };
	transform.m_rotation = glm::vec3{ 0.0f, -90.0f, 0.0f };
	
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	};
	uint32_t indices[] = { 0, 1, 3, 1, 2, 3 };

	sl::VertexBuffer *pVertexBuffer = sl::VertexBuffer::Create(vertices, sizeof(vertices));
	pVertexBuffer->SetLayout(sl::VertexLayout
	{
		{ "Position", sl::AttribType::Float, 3 },
		{ "UV", sl::AttribType::Float, 2 },
	});
	sl::IndexBuffer *pIndexBuffer = sl::IndexBuffer::Create(indices, sizeof(indices));

	sl::VertexArray *pVertexArray = sl::VertexArray::Create();
	pVertexArray->SetVertexBuffer(pVertexBuffer);
	pVertexArray->SetIndexBuffer(pIndexBuffer);

	sl::Shader *pShader = sl::Shader::Creat("Test Shader",
		sl::Path::FromeAsset("Shader/vs_Test.glsl"),
		sl::Path::FromeAsset("Shader/fs_Test.glsl"));

	sl::TextureResource *pTextureResource = new sl::TextureResource{
		sl::Path::FromeAsset("Texture/jc.png"), SL_SAMPLER_REPEAT | SL_SAMPLER_TRILINEAR };

	auto entity = sl::ECSWorld::CreateEntity("Test Mesh");
	auto &rendering = entity.AddComponent<sl::RenderingComponent>();
	rendering.m_pVertexArray = pVertexArray;
	rendering.m_pTextureResource = pTextureResource;
	rendering.m_pShader = pShader;
}

SandboxLayer::~SandboxLayer()
{

}

void SandboxLayer::OnAttach()
{

}

void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnEvent(sl::Event &event)
{

}

void SandboxLayer::BeginFrame()
{

}

void SandboxLayer::OnUpdate(float deltaTime)
{

}

void SandboxLayer::OnRender()
{

}

void SandboxLayer::EndFrame()
{

}
