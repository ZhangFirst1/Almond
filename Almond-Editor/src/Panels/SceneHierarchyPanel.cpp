#include "Almond/Scene/Components.h"
#include "SceneHierarchyPanel.h"


#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>


/* TODO: 为一个Entity重复添加Component会报错，需要阻止这种行为 */

namespace Almond {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene Hierarchy");

		if(m_Context){
			m_Context->m_Registry.view<entt::entity>().each([&](auto entityID) {
				Entity entity{ entityID, m_Context.get() };	// 此处使用get获取原始指针
				DrawEntityNode(entity);
				});
			// 当按下左键且鼠标在SceneHierarchyPanel, 将选择的目标置空
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

			// 空白处右键，不会在内部的项上弹出
			if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems)) {
				if (ImGui::MenuItem("Create Empty Entity"))
					m_Context->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext) {
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;;

		// TreeNodeEx(树节点ID，树节点标志用于控制节点行为，树节点标签) 返回值为该节点是否被展开
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		// 判断当前entity是否被点击
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
		}

		bool entityDelete = false;		// 延迟删除，以便处理完当前待删除entity的剩余工作
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity"))
				entityDelete = true;

			ImGui::EndPopup();
		}

		if (opened) {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			// TODO: Delete
			bool opened = ImGui::TreeNodeEx((void*)66666, flags, tag.c_str());
			if (opened) {
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		// 删除entity
		if (entityDelete) {
			m_Context->DestroyEntity(entity);
			// 删除的是当前选中的entity，则需要将其置空
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	// 控制颜色的组件
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		// 更改字体
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];		// 粗体

		// 为当前控件推入一个唯一的 ID，避免与其他控件发生 ID 冲突
		ImGui::PushID(label.c_str());

		// 设置两列布局
		ImGui::Columns(2);
		// 设置第一列（标签列）的宽度 和 标签
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		// 切换到第二列
		ImGui::NextColumn();
		// 设置后续 3 个控件的宽度为自动计算宽度，适配 3 个项
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		// 设置项之间的间距为 0（使得按钮和拖动条无缝连接）
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		// 设置按钮大小
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// --------------------------- X 分量 ---------------------------

		// 设置按钮颜色为红色风格
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		// 如果按下 "X" 按钮，将 x 分量重置为指定的 resetValue
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		// 恢复颜色
		ImGui::PopStyleColor(3);

		// 在同一行放置拖动条
		ImGui::SameLine();
		// 拖动条用于编辑 x 分量
		// 通过使用 ##，即使视觉标签相同，只要标识符不同，ImGui 就会将它们视为独立的控件。
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		// 弹出对应的宽度设置
		ImGui::PopItemWidth();

		// 继续在同一行绘制 Y 控件
		ImGui::SameLine();

		// --------------------------- Y 分量 ---------------------------

		// 设置按钮颜色为绿色风格
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

		// 如果按下 "Y" 按钮，将 y 分量重置为 resetValue
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		// 恢复颜色
		ImGui::PopStyleColor(3);

		// 拖动条
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		// 同一行继续绘制 Z 控件
		ImGui::SameLine();

		// --------------------------- Z 分量 ---------------------------

		// 设置按钮颜色为蓝色风格
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		// 如果按下 "Z" 按钮，将 z 分量重置为 resetValue
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		// 拖动条
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		// 恢复样式变量（ItemSpacing）
		ImGui::PopStyleVar();

		// 恢复列布局为单列
		ImGui::Columns(1);

		// 弹出 ID，结束当前控件作用域
		ImGui::PopID();

	}

	// T为Component类型，UIFunction为可调用对象（如lambda），用于绘制component的UI
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity& entity, UIFunction uiFunction)
	{
		// 设置默认样式，默认打开、有边框、适应可用宽度、允许重叠使用内边距
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		// 如果有T组件
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();						// 获取当前内容区域的可用空间（用于计算按钮位置）

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	// 计算一行的高度
			ImGui::Separator();		// 分割线
			// 创建一个可展开/折叠的 TreeNode，标题为传入的 name，唯一标识为组件类型的哈希值
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());	
			ImGui::PopStyleVar();

			// 在同一行，绘制一个 “+” 按钮，用于弹出组件设置菜单
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");				// 右键菜单
			}

			bool removeComponent = false;							// 移除组件
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			// 如果 TreeNode 是打开的，调用传入的 UI 绘制函数
			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
			// 移除组件
			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity& entity)
	{
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			// 可以输入字符改变Tag的名称
			char buffer[256];
			memset(buffer, 0, sizeof buffer);
			strcpy_s(buffer, sizeof buffer, tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof buffer)) {
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		// 如果点击了 "Add component" 按钮，打开一个弹出菜单
		if (ImGui::Button("Add component"))
			ImGui::OpenPopup("AddComponet");

		// 如果菜单正在打开
		if (ImGui::BeginPopup("AddComponet")) {
			if (ImGui::MenuItem("Camera")) {
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();		// 添加完毕后关闭弹出菜单
			}

			if (ImGui::MenuItem("Sprite Renderer")) {
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
			auto& camera = component.Camera;
			// 主相机
			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			// 正交相机
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);
			}
			// 透视相机
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
				float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &verticalFOV))
					camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
				// 固定宽高比
				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}

		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});


	}// End Of DrawComponents()

}