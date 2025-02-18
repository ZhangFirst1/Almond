#include <ampch.h>
#include <Almond.h>

class ExampleLayer : public Almond::Layer {
public:
	ExampleLayer()
		:Layer("Example") {

	}

	void OnUpdate() override {
		AM_INFO("ExampleLayer::Update");
	}

	void OnEvent(Almond::Event& event) {
		AM_TRACE("{0}", event);
	}
};

class Sandbox : public Almond::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}

};

Almond::Application* Almond::CreateApplication() {
	return new Sandbox();
}