#pragma once

#include "Almond.h"


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

	Almond::OrthographicCameraController m_CameraController;// 摄像机控制器

	glm::vec4 m_SquareColor = { 0.1f, 0.2f, 0.8f, 1.0f };
};