#pragma once
// 轮询，用以获取输入状态

#include "Almond/Core/Core.h"

namespace Almond {
	class ALMOND_API Input {
	public:
		virtual ~Input() = default;

		static bool IsKeypressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition(); 
		static float GetMouseX();
		static float GetMouseY();
	};

}