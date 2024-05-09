#pragma once

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sl
{

enum class ProjectionType : uint8_t
{
	Perspective = 0,
	Orthographic = 1,
};

struct CameraComponent
{
	static constexpr glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };

	void Reset();

	const glm::mat4 &GetView();
	const glm::mat4 &GetProjection();
	const glm::mat4 &GetViewProjection();
	void Recalculate();

	ProjectionType m_projectionType = ProjectionType::Perspective;

	// Perspective datas
	float m_aspect = 1920.0f / 1080.0f;
	float m_fov = glm::radians(45.0f); // Stores in radians
	float m_fovMultiplier = 1.0f;
	float m_nearPlane = 0.01f;
	float m_farPlane = 10000.0f;

	// Orthographic datas
	float m_orthoSize = 10.0f;
	float m_orthoNearClip = -10.0f;
	float m_orthoFarClip = 10.0f;

	// Camera Controller datas
	float m_rotateSpeed = glm::radians(0.04f);

	float m_maxMoveSpeed = 0.015f;
	float m_maxSpeedToAcceleration = 0.004f;
	float m_acceleration = 0.0f;
	float m_moveSpeed = 0.0f;

	float m_moveSpeedKeyShiftMultiplier = 4.0f;
	float m_moveSpeedMouseScrollMultiplier = 1.0f;

	glm::vec2 m_mouseLastPos{ 0.0f, 0.0f };
	glm::vec3 m_lastMoveDir{ 0.0f, 0.0f, 0.0f };

	// Caches
	bool m_isDirty = true;

	glm::vec3 m_frontDir{ 0.0f, 0.0f, 1.0f };
	glm::vec3 m_upDir{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_rightDir{ 1.0f, 0.0f, 0.0f };
	
	glm::mat4 m_viewMat{ 1.0f };
	glm::mat4 m_projectionMat{ 1.0f };
	glm::mat4 m_viewProjectionMat{ 1.0f };
};

} // namespace sl
