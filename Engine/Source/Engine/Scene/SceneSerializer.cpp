#include "SceneSerializer.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "Scene/ECSWorld.h"
#include "Scene/YAMLConvert.hpp"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace sl
{

void SceneSerializer::SerializeYAML(const char *sceneName)
{
	std::string outputPath = Path::FromeRoot(Path::Join("Project", sceneName));
	outputPath += ".yaml";
	SL_ENGINE_INFO("Serialize scene data file: \"{}\"", outputPath);

	YAML::Emitter out;
	out << YAML::Comment("Generated by yaml-cpp");

	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << sceneName;
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

	auto view = sl::ECSWorld::GetRegistry().view<sl::TagComponent>();
	for (auto entity : view)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity ID" << YAML::Value << (uint32_t)entity;

		if (auto *pTag = sl::ECSWorld::GetRegistry().try_get<sl::TagComponent>(entity); pTag)
		{
			out << YAML::Key << "Tag Component";
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << pTag->m_name;
			out << YAML::EndMap;
		}

		if (auto *pTransform = sl::ECSWorld::GetRegistry().try_get<sl::TransformComponent>(entity); pTransform)
		{
			out << YAML::Key << "Transform Component";
			out << YAML::BeginMap;
			out << YAML::Key << "Position" << YAML::Value << pTransform->m_position;
			out << YAML::Key << "Rotation" << YAML::Value << pTransform->m_rotation;
			out << YAML::Key << "Scale" << YAML::Value << pTransform->m_scale;
			out << YAML::EndMap;
		}

		if (auto *pCamera = sl::ECSWorld::GetRegistry().try_get<sl::CameraComponent>(entity); pCamera)
		{
			out << YAML::Key << "Camera Component";
			out << YAML::BeginMap;
			
			out << YAML::Key << "Projection Type" << YAML::Value << pCamera->m_projectionType;

			out << YAML::Key << "Perspective";
			out << YAML::BeginMap;
			out << YAML::Key << "FOV" << YAML::Value << pCamera->m_fov;
			out << YAML::Key << "Near Plane" << YAML::Value << pCamera->m_nearPlane;
			out << YAML::Key << "Far Plane" << YAML::Value << pCamera->m_farPlane;
			out << YAML::EndMap;

			out << YAML::Key << "Orthographic";
			out << YAML::BeginMap;
			out << YAML::Key << "Size" << YAML::Value << pCamera->m_orthoSize;
			out << YAML::Key << "Near Clip" << YAML::Value << pCamera->m_orthoNearClip;
			out << YAML::Key << "Far Clip" << YAML::Value << pCamera->m_orthoFarClip;
			out << YAML::EndMap;

			out << YAML::Key << "Controller";
			out << YAML::BeginMap;
			out << YAML::Key << "Rotate Speed" << YAML::Value << pCamera->m_rotateSpeed;
			out << YAML::Key << "Move Speed" << YAML::Value << pCamera->m_maxMoveSpeed;
			out << YAML::Key << "Acceleration" << YAML::Value << pCamera->m_maxSpeedToAcceleration;
			out << YAML::Key << "Shift Multiplier" << YAML::Value << pCamera->m_moveSpeedKeyShiftMultiplier;
			out << YAML::Key << "Scroll Multiplier" << YAML::Value << pCamera->m_moveSpeedMouseScrollMultiplier;
			out << YAML::EndMap;

			out << YAML::EndMap; // CameraComponent
		}

		if (auto *pRendering = sl::ECSWorld::GetRegistry().try_get<sl::RenderingComponent>(entity); pRendering)
		{
			out << YAML::Key << "Rendering Component";
			out << YAML::BeginMap;

			out << YAML::Key << "Shader" << YAML::Value << "Shader name goes here";

			out << YAML::EndMap;
		}

		if (auto *pCornerstone = sl::ECSWorld::GetRegistry().try_get<sl::CornerstoneComponent>(entity); pCornerstone)
		{
			out << YAML::Key << "Cornerstone Component";
			out << YAML::BeginMap;

			out << YAML::Key << "Info" << YAML::Value << pCornerstone->m_info;

			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Entity
	}
	out << YAML::EndSeq; // Entities
	out << YAML::EndMap; // Scene

	std::ofstream fileStream{ outputPath };
	fileStream << out.c_str();
}

void SceneSerializer::SerializeBinary()
{

}

bool SceneSerializer::DeserializeYAML(const char *sceneName)
{
	std::string filePath = Path::FromeRoot(Path::Join("Project", sceneName));
	filePath += ".yaml";
	SL_ENGINE_INFO("Deserialize scene data file: \"{}\"", filePath);

	std::ifstream fileStream{ filePath };
	if (fileStream.fail())
	{
		SL_ENGINE_ERROR("Can not open scene data file: \"{}\"", filePath);
		return false;
	}
	std::stringstream stringStream;
	stringStream << fileStream.rdbuf();

	YAML::Node data = YAML::Load(stringStream.str());
	if (!data["Scene"])
	{
		SL_ENGINE_ERROR("Unknown scene data file: \"{}\"", filePath);
		return false;
	}

	SL_ENGINE_ERROR("Jest some test code here, will not actually open a new scene.");

	std::string scene = data["Scene"].as<std::string>();
	SL_ENGINE_TRACE("Scene: {}", scene);

	if (auto entities = data["Entities"]; entities)
	{
		for (const auto entity : entities)
		{
			// Entity ID
			if (auto entityID = entity["Entity ID"]; entityID)
			{
				SL_ENGINE_TRACE("  Entity ID: {}", entityID.as<uint32_t>());
			}

			// Tag Component
			if (auto tag = entity["Tag Component"]; tag)
			{
				SL_ENGINE_TRACE("    Tag Component:");
				if (auto name = tag["Name"]; name)
				{
					SL_ENGINE_TRACE("      Name: {}", name.as<std::string>());
				}
			}

			// Transform Component
			if (auto transform = entity["Transform Component"]; transform)
			{
				SL_ENGINE_TRACE("    Transform Component:");
				if (auto position = transform["Position"]; position)
				{
					SL_ENGINE_TRACE("      Position: {}", position.as<glm::vec3>());
				}
				if (auto rotation = transform["Rotation"]; rotation)
				{
					SL_ENGINE_TRACE("      Rotation: {}", rotation.as<glm::vec3>());
				}
				if (auto scale = transform["Scale"]; scale)
				{
					SL_ENGINE_TRACE("      Scale: {}", scale.as<glm::vec3>());
				}
			}

			// Camera Component
			if (auto camera = entity["Camera Component"]; camera)
			{
				SL_ENGINE_TRACE("    Camera Component:");

				if (auto projectionType = camera["Projection Type"]; projectionType)
				{
					SL_ENGINE_TRACE("      Projection Type: {}", nameof::nameof_enum(projectionType.as<sl::ProjectionType>()));
				}

				if (auto perspective = camera["Perspective"]; perspective)
				{
					SL_ENGINE_TRACE("      Perspective:");
					if (auto fov = perspective["FOV"]; fov)
					{
						SL_ENGINE_TRACE("        FOV: {}", fov.as<float>());
					}
					if (auto nearPlan = perspective["Near Plane"]; nearPlan)
					{
						SL_ENGINE_TRACE("        Near Plane: {}", nearPlan.as<float>());
					}
					if (auto farPaln = perspective["Far Plane"]; farPaln)
					{
						SL_ENGINE_TRACE("        Far Plane: {}", farPaln.as<float>());
					}
				}
				if (auto orthographic = camera["Orthographic"]; orthographic)
				{
					SL_ENGINE_TRACE("      Orthographic:");
					if (auto size = orthographic["Size"]; size)
					{
						SL_ENGINE_TRACE("        Size: {}", size.as<float>());
					}
					if (auto nearClip = orthographic["Near Clip"]; nearClip)
					{
						SL_ENGINE_TRACE("        Near Clip: {}", nearClip.as<float>());
					}
					if (auto farClip = orthographic["Far Clip"]; farClip)
					{
						SL_ENGINE_TRACE("        Far Clip: {}", farClip.as<float>());
					}
				}
				if (auto controller = camera["Controller"]; controller)
				{
					SL_ENGINE_TRACE("      Controller:");
					if (auto rotateSpeed = controller["Rotate Speed"]; rotateSpeed)
					{
						SL_ENGINE_TRACE("        Rotate Speed: {}", rotateSpeed.as<float>());
					}
					if (auto moveSpeed = controller["Move Speed"]; moveSpeed)
					{
						SL_ENGINE_TRACE("        Move Speed: {}", moveSpeed.as<float>());
					}
					if (auto acceleration = controller["Acceleration"]; acceleration)
					{
						SL_ENGINE_TRACE("        Acceleration: {}", acceleration.as<float>());
					}
					if (auto shiftMultiplier = controller["Shift Multiplier"]; shiftMultiplier)
					{
						SL_ENGINE_TRACE("        Shift Multiplier: {}", shiftMultiplier.as<float>());
					}
					if (auto scrollMultiplier = controller["Scroll Multiplier"]; scrollMultiplier)
					{
						SL_ENGINE_TRACE("        Scroll Multiplier: {}", scrollMultiplier.as<float>());
					}
				}
			}

			// Redering Component
			if (auto redering = entity["Redering Component"]; redering)
			{
				SL_ENGINE_TRACE("    Redering Component:");
				if (auto shader = redering["Shader"]; shader)
				{
					SL_ENGINE_TRACE("      Shader: {}", shader.as<std::string>());
				}
			}

			// Cornerstone Component
			if (auto cornerstone = entity["Cornerstone Component"]; cornerstone)
			{
				SL_ENGINE_TRACE("    Cornerstone Component:");
				if (auto info = cornerstone["Info"]; info)
				{
					SL_ENGINE_TRACE("      Info: {}", info.as<std::string>());
				}
			}
		}
	}

	return true;
}

bool SceneSerializer::DeserializeBinary()
{
	return false;
}

} // namespace sl
