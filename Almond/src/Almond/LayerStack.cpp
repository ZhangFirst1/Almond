#include "ampch.h"
#include "LayerStack.h"

namespace Almond {

	LayerStack::LayerStack() {

	}

	LayerStack::~LayerStack() {
		for (Layer* layer : m_Layers)		// 释放内存
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer) {
		// emplace 在m_LayerInsert前插入（直接在vector中构造），并返回指向该元素的迭代器
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* layer) {
		m_Layers.emplace_back(layer);		// 将覆盖图层插入图层列表的后半部分
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);					// 只将图层删除，并未释放内存
			m_LayerInsertIndex--;				// 此处有些疑惑，每次只删除栈顶?
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}

}