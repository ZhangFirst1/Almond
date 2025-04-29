#include "ampch.h"
#include "Sandbox2d.h"
#include "imgui/imgui.h"
#include "Almond/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>	

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {

}

void Sandbox2D::OnAttach()
{
	m_WoodTexture = Almond::Texture2D::Create("assets/textures/wood.png"); 
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = Almond::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCall);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnUpdate(Almond::Timestep ts)
{
	AM_PROFILE_FUNCTION();
	
	// Update
	{
		AM_PROFILE_SCOPE("Camera::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render
	Almond::Renderer2D::ResetStats();
	{
		AM_PROFILE_SCOPE("Renderer Prep");
		Almond::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Almond::RendererCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		AM_PROFILE_SCOPE("Renderer Draw");
		Almond::Renderer2D::BeginScene(m_CameraController.GetCamera());						// 负责每帧渲染前的环境设置
		Almond::Renderer2D::DrawRotatedQuad({ 1.0f, 1.0f }, { 0.6f, 0.6f }, -45.0f, { 0.1f, 0.8f, 0.3f, 1.0f });
		Almond::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Almond::Renderer2D::DrawQuad({ -2.0f, -1.0f }, { 0.5f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f });
		Almond::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_WoodTexture, 10.0f);		// 深度-0.1f，使用右手坐标系
		Almond::Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, rotation, m_WoodTexture, 20.0f);		// 深度-0.1f，使用右手坐标系
		Almond::Renderer2D::EndScene();		

		Almond::Renderer2D::BeginScene(m_CameraController.GetCamera());						// 负责每帧渲染前的环境设置
		for(float y = -5.0f; y < 5.0f; y += 0.5f)
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.5f };
				Almond::Renderer2D::DrawQuad({ x, y }, {0.45f, 0.45f}, color);
			}

		Almond::Renderer2D::EndScene();

	}

}

void Sandbox2D::OnEvent(Almond::Event& e)
{
	m_CameraController.OnEvent(e);
}
