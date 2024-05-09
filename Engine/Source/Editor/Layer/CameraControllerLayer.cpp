#include "CameraControllerLayer.h"

#include "Event/CameraEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/SceneViewportEvent.h"
#include "Scene/ECSWorld.h"
#include "Window/Input.h"

#include <algorithm>
#include <array>

namespace
{

static constexpr std::array<uint32_t, 6> CamraMoveKey =
{
	SL_KEY_W, SL_KEY_A, SL_KEY_S, SL_KEY_D,
	SL_KEY_Q, SL_KEY_E,
};

}

void CameraControllerLayer::OnAttach()
{

}

void CameraControllerLayer::OnDetach()
{

}
void CameraControllerLayer::OnEvent(sl::Event &event)
{
	sl::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<sl::MouseScrollEvent>(BIND_EVENT_CALLBACK(CameraControllerLayer::OnMouseScroll));
	dispatcher.Dispatch<sl::SceneViewportResizeEvent>(BIND_EVENT_CALLBACK(CameraControllerLayer::OnSceneViewportResize));
	dispatcher.Dispatch<sl::CameraActivateEvent>(BIND_EVENT_CALLBACK(CameraControllerLayer::OnCameraActivate));
	dispatcher.Dispatch<sl::MouseButtonReleaseEvent>(BIND_EVENT_CALLBACK(CameraControllerLayer::OnMouseButtonRelease));
}

void CameraControllerLayer::BeginFrame()
{

}
void CameraControllerLayer::OnUpdate(float deltaTime)
{
	UpdateMainCamera(deltaTime);
}

void CameraControllerLayer::OnRender()
{

}

void CameraControllerLayer::EndFrame()
{

}

void CameraControllerLayer::UpdateMainCamera(float deltaTime)
{
	if (sl::CameraControllerMode::None == m_controllerMode)
	{
		m_isRotating = false;
		m_isMoving = false;
		return;
	}

	if (sl::CameraControllerMode::FPS == m_controllerMode)
	{
		UpdateFPSCamera(deltaTime);
	}
	else if(sl::CameraControllerMode::Editor == m_controllerMode)
	{
		UpdateEditorCamera(deltaTime);
	}
}

void CameraControllerLayer::UpdateFPSCamera(float deltaTime)
{
	auto [camera, transform] = sl::ECSWorld::GetMainCameraEntity().GetComponent<sl::CameraComponent, sl::TransformComponent>();

	if (!m_isRotating)
	{
		// When the camera starts to rotate.
		camera.m_acceleration = 0.0f;
		camera.m_moveSpeed = 0.0f;
		camera.m_mouseLastPos = sl::Input::GetMousePos();
		m_isRotating = true;
	}

	// Rotation
	{
		glm::vec2 crtPos = sl::Input::GetMousePos();
		glm::vec2 offset{ camera.m_mouseLastPos.y - crtPos.y, crtPos.x - camera.m_mouseLastPos.x };
		camera.m_mouseLastPos = crtPos;

		transform.m_rotation += glm::vec3{ offset * camera.m_rotateSpeed, 0.0f };
		transform.m_rotation.x = std::clamp(transform.m_rotation.x, glm::radians(-89.9f), glm::radians(89.9f));

		camera.m_isDirty = true;
	}

	static_assert(8 >= CamraMoveKey.size());
	uint8_t moveKeyMask = 0x00;
	for (size_t i = 0; i < CamraMoveKey.size(); ++i)
	{
		if (sl::Input::IsKeyPressed(CamraMoveKey[i]))
		{
			moveKeyMask |= UINT8_C(1 << i);
		}
	}

	glm::vec3 finalMoveDir;
	if (moveKeyMask)
	{
		if (!m_isMoving)
		{
			// When the camera starts to move.
			camera.m_acceleration = camera.m_maxMoveSpeed * camera.m_maxSpeedToAcceleration;
			m_isMoving = true;
		}

		finalMoveDir = glm::vec3{ 0.0f, 0.0f, 0.0f };
		const std::array<glm::vec3, 6> moveBehaviors =
		{
			camera.m_frontDir,             // SL_KEY_W
			-camera.m_rightDir,            // SL_KEY_A
			-camera.m_frontDir,            // SL_KEY_S
			camera.m_rightDir,             // SL_KEY_D
			-sl::CameraComponent::WorldUp, // SL_KEY_Q
			sl::CameraComponent::WorldUp,  // SL_KEY_E
		};
		for (size_t i = 0; i < CamraMoveKey.size(); ++i)
		{
			if (moveKeyMask & UINT8_C(1 << i))
			{
				finalMoveDir += moveBehaviors[i];
			}
		}
		finalMoveDir = glm::normalize(finalMoveDir);

		if (glm::any(glm::isnan(finalMoveDir)))
		{
			// When pressing buttons in the opposite direction at the same time.
			finalMoveDir = glm::vec3{ 0.0f, 0.0f, 0.0f };
		}
		camera.m_lastMoveDir = finalMoveDir;
	}
	else
	{
		if (m_isMoving)
		{
			// Stop moving
			camera.m_acceleration = -camera.m_maxMoveSpeed * camera.m_maxSpeedToAcceleration;
			m_isMoving = false;
		}
		finalMoveDir = camera.m_lastMoveDir;
	}

	camera.m_moveSpeed += camera.m_acceleration * deltaTime;
	camera.m_moveSpeed = std::clamp(camera.m_moveSpeed, 0.0f, camera.m_maxMoveSpeed);
	float moveSpeedKeyShiftMultiplier = sl::Input::IsKeyPressed(SL_KEY_LEFT_SHIFT) ? camera.m_moveSpeedKeyShiftMultiplier : 1.0f;
	float finalMoveSpeed = camera.m_moveSpeed * moveSpeedKeyShiftMultiplier * camera.m_moveSpeedMouseScrollMultiplier * deltaTime;

	transform.m_position += finalMoveDir * finalMoveSpeed;
	camera.m_isDirty = true;
}

void CameraControllerLayer::UpdateEditorCamera(float deltaTime)
{
	// TODO
}

bool CameraControllerLayer::OnMouseScroll(sl::MouseScrollEvent &event)
{
	if (sl::CameraControllerMode::None == m_controllerMode)
	{
		return false;
	}

	auto &camera = sl::ECSWorld::GetMainCameraEntity().GetComponent<sl::CameraComponent>();

	camera.m_moveSpeedMouseScrollMultiplier += event.GetOffsetY() * 0.1f;
	camera.m_moveSpeedMouseScrollMultiplier = std::clamp(camera.m_moveSpeedMouseScrollMultiplier, 0.1f, 10.0f);

	return true;
}

bool CameraControllerLayer::OnSceneViewportResize(sl::SceneViewportResizeEvent &event)
{
	auto &camera = sl::ECSWorld::GetMainCameraEntity().GetComponent<sl::CameraComponent>();

	float aspect = (float)event.GetWidth() / (float)event.GetHeight();
	float fovMultiplier = aspect * 9.0f / 16.0f;

	camera.m_aspect = aspect;
	camera.m_fovMultiplier = fovMultiplier;
	camera.m_isDirty = true;

	return true;
}

bool CameraControllerLayer::OnCameraActivate(sl::CameraActivateEvent &event)
{
	m_controllerMode = event.GetMode();

	return false;
}

bool CameraControllerLayer::OnMouseButtonRelease(sl::MouseButtonReleaseEvent &event)
{
	if ((sl::CameraControllerMode::FPS == m_controllerMode && SL_MOUSE_BUTTON_2 == event.GetButton()) ||
		(sl::CameraControllerMode::Editor == m_controllerMode && SL_MOUSE_BUTTON_1 == event.GetButton()))
	{
		m_controllerMode = sl::CameraControllerMode::None;
	}

	return false;
}

bool CameraControllerLayer::OnKeyRelease(sl::KeyReleaseEvent &event)
{
	if (sl::CameraControllerMode::Editor == m_controllerMode && SL_KEY_LEFT_ALT == event.GetKey())
	{
		m_controllerMode = sl::CameraControllerMode::None;
	}

	return false;
}
