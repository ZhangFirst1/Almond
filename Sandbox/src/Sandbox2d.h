#pragma once

#include "Almond.h"

#include "ParticleSystem.h"

class Sandbox2D : public Almond::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnImGuiRender() override;

	void OnUpdate(Almond::Timestep ts) override;
	void OnEvent(Almond::Event& e) override;

private:
	Almond::Ref<Almond::VertexArray> m_SquareVA;			// VAO指针
	Almond::Ref<Almond::Shader> m_FlatColorShader;
	Almond::Ref<Almond::Texture2D> m_WoodTexture;
	Almond::Ref<Almond::Texture2D> m_SpriteSheet;
	Almond::Ref<Almond::SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;

	Almond::OrthographicCameraController m_CameraController;// 摄像机控制器

	glm::vec4 m_SquareColor = { 0.1f, 0.2f, 0.8f, 1.0f };

	ParticleSystem m_ParticleSystem;						// 粒子系统
	ParticleProps m_Particle;								// 粒子属性

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Almond::Ref<Almond::SubTexture2D>> s_TextureMap;
};