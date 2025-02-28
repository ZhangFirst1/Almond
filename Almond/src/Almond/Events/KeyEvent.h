#pragma once
// 键盘事件

#include "Event.h"

namespace Almond {
	// 键盘事件基类-------------------------------------------------------------------
	class ALMOND_API KeyEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)	// 事件：输入事件		输入设备：键盘

		inline int GetKeyCode() const { return m_KeyCode; }

	protected:
		KeyEvent(int keycode)						// 保护构造函数 子类可以访问
			:m_KeyCode(keycode) {}
		
		int m_KeyCode;
	};

	// 按下键盘-----------------------------------------------------------------------
	class ALMOND_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			:KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatcount() const { return m_RepeatCount; }

		EVENT_CLASS_TYPE(keyPressed)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << "(" << m_RepeatCount << "repeats)" << std::endl;
			return ss.str();
		}
		
	private:
		int m_RepeatCount;							// 按键重复计数
	};

	// 松开键盘-------------------------------------------------------------------------
	class ALMOND_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode)
			:KeyEvent(keyCode) {}

		EVENT_CLASS_TYPE(KeyReleased)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode << std::endl;
			return ss.str();
		}

	};

	// 输入文字-------------------------------------------------------------------------
	class ALMOND_API KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keyCode)
			:KeyEvent(keyCode) {}

		EVENT_CLASS_TYPE(KeyTyped)
		
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode << std::endl;
			return ss.str();
		}
	};

}