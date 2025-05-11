#pragma once

#include "entt.hpp"
#include "Almond/Core/Timestep.h"
#include "Almond/Renderer/EditorCamera.h"

namespace Almond {
	
	// 向前声明
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		// Temp 不应将entt的内容返回给客户端
		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity& entity);
		
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;		// entt的数据结构，用于管理实体和组件
		uint32_t m_ViewportWidth, m_ViewportHeight;	// 视口大小

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}