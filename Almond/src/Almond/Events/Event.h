#pragma once
#include "Almond/Core.h"

namespace Almond {

	// 事件类，目前是当事件发生时立刻响应

	// 类别-----------------------------------------------------------
	// enum class有作用域限制，EventType::    enum不需要
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		keyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// 事件分类  BIT宏展开为1 << x
	enum EventCategory {								
		None = 0,
		EventCategoryApplication = BIT(0),		
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	// 宏-----用以自动化重写虚函数--------------------------------------
	// 使用静态成员函数 因为在获取事件类型时并不需要创建类的对象
	// #type用于将type转为字符串形式
#define EVENT_CLASS_TYPE(type) \
						static EventType GetStaticType() { return EventType::type; } \
						virtual EventType GetEventType() const override { return GetStaticType(); } \
						virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
						virtual int GetCategoryFlags() const override { return category; }

	// 事件类-----------------------------------------------------------
	class ALMOND_API Event {
	public:

		virtual ~Event() = default;

		bool Handled = false;										// 事件是否已被处理

		virtual EventType GetEventType() const = 0;					// 获取事件类型等
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {			// 判断是否是该类型的事件
			return GetCategoryFlags() & category;
		}

	};

	// 事件分发器-----------------------------------------------------------
	class EventDispatcher
	{
		// 定义一个接收参数T&，返回bool类型的函数对象
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			:m_Event(event) {}

		template<typename T>										// 写两个T是为了区分共有和私有
		bool Dispatch(EventFn<T> func) {							// 接收一个“参数为T”且“返回值为bool”的函数func（使用函数指针）
			if (m_Event.GetEventType() == T::GetStaticType()) {		// T::即通过类名调用获取静态类型的函数，如果当前事件（创建Dispatcher时使用的事件）与接收的函数指针所对应的事件相同
				m_Event.Handled = func(static_cast<T&>(m_Event) );	// 将m_Event转为T&，执行事件
				return true;										// 事件类型匹配且已经被处理，返回true
			}
			return false;											// 事件类型不匹配，返回false
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}
