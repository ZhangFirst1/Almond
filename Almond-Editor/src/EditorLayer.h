#pragma once

#include "Almond.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Almond {

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;

	private:
		OrthographicCameraController m_CameraController;	// 摄像机控制器

		Ref<VertexArray> m_SquareVA;						// VAO指针
		Ref<Shader> m_FlatColorShader;
		Ref<Texture2D> m_WoodTexture;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity, m_SecondCamera;

		bool m_PrimaryCamera = true;

		bool m_ViewportFocusd = false, m_ViewportHovered = false;// 是否在当前视口

		glm::vec2 m_ViewportSize = {0.0f, 0.0f};				// 视口大小
		glm::vec4 m_SquareColor = { 0.1f, 0.2f, 0.8f, 1.0f };

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}