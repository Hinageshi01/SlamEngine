#include "CameraComponent.h"

#include "Scene/ECSWorld.h"

#include <glm/gtc/matrix_transform.hpp>

namespace sl
{

void CameraComponent::Reset()
{
	m_projectionType = ProjectionType::Perspective;

	m_fov = glm::radians(45.0f);
	m_nearPlane = 0.01f;
	m_farPlane = 10000.0f;
	m_orthoSize = 10.0f;
	m_orthoNearClip = -10.0f;
	m_orthoFarClip = 10.0f;
	m_rotateSpeed = glm::radians(0.04f);
	m_maxMoveSpeed = 0.015f;
	m_maxSpeedToAcceleration = 0.004f;
	m_moveSpeedKeyShiftMultiplier = 4.0f;
	m_moveSpeedMouseScrollMultiplier = 1.0f;
	m_isDirty = true;
}

const glm::mat4 &CameraComponent::GetView()
{
	Recalculate();
	return m_viewMat;
}

const glm::mat4 &CameraComponent::GetProjection()
{
	Recalculate();
	return m_projectionMat;
}

const glm::mat4 &CameraComponent::GetViewProjection()
{
	Recalculate();
	return m_viewProjectionMat;
}

void CameraComponent::Recalculate()
{
	if (!m_isDirty)
	{
		return;
	}

	auto transform = ECSWorld::GetMainCameraEntity().GetComponent<TransformComponent>();
	auto rotation = transform.m_rotation;
	auto position = transform.m_position;

	m_frontDir.x = glm::cos(rotation.y) * glm::cos(rotation.x);
	m_frontDir.y = glm::sin(rotation.x);
	m_frontDir.z = glm::sin(rotation.y) * glm::cos(rotation.x);
	m_frontDir = glm::normalize(m_frontDir);
	m_rightDir = glm::normalize(glm::cross(m_frontDir, WorldUp));
	m_upDir = glm::normalize(glm::cross(m_rightDir, m_frontDir));

	m_viewMat = glm::lookAt(position, position + m_frontDir, m_upDir);
	if (ProjectionType::Perspective == m_projectionType)
	{
		m_projectionMat = glm::perspective(m_fov * m_fovMultiplier, m_aspect, m_nearPlane, m_farPlane);
	}
	else
	{
		float orthoLeft = -m_orthoSize * m_aspect * 0.5f;
		float orthoRight = m_orthoSize * m_aspect * 0.5f;
		float orthoBottom = -m_orthoSize * 0.5f;
		float orthoTop = m_orthoSize * 0.5f;
		m_projectionMat = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthoNearClip, m_orthoFarClip);
	}

	m_viewProjectionMat = m_projectionMat * m_viewMat;
	m_isDirty = false;
}

} // nmaespace sl
