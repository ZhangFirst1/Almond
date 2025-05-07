#pragma once

#include "entt.hpp"
#include "Almond/Core/Timestep.h"

namespace Almond {
	
	// 向前声明
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		// Temp 不应将entt的内容返回给客户端
		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry m_Registry;		// entt的数据结构，用于管理实体和组件
		uint32_t m_ViewportWidth, m_ViewportHeight;

		friend class Entity;
	};

}