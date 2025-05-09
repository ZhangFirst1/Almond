#pragma once

#include "Almond/Core/Layer.h"
#include "Almond/Events/KeyEvent.h"
#include "Almond/Events/MouseEvent.h"
#include "Almond/Events/ApplicationEvent.h"
 
namespace Almond {

	class ALMOND_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;
		// virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();		// 设置暗系主题
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;

	};

}