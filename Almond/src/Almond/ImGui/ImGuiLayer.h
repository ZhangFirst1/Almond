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
		// virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:

		float m_Time = 0.0f;

	};

}