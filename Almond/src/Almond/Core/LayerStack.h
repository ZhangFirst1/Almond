#pragma once

#include "Almond/Core/Core.h"
#include "Layer.h"

/*
层栈结构：
		overlay3    某一设置选项图层
		overlay2    设置图层
		overlay1    菜单图层
		layer1      游戏中的UI（击杀敌人时显示的图标）
		layer2      游戏角色
		layer3      游戏背景

实际存储：
（栈底，vector头部）layer3, layer2, layer1, overlay1, overlay2, overlay3（栈顶，vector尾部）
*/

namespace Almond {

	class ALMOND_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);					// 将一个 “普通图层” 推入图层列表的前半部分
		void PushOverlay(Layer* layer);					// 将一个 “覆盖图层” 推入图层列表的后半部分
		void PopLayer(Layer* layer);					// 从图层列表中删除指定的图层
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin()						{ return m_Layers.begin(); }	//设计原因：当这个LayerStack对象作为for each loop（基于范围的for循环）的范围时
		std::vector<Layer*>::iterator end()							{ return m_Layers.end(); }		//要求LayerStack 这个容器具有 begin() 和 end() 成员函数
		std::vector<Layer*>::const_iterator begin() const			{ return m_Layers.cbegin(); }
		std::vector<Layer*>::const_iterator end() const				{ return m_Layers.cend(); }

		std::vector<Layer*>::reverse_iterator rbegin()				{ return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend()				{ return m_Layers.rend(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const	{ return m_Layers.crbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend()   const	{ return m_Layers.crbegin(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;		// 迭代器，用于指向下一个图层插入的位置
	};

}

