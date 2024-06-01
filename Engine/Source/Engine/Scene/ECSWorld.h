#pragma once

#include "Core/Log.h"
#include "Scene/CameraComponent.h"
#include "Scene/CornerstoneComponent.h"
#include "Scene/EntityIDComponent.h"
#include "Scene/RenderingComponent.h"
#include "Scene/TagComponent.h"
#include "Scene/TransformComponent.h"

#include <entt/entt.hpp>

namespace sl
{

class ECSWorld final
{
	friend class Entity;

public:
	static entt::registry &GetRegistry() { return m_registry; }

	static Entity CreateEntity(std::string name = "Empty Entity");
	// Destroy an entity and all its components.
	static void DestroyEntity(Entity entity);

	static void SetEditorCameraEntity(Entity entity);
	static Entity GetEditorCameraEntity();
	static CameraComponent &GetEditorCameraComponent();

private:
	static entt::registry m_registry;
	static Entity m_editorCameraEntity;
};

// Basically, Entity is just a tool class that allows us to use ECS more intuitively.
class Entity final
{
public:
	Entity() = default;
	Entity(entt::entity entity) : m_handle(entity) {}
	Entity(uint32_t entity) : m_handle((entt::entity)entity) {}

	void Reset() { m_handle = entt::null; }

	// Returns reference.
	template<class T, class... Args>
	decltype(auto) AddComponent(Args&&... args)
	{
		SL_ENGINE_ASSERT_INFO(!HasAllComponent<T>(), "Entity already holds component!");
		return ECSWorld::m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
	}

	// Returns reference.
	template<class T, class... Args>
	decltype(auto) ReplaceComponent(Args&&... args)
	{
		SL_ENGINE_ASSERT_INFO(HasAllComponent<T>(), "Entity does not hold component!");
		return ECSWorld::m_registry.replace<T>(m_handle, std::forward<Args>(args)...);
	}

	// Returns reference.
	template<class T, class... Args>
	decltype(auto) AddOrReplaceComponent(Args&&... args)
	{
		return ECSWorld::m_registry.emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
	}

	// Returns tuple of references.
	template<class... T>
	decltype(auto) GetComponent()
	{
		SL_ENGINE_ASSERT_INFO(HasAllComponent<T...>(), "Entity does not hold component!");
		return ECSWorld::m_registry.get<T...>(m_handle);
	}

	// Returns tuple of pointers.
	template<class... T>
	auto TryGetComponent()
	{
		return ECSWorld::m_registry.try_get<T...>(m_handle);
	}

	template<class... T>
	bool HasAllComponent()
	{
		return (ECSWorld::m_registry.all_of<T...>(m_handle));
	}

	template<class... T>
	bool HasAnyComponent()
	{
		return (ECSWorld::m_registry.any_of<T...>(m_handle));
	}

	bool HasComponent()
	{
		return !ECSWorld::m_registry.orphan(m_handle);
	}

	template<class T>
	auto RemoveComponent()
	{
		if (auto *pCornerstone = TryGetComponent<sl::CornerstoneComponent>(); pCornerstone && !pCornerstone->m_info.empty())
		{
			SL_ENGINE_WARN("Remove Cornerstone component from \"{}\"", GetComponent<sl::TagComponent>().m_name);
			SL_ENGINE_WARN("  Info: {}", pCornerstone->m_info.c_str());
		}
	
		// 'registry::remove' is safer than 'registry::erase'.
		return ECSWorld::m_registry.remove<T>(m_handle);
	}

	operator bool() const { return entt::null != m_handle; }
	operator uint32_t() const{ return (uint32_t)m_handle; }
	operator entt::entity() const { return m_handle; }

	bool operator==(const uint32_t other) const { return other == (uint32_t)m_handle; }
	bool operator!=(const uint32_t other) const { return !(operator==(other)); }

	bool operator==(const Entity &other) const { return other.m_handle == m_handle; }
	bool operator!=(const Entity &other) const { return !(operator==(other)); }

	bool operator==(const entt::entity &other) const { return other == m_handle; }
	bool operator!=(const entt::entity &other) const { return !(operator==(other)); }

private:
	entt::entity m_handle = entt::null;
};

} // namespace sl
