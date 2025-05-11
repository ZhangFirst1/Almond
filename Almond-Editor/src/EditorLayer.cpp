#include "ampch.h"
#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "Almond/Renderer/Renderer2D.h"
#include "Almond/Scene/SceneSerializer.h"
#include "Almond/Utils/PlatformUtils.h"
#include "Almond/Math/Math.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

namespace Almond {

	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f) {

	}

	void EditorLayer::OnAttach()
	{

		

		m_WoodTexture = Texture2D::Create("assets/textures/wood.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		// 创建Scene
		m_ActiveScene = CreateRef<Scene>();

#if 0
		// 创建实体
		Entity squareGreen = m_ActiveScene->CreateEntity("Green Square");
		squareGreen.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		Entity squareRed = m_ActiveScene->CreateEntity("Red Square");
		squareRed.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_SquareEntity = squareGreen;


		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity {
		public:
			void OnCreate() {
				auto& tanslation = GetComponent<TransformComponent>().Translation;
				tanslation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy() {

			}

			void OnUpdate(Timestep ts) {
				auto& tanslation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeypressed(AM_KEY_A))
					tanslation.x -= speed * ts;
				if (Input::IsKeypressed(AM_KEY_D))
					tanslation.x += speed * ts;
				if (Input::IsKeypressed(AM_KEY_W))
					tanslation.y += speed * ts;
				if (Input::IsKeypressed(AM_KEY_S))
					tanslation.y -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

#endif
		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);	// 为SceneHierarchyPanel设置场景
	}

	void EditorLayer::OnDetach() {

	}

	void EditorLayer::OnImGuiRender()
	{
		// copy from imgui_demo.cpp ShowExampleAppDockSpace

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		// 限制窗口停靠时最小宽度
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// 新建一个场景
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				// 打开文件（反序列化）
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				// 另存为（序列化）
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCall);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportOffset = ImGui::GetCursorPos();							// 获取鼠标位置，相对于当前窗口左上角的坐标

		m_ViewportFocusd = ImGui::IsWindowFocused();							// 鼠标是否聚焦当前视口
		m_ViewportHovered = ImGui::IsWindowHovered();							// 鼠标是否停留当前视口
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocusd && !m_ViewportHovered);	// 如果聚焦则不阻塞事件

		ImVec2 viewportPaneSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPaneSize.x, viewportPaneSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		auto windowSize = ImGui::GetWindowSize();		// 返回当前窗口的宽度
		ImVec2 minBound = ImGui::GetWindowPos();		// 返回当前窗口左上角相较于屏幕的坐标位置
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + m_ViewportSize.x, minBound.y + m_ViewportSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };// 确定窗口边界
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };
			
		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		// 选中物体且Gizmo类型合法
		if (selectedEntity && m_GizmoType != -1)
		{
			// 设置投影模式为正交
			ImGuizmo::SetOrthographic(true);
			// 设置 ImGuizmo 使用默认绘制列表（使用当前 ImGui 窗口的绘制列表）
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			// 设置 Gizmo 的绘制区域为整个窗口

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			//auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeypressed(Key::LeftControl);
			float snapValue = 0.5f;
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			// 渲染并处理 Gizmo 操作，用户可以通过 Gizmo 操作 transform
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			// 如果正在使用 Gizmo（鼠标拖动）
			if (ImGuizmo::IsUsing())
			{
				// 将变换矩阵分解为平移、旋转、缩放分量
				glm::vec3 translation, rotation, scale;
				std::cout << transform[0][3] << std::endl;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				// 计算旋转变化量（因为 ImGuizmo 的输出是绝对值）
				glm::vec3 deltaRotation = rotation - tc.Rotation;
				// 设置新的变换
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;

				std::cout << translation.x << ' ' << translation.y << ' ' << translation.z << std::endl;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		AM_PROFILE_FUNCTION();
		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update
		// 只有悬停在视口才会更新摄像机
		if (m_ViewportFocusd) 
			m_CameraController.OnUpdate(ts);
		
		m_EditorCamera.OnUpdate(ts);
		
		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RendererCommand::Clear();

		// Clear our entity ID attachemnt to -1
		m_Framebuffer->ClearAttachment(1, -1);

		// Update Scene
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		auto [mx, my] = ImGui::GetMousePos();// 返回当前鼠标光标在屏幕上的坐标（全局坐标）
		mx -= m_ViewportBounds[0].x;		 // 通过全局坐标相减，获取鼠标在视口中的位置
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;			// OpenGL左下角是(0,0) ImGui左上角是(0,0) 需要反转一下坐标
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			// 读取像素值（第二个颜色缓冲的值）
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		// 解绑帧缓冲区
		m_Framebuffer->UnBind();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(AM_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
		// 快捷键
		if (e.GetRepeatcount() > 0)
			return false;

		bool control = Input::IsKeypressed(Key::LeftControl) || Input::IsKeypressed(Key::RightControl);
		bool shift = Input::IsKeypressed(Key::LeftShift) || Input::IsKeypressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (control)
				NewScene();

			break;
		}
		case Key::O:
		{
			if (control)
				OpenScene();

			break;
		}
		case Key::S:
		{
			if (control && shift)
				SaveSceneAs();

			break;
		}
		// Gizmos
		case Key::Q:
		{
			m_GizmoType = -1;
			break;
		}
		case Key::W:
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Key::E:
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::R:
		{
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}

		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == Mouse::ButtonLeft)
			m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();					// 打开新场景时，新建一个场景
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);	// SceneHierarchyPanel设置场景
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Almond Scene (*.almond)\0*.almond\0");
		if (!filepath.empty()) {
			m_ActiveScene = CreateRef<Scene>();					// 打开新场景时，新建一个场景
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);	// SceneHierarchyPanel设置场景

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Almond Scene (*.almond)\0*.almond\0");
		if (!filepath.empty()) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}

}