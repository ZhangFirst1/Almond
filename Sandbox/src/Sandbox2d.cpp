#include "ampch.h"
#include "Sandbox2d.h"
#include "imgui/imgui.h"
#include "Almond/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWDDDDDDDDWWWWWDWWW"
"WWWWWWDDDDDDDDDWWWWDDDWW"
"WWWWWDDDDDDDDDDDWWWWDDWW"
"WWWWDDDWWDDDDDDDDWWWWWWW"
"WWWWDDDWWWDDDDDDDDWWWWWW"
"WWWWDDDDWDDDDDDDDDWWWWWW"
"WWWWWDDDDDDDDDDDDDWWCWWW"
"WWWWWWDDDDDDDDDDDWWWWWWW"
"WWWWWWWDDDDDDDDDWWWWWWWW"
"WWWWWWWWDDDDDDDWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {

}

void Sandbox2D::OnAttach()
{
	m_WoodTexture = Almond::Texture2D::Create("assets/textures/wood.png"); 
	m_SpriteSheet = Almond::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");


	//m_TextureStairs = Almond::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128, 128 });
	//m_TextureBarrel = Almond::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 2 }, { 128, 128 });
	//m_TextureTree = Almond::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, {1, 2});

	//m_MapWidth = s_MapWidth;
	//m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	//s_TextureMap['D'] = Almond::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 });
	//s_TextureMap['W'] = Almond::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 });

	//// Init here
	//m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	//m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	//m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	//m_Particle.LifeTime = 1.0f;
	//m_Particle.Velocity = { 0.0f, 0.0f };
	//m_Particle.VelocityVariation = { 3.0f, 1.0f };
	//m_Particle.Position = { 0.0f, 0.0f };

	//m_CameraController.SetZoomLevel(5.0f);
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnImGuiRender()
{
	AM_PROFILE_FUNCTION();
	        
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
		Almond::Renderer2D::DrawRotatedQuad({ 1.0f, 1.0f }, { 0.6f, 0.6f }, glm::radians(-45.0f), {0.1f, 0.8f, 0.3f, 1.0f});
		Almond::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Almond::Renderer2D::DrawQuad({ -2.0f, -1.0f }, { 0.5f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f });
		Almond::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_WoodTexture, 10.0f);		// 深度-0.1f，使用右手坐标系
		Almond::Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_WoodTexture, 20.0f);		// 深度-0.1f，使用右手坐标系
		Almond::Renderer2D::EndScene();		

		Almond::Renderer2D::BeginScene(m_CameraController.GetCamera());						// 负责每帧渲染前的环境设置
		for(float y = -5.0f; y < 5.0f; y += 0.5f)
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.5f };
				Almond::Renderer2D::DrawQuad({ x, y }, {0.45f, 0.45f}, color);
			}

		Almond::Renderer2D::EndScene();
	}


	//if (Almond::Input::IsMouseButtonPressed(AM_MOUSE_BUTTON_LEFT))
	//{
	//	auto [x, y] = Almond::Input::GetMousePosition();
	//	auto width = Almond::Application::Get().GetWindow().GetWidth();
	//	auto height = Almond::Application::Get().GetWindow().GetHeight();

	//	auto bounds = m_CameraController.GetBounds();
	//	auto pos = m_CameraController.GetCamera().GetPosition();
	//	x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
	//	y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
	//	m_Particle.Position = { x + pos.x, y + pos.y };
	//	for (int i = 0; i < 5; i++)
	//		m_ParticleSystem.Emit(m_Particle);
	//}

	//m_ParticleSystem.OnUpdate(ts);
	//m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	//Almond::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//for (uint32_t y = 0; y < m_MapHeight; y++) {
	//	for (uint32_t x = 0; x < m_MapWidth; x++) {
	//		char tileType = s_MapTiles[x + m_MapWidth * y];
	//		Almond::Ref<Almond::SubTexture2D> texture;
	//		if (s_TextureMap.find(tileType) != s_TextureMap.end())
	//			texture = s_TextureMap[tileType];
	//		else
	//			texture = m_TextureBarrel;

	//		Almond::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f, 0.0f }, { 1.0f, 1.0f }, texture);
	//	}
	//}

	//Almond::Renderer2D::DrawQuad({ 0.0f,0.0f,0.0f }, { 1.0f, 1.0f }, m_TextureStairs);
	//Almond::Renderer2D::DrawQuad({ 1.0f,0.0f,0.0f }, { 1.0f, 1.0f }, m_TextureBarrel);
	//Almond::Renderer2D::DrawQuad({ -1.0f,0.0f,0.0f }, { 1.0f, 2.0f }, m_TextureTree);
	//Almond::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Almond::Event& e)
{
	m_CameraController.OnEvent(e);
}
