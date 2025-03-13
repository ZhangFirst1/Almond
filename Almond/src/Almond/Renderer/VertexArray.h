#pragma once

#include "Buffer.h"

#include <memory>

namespace Almond {

	// VAO
	class VertexArray {
	public:
		virtual ~VertexArray() {}
		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// 为VAO绑定VBO和EBO
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer)  = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer)  = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
		static VertexArray* Create();
	};


}