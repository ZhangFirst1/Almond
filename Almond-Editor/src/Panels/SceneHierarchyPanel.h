#pragma once

#include "Almond/Core/Core.h"
#include "Almond/Scene/Scene.h"
#include "Almond/Scene/Entity.h"

namespace Almond {

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity& entity); // Scene Hierarchy窗口
		void DrawComponents(Entity& entity); // Properties窗口
	private:
		Ref<Scene> m_Context;				// 构造时赋值，从Scene中获取Entity
		Entity m_SelectionContext;			// 选择的entity
	};

}