#include <ampch.h>
#include <Almond.h>

#include "imgui/imgui.h"

class ExampleLayer : public Almond::Layer {
public:
	ExampleLayer()
		:Layer("Example") {

	}

	void OnUpdate() override {
		if (Almond::Input::IsKeypressed(AM_KEY_TAB))
			AM_TRACE("Tab is pressed!(poll)");
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
	}

	void OnEvent(Almond::Event& event) override{
		// AM_TRACE("{0}", event);
		if (event.GetEventType() == Almond::EventType::keyPressed) {
			Almond::KeyPressedEvent& e = (Almond::KeyPressedEvent&)event;
			if(e.GetKeyCode() == AM_KEY_TAB)
				AM_TRACE("Tab is pressed!(event)");
			AM_TRACE("{0}", (char)e.GetKeyCode());
		}

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