#pragma once
#include "Almond/Input.h"

namespace Almond {
	class WindowsInput : public Input {
	protected:
		virtual bool IskeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button);
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}