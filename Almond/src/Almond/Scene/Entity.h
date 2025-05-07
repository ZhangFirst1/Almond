#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Almond {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		// 向Entity添加Component
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			AM_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");

			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		// 获取Component
		template<typename T>
		T& GetComponent() {
			AM_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		// Entity是否具有Component T
		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		// 移除Component
		template<typename T>
		void RemoveComponent() {
			AM_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

}