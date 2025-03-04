#pragma once
// 鼠标事件

#include "Event.h"

namespace Almond {
	// 鼠标移动---------------------------------------------------
	class ALMOND_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float mouseX, float mouseY)
			:m_MouseX(mouseX), m_MouseY(mouseY) {}

		inline float GetX() const { return m_MouseX;  }
		inline float GetY() const { return m_MouseY;  }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_MouseX, m_MouseY;
	};
	// 鼠标滚动---------------------------------------------------
	class ALMOND_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float mouseX, float mouseY)
			:m_XOffset(mouseX), m_YOffset(mouseY) {}

		inline float GetXOffset() const { return m_YOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_XOffset, m_YOffset;
	};

	// 按钮---------------------------------------------------
	class ALMOND_API MouseButtonEvent : public Event {
	public:
		inline int GetButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	// 点击按钮--------------------------------------------------
	class ALMOND_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button)
			:MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	// 松开按钮--------------------------------------------------
	class ALMOND_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			:MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}