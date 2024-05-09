#include "ImGuiLayer.h"

#include "Core/Log.h"
#include "Event/CameraEvent.h"
#include "Event/SceneViewportEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "RenderCore/RenderCore.h"
#include "Resource/Font.h"
#include "Scene/ECSWorld.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <implot/implot.h>
#include <nameof/nameof.hpp>

namespace
{

constexpr ImGuiTreeNodeFlags DefaultTreeFlags =
	ImGuiTreeNodeFlags_AllowOverlap |
	ImGuiTreeNodeFlags_DefaultOpen |
	ImGuiTreeNodeFlags_OpenOnDoubleClick |
	ImGuiTreeNodeFlags_OpenOnArrow |
	ImGuiTreeNodeFlags_CollapsingHeader;

constexpr ImGuiTreeNodeFlags DefaultSubTreeFlags =
	ImGuiTreeNodeFlags_NoTreePushOnOpen |
	ImGuiTreeNodeFlags_NoAutoOpenOnLog |
	ImGuiTreeNodeFlags_DefaultOpen |
	ImGuiTreeNodeFlags_SpanFullWidth;

SL_FORCEINLINE static glm::vec3 ModVec3(const glm::vec3 &v, float m)
{
	// Why the second argument of glm::modf must accept a non const left value reference?
	return glm::vec3{ std::fmod(v.x, m), std::fmod(v.y, m) , std::fmod(v.z, m) };
}

// From ImPlot
struct ScrollingBuffer
{
	int MaxSize;
	int Offset;
	ImVector<ImVec2> Data;
	ScrollingBuffer(int max_size = 1024)
	{
		MaxSize = max_size;
		Offset = 0;
		Data.reserve(MaxSize);
	}
	void AddPoint(float x, float y)
	{
		if (Data.size() < MaxSize)
		{
			Data.push_back(ImVec2(x, y));
		}
		else
		{
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}
	}
	void Erase()
	{
		if (Data.size() > 0)
		{
			Data.shrink(0);
			Offset = 0;
		}
	}
};

bool AlignButton(const char *label, float align = 0.5f, float offset = 0.0f)
{
	float size = ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float startOffset = (avail - size) * align;
	if (startOffset > 0.0f)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + startOffset + offset);
	}

	return ImGui::Button(label);
}

void RightClickFocus()
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		ImGui::SetWindowFocus();
	}
}

}

ImGuiLayer::ImGuiLayer()
{
	SetName("ImGui Layer");

	m_dockSpaceFlag |= ImGuiDockNodeFlags_NoUndocking;
}

ImGuiLayer::~ImGuiLayer()
{

}

void ImGuiLayer::OnAttach()
{
	m_selectedEntity = sl::ECSWorld::GetMainCameraEntity();
}

void ImGuiLayer::OnDetach()
{

}

void ImGuiLayer::OnEvent(sl::Event &event)
{

}

void ImGuiLayer::BeginFrame()
{

}

void ImGuiLayer::OnUpdate(float deltaTime)
{
	sl::ImGuiContext::NewFrame();

	ShowDebugPanels();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), (ImGuiDockNodeFlags)m_dockSpaceFlag);

	ShowMenuBar();
	ShowEntityList();
	ShowLog();
	ShowInfo(deltaTime);
	ShowDetails();
	ShowSceneViewport();
}

void ImGuiLayer::OnRender()
{
	sl::ImGuiContext::Submit();
}

void ImGuiLayer::EndFrame()
{

}

void ImGuiLayer::ShowDebugPanels()
{
	if (m_debugImGuiDemo)
	{
		ImGui::ShowDemoWindow(&m_debugImGuiDemo);
	}
	if (m_styleEditor)
	{
		ImGui::ShowStyleEditor();
	}
	if (m_debugIDStack)
	{
		ImGui::ShowIDStackToolWindow(&m_debugIDStack);
	}
	if (m_debugItemPicker)
	{
		ImGui::DebugStartItemPicker();
	}
}

void ImGuiLayer::ShowMenuBar()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{

		}
		if (ImGui::MenuItem("Open"))
		{

		}
		if (ImGui::MenuItem("Save"))
		{

		}
		ImGui::Separator();
		if (ImGui::MenuItem("Exit"))
		{
			sl::WindowCloseEvent event;
			m_eventCallback(event);
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Setting"))
	{
		if (ImGui::MenuItem("No Undocking", "", m_dockSpaceFlag & ImGuiDockNodeFlags_NoUndocking))
		{
			m_dockSpaceFlag ^= ImGuiDockNodeFlags_NoUndocking;
		}
		if (ImGui::MenuItem("No Resize", "", m_dockSpaceFlag & ImGuiDockNodeFlags_NoResize))
		{
			m_dockSpaceFlag ^= ImGuiDockNodeFlags_NoResize;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Debug"))
	{
		ImGui::MenuItem("ImGui Demo", "", &m_debugImGuiDemo);
		ImGui::MenuItem("Style Editor", "", &m_styleEditor);
		ImGui::MenuItem("ID Stack", "", &m_debugIDStack);
		ImGui::MenuItem("Item Picker", "", &m_debugItemPicker);
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void ImGuiLayer::ShowEntityList()
{
	ImGui::Begin("Entity List");

	RightClickFocus();

	if (ImGui::Button("+"))
	{
		sl::ECSWorld::CreateEntity("Empty Entity");
	}
	ImGui::Separator();

	auto view = sl::ECSWorld::GetRegistry().view<sl::TagComponent>();
	for (auto entity : view)
	{
		ImGui::PushID((void *)(uint64_t)(uint32_t)entity);

		ImGuiTreeNodeFlags treeNodeFlag =
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_Leaf; // TODO: hierarchy
		if (m_selectedEntity == entity)
		{
			treeNodeFlag |= ImGuiTreeNodeFlags_Selected;
		}

		auto &tag = view.get<sl::TagComponent>(entity);
		bool nodeOpen = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, treeNodeFlag, tag.m_name.c_str());

		// Left click to select entity.
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			SL_ENGINE_TRACE("Select entity: \"{}\"", tag.m_name);
			m_selectedEntity = entity;
		}

		// Right click to open an entity popup.
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("EntityPopupContext");
		}
		if (ImGui::BeginPopup("EntityPopupContext"))
		{
			if (ImGui::MenuItem("Destory Entity"))
			{
				sl::ECSWorld::DestroyEntity(entity);
			}
			ImGui::EndPopup();
		}

		if (nodeOpen)
		{
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	// Left click at enpty space to clear selected entity.
	if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		m_selectedEntity.Reset();
	}

	ImGui::End();
}

void ImGuiLayer::ShowLog()
{
	ImGui::Begin("Log");

	RightClickFocus();

	ImGui::Text("TODO");

	ImGui::End();
}

void ImGuiLayer::ShowInfo(float deltaTime)
{
	ImGui::Begin("Info");

	RightClickFocus();

	// Infos
	{
		ImGui::Text("Backend: %s", nameof::nameof_enum(sl::RenderCore::GetBackend()).data());
		ImGui::Separator();
	}

	// FPS plot
	{
		// Stores in millisecond.
		static float s_sumTime = 0.0f;
		static float s_deltaTimeMultiplier = 10.0f;
		static ScrollingBuffer s_coastBuffer;
		static ScrollingBuffer s_fpsBuffer;

		constexpr float History = 1000.0f;
		constexpr float Delay = 1000.0f;

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Delta Time Multiplier:");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::DragFloat("##DeltaTimeMultiplier", &s_deltaTimeMultiplier, 1.0f, 1.0f, 1000.0f);

		s_sumTime += deltaTime;
		if (s_sumTime > Delay)
		{
			// Waiting for programme to stabilise.
			s_coastBuffer.AddPoint(s_sumTime, s_deltaTimeMultiplier * deltaTime);
			s_fpsBuffer.AddPoint(s_sumTime, 1000.0f / deltaTime);
		}

		constexpr ImPlotFlags PlotFlag = ImPlotFlags_CanvasOnly;
		constexpr ImPlotAxisFlags AxisFlagX = ImPlotAxisFlags_NoTickLabels;
		constexpr ImPlotAxisFlags AxisFlagY = ImPlotAxisFlags_AutoFit;
		if (ImPlot::BeginPlot("Monitor", ImVec2(-1.0f, 150.0f), PlotFlag))
		{
			ImPlot::SetupAxes(nullptr, nullptr, AxisFlagX, AxisFlagY);
			ImPlot::SetupAxisLimits(ImAxis_X1, s_sumTime - History, s_sumTime, ImGuiCond_Always);
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
			if (s_sumTime > Delay)
			{
				ImPlot::PlotShaded("Coast in ms",
					&s_coastBuffer.Data[0].x,
					&s_coastBuffer.Data[0].y,
					s_coastBuffer.Data.size(),
					0, 0,
					s_coastBuffer.Offset,
					2 * sizeof(float));

				ImPlot::PlotLine("FPS",
					&s_fpsBuffer.Data[0].x,
					&s_fpsBuffer.Data[0].y,
					s_fpsBuffer.Data.size(), 
					0,
					s_fpsBuffer.Offset,
					2 * sizeof(float));
			}
			ImPlot::EndPlot();
		}
	}

	ImGui::End();
}

template<class T, class Fun>
void ImGuiLayer::DrawComponent(const char *label, Fun uiFunction)
{
	if (auto *pComponent = m_selectedEntity.TryGetComponent<T>(); pComponent)
	{
		ImGui::PushID(nameof::nameof_type<T>().data());

		// Draw tree node
		ImGui::PushFont(sl::Font::GetBold());
		bool componentTreeOpen = ImGui::TreeNodeEx(label, DefaultTreeFlags, label);
		ImGui::PopFont();

		// Draw component menu button
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::SameLine();
		if (AlignButton(" : ", 1.0f, ImGui::GetStyle().WindowPadding.x / 2.0f))
		{
			ImGui::OpenPopup("ComponentPopup");
		}
		ImGui::PopStyleColor();

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentPopup"))
		{
			if (ImGui::MenuItem("Reset Component"))
			{
				pComponent->Reset();
				if constexpr (std::is_same_v<T, sl::TransformComponent>)
				{
					if (auto pCamera = m_selectedEntity.TryGetComponent<sl::CameraComponent>(); pCamera)
					{
						pCamera->m_isDirty = true;
					}
				}
			}
			if constexpr (!std::is_same_v<T, sl::TagComponent> && !std::is_same_v<T, sl::TransformComponent>)
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
			}
			ImGui::EndPopup();
		}

		// Draw component specific item
		if (componentTreeOpen)
		{
			uiFunction(pComponent);
		}

		if (removeComponent)
		{
			m_selectedEntity.RemoveComponent<T>();
			m_maxTextSize = 0.0f;
		}

		ImGui::Separator();
		ImGui::PopID();
	}
}

template<class T>
void ImGuiLayer::AddComponent(const char *label)
{
	if (ImGui::MenuItem(label))
	{
		if (!m_selectedEntity.TryGetComponent<T>())
		{
			m_selectedEntity.AddComponent<T>();
		}
		else
		{
			SL_ENGINE_WARN("Entity \"{}\" already has component \"{}\"", m_selectedEntity.GetComponent<sl::TagComponent>().m_name, label);
		}
	}
}

void ImGuiLayer::StartWithText(std::string text)
{
	static sl::Entity s_crtEntity;
	if (s_crtEntity != m_selectedEntity)
	{
		// ImGui::CalcTextSize("Position").x == 56.0f
		// ImGui::CalcTextSize("Rotation").x == 56.0f
		// Just a little trick to avoid Tag Component flickering when it is rendered the first time,
		// as we know every entity must hold both Tag and Transform component.
		m_maxTextSize = 56.0f;
		s_crtEntity = m_selectedEntity;
	}

	float crtTextSize = ImGui::CalcTextSize(text.c_str()).x;
	m_maxTextSize = std::max(m_maxTextSize, crtTextSize);

	float offset = ImGui::GetStyle().IndentSpacing + ImGui::GetFontSize();
	ImGui::SetCursorPosX(offset);
	ImGui::AlignTextToFramePadding();
	ImGui::Text(text.c_str());

	ImGui::SameLine(m_maxTextSize + 50.0f);
	ImGui::SetNextItemWidth(-8.0f);
}

void ImGuiLayer::ShowDetails()
{
	ImGui::Begin("Details");

	RightClickFocus();

	if (!m_selectedEntity)
	{
		ImGui::End();
		return;
	}

	// Draw Tag component
	DrawComponent<sl::TagComponent>("Tag", [this](auto *pComponent)
	{
		std::string &name = pComponent->m_name;

		constexpr size_t BufferSize = 256;
		SL_EDITOR_ASSERT(BufferSize >= name.size());

		char buffer[BufferSize] = { 0 };
		memcpy(buffer, name.c_str(), name.size());

		StartWithText("Name");
		if (ImGui::InputText("##Name", buffer, BufferSize))
		{
			name = std::string{ buffer };
			if (name.empty())
			{
				name = "Default Name";
			}
		}
	});

	// Draw Transform component
	DrawComponent<sl::TransformComponent>("Transform", [this](auto *pComponent)
	{
		bool cameraMayBeDirty = false;

		glm::vec3 &position = pComponent->m_position;
		StartWithText("Position");
		if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.1f))
		{
			cameraMayBeDirty = true;
		}

		glm::vec3 ratationDegree = ModVec3(pComponent->GetRotationDegrees(), 360.0f);
		StartWithText("Rotation");
		if (ImGui::DragFloat3("##Rotation", glm::value_ptr(ratationDegree), 0.1f))
		{
			pComponent->SetRotationDegrees(ratationDegree);
			cameraMayBeDirty = true;
		}

		glm::vec3 &scale = pComponent->m_scale;
		StartWithText("Scale");
		ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f);

		if (cameraMayBeDirty)
		{
			if (auto *pCamera = m_selectedEntity.TryGetComponent<sl::CameraComponent>(); pCamera)
			{
				pCamera->m_isDirty = true;
			}
		}
	});

	// Draw Camera component
	DrawComponent<sl::CameraComponent>("Camera", [this](auto *pComponent)
	{
		constexpr const char *ProjectionTypeNames[] =
		{
			nameof::nameof_enum(sl::ProjectionType::Perspective).data(),
			nameof::nameof_enum(sl::ProjectionType::Orthographic).data(),
		};

		const char *crtProjectionTypeName = ProjectionTypeNames[static_cast<size_t>(pComponent->m_projectionType)];
		StartWithText("Projection");
		if (ImGui::BeginCombo("##Projection", crtProjectionTypeName, ImGuiComboFlags_WidthFitPreview))
		{
			for (size_t i = 0; i < 2; ++i)
			{
				bool isSelected = (ProjectionTypeNames[i] == crtProjectionTypeName);
				if (ImGui::Selectable(ProjectionTypeNames[i], isSelected))
				{
					pComponent->m_projectionType = static_cast<sl::ProjectionType>(i);
					pComponent->m_isDirty = true;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Separator();

		if (ImGui::TreeNodeEx("##Perspective", DefaultSubTreeFlags, "Perspective"))
		{
			ImGui::Indent();

			float fovDegrees = glm::degrees(pComponent->m_fov);
			StartWithText("FOV");
			if (ImGui::DragFloat("##FOV", &fovDegrees, 0.1f, 1.0f, 180.0f))
			{
				pComponent->m_fov = glm::radians(fovDegrees);
				pComponent->m_isDirty = true;
			}

			StartWithText("Near Plane");
			if (ImGui::DragFloat("##NearPlane", &(pComponent->m_nearPlane), 0.001f, 0.001f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}

			StartWithText("Far Plane");
			if (ImGui::DragFloat("##FarPlane", &(pComponent->m_farPlane), 1.0f, 0.001f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}

			ImGui::Unindent();
		}
		ImGui::Separator();

		if (ImGui::TreeNodeEx("##Orthographic", DefaultSubTreeFlags, "Orthographic"))
		{
			ImGui::Indent();

			StartWithText("Size");
			if (ImGui::DragFloat("##Size", &(pComponent->m_orthoSize), 0.1f, 0.1f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}

			StartWithText("Near Clip");
			if (ImGui::DragFloat("##NearClip", &(pComponent->m_orthoNearClip), 0.1f, -100000.0f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}

			StartWithText("Far Clip");
			if (ImGui::DragFloat("##FarClip", &(pComponent->m_orthoFarClip), 0.1f, -100000.0f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}

			ImGui::Unindent();
		}
		ImGui::Separator();

		if (ImGui::TreeNodeEx("##Controller", DefaultSubTreeFlags, "Controller"))
		{
			ImGui::Indent();

			float rotateSpeedDegrees = glm::degrees(pComponent->m_rotateSpeed);
			StartWithText("Rotate Speed");
			if (ImGui::DragFloat("##RotateSpeed", &rotateSpeedDegrees, 0.001f, 0.001f, 1.0f))
			{
				pComponent->m_rotateSpeed = glm::radians(rotateSpeedDegrees);
			}

			StartWithText("Move Speed");
			ImGui::DragFloat("##MoveSpeed", &(pComponent->m_maxMoveSpeed), 0.001f, 0.001f, 1.0f);

			StartWithText("Acceleration");
			ImGui::DragFloat("##Acceleration", &(pComponent->m_maxSpeedToAcceleration), 0.001f, 0.001f, 1.0f);

			StartWithText("Shift Multiplier");
			ImGui::DragFloat("##ShiftMultiplier", &(pComponent->m_moveSpeedKeyShiftMultiplier), 0.1f, 0.1f, 10.0f);

			StartWithText("Scroll Multiplier");
			ImGui::DragFloat("##ScrollMultiplier", &(pComponent->m_moveSpeedMouseScrollMultiplier), 0.1f, 0.1f, 10.0f);

			ImGui::Unindent();
		}
	});

	// Draw Cornerstone component
	DrawComponent<sl::CornerstoneComponent>("Cornerstone", [this](auto *pComponent)
	{
		StartWithText("Info");
		ImGui::TextWrapped(pComponent->m_info.c_str());
	});

	// Add component button
	if (AlignButton("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}
	if (ImGui::BeginPopup("AddComponent"))
	{
		AddComponent<sl::CameraComponent>("Camera");
		AddComponent<sl::CornerstoneComponent>("Cornerstone");
		ImGui::EndPopup();
	}

	ImGui::End();
}

void ImGuiLayer::ShowSceneViewport()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
	ImGui::Begin("Scene");

	// Scene viewport event stuff
	{
		// Camera event
		if (ImGui::IsWindowHovered())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				ImGui::SetWindowFocus();

				sl::CameraActivateEvent event{ sl::CameraControllerMode::FPS };
				m_eventCallback(event);
			}
			else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsKeyDown(ImGuiKey_LeftAlt))
			{
				sl::CameraActivateEvent event{ sl::CameraControllerMode::Editor };
				m_eventCallback(event);
			}
		}

		// Resize event
		auto crtSize = ImGui::GetContentRegionAvail();
		uint32_t crtSizeX = (uint32_t)crtSize.x;
		uint32_t crtSizeY = (uint32_t)crtSize.y;
		if (crtSizeX != m_viewportSizeX || crtSizeY != m_viewportSizeY)
		{
			sl::SceneViewportResizeEvent event{ crtSizeX , crtSizeY };
			m_eventCallback(event);

			m_viewportSizeX = crtSizeX;
			m_viewportSizeY = crtSizeY;

			sl::RenderCore::GetMainFrameBuffer()->Resize(m_viewportSizeX, m_viewportSizeY);
		}
	}

	// The invisible button is designed to prevent the mouse from hovering over the ui item
	// even when the camera is moving, using an internal imgui mechanism.
	// I haven't come up with a better solution, but it just works.
	ImVec2 pos = ImGui::GetCursorPos();
	ImGui::InvisibleButton("SceneViewport", ImVec2{ (float)m_viewportSizeX, (float)m_viewportSizeY },
			ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	ImGui::SetCursorPos(pos);

	// Draw main frame buffer color attachment
	uint32_t handle = sl::RenderCore::GetMainFrameBuffer()->GetColorAttachmentHandle();
	ImGui::Image((void *)(uint64_t)handle, ImVec2{ (float)m_viewportSizeX, (float)m_viewportSizeY }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

	ImGui::End();
	ImGui::PopStyleVar();
}

// For ImGuiLayer::m_dockSpaceFlag
static_assert(std::is_same_v<ImGuiDockNodeFlags, int>);
static_assert(ImGuiDockNodeFlags_None == 0);
