#pragma once

#include "Scene.h"
#include "entt.hpp"
#include "Almond/Core/Log.h"

namespace Almond {
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AM_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			AM_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			AM_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
		entt::entity m_EntityHandle{ entt::null };
	private:
		Scene* m_Scene = nullptr;
	};

	//class Entity {
	//public:
	//	Entity() = default;
	//	Entity(entt::entity handle, Scene* scene);
	//	Entity(const Entity& other) = default;

	//	// 向Entity添加Component
	//	template<typename T, typename... Args>
	//	T& AddComponent(Args&&... args) {
	//		AM_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
	//		T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
	//		// 添加组件到实体时执行特定的初始化逻辑
	//		m_Scene->OnComponentAdded<T>(*this, component);
	//		return component;
	//	}

	//	// 获取Component
	//	template<typename T>
	//	T& GetComponent() {
	//		AM_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

	//		return m_Scene->m_Registry.get<T>(m_EntityHandle);
	//	}

	//	// Entity是否具有Component T
	//	template<typename T>
	//	bool HasComponent() {
	//		return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
	//	}

	//	// 移除Component
	//	template<typename T>
	//	void RemoveComponent() {
	//		AM_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
	//		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	//	}

	//	operator bool() const { return m_EntityHandle != entt::null; }
	//	operator uint32_t() const { return (uint32_t)m_EntityHandle; }
	//	operator entt::entity() const { return m_EntityHandle; }

	//	bool operator ==(const Entity& other) const {
	//		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	//	}

	//	bool operator !=(const Entity& other) const {
	//		return !(*this == other);
	//	}
	//private:
	//	entt::entity m_EntityHandle = entt::null;
	//	Scene* m_Scene = nullptr;
	//};

}