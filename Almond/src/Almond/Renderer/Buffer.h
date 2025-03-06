#pragma once

// VAO、VBO、EBO 用于不同平台

namespace Almond {

	//需要实现glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr)的抽象
	// Shader数据类型
	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	// 获得元素类型的大小
	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
			case ShaderDataType::Float:			return 4;
			case ShaderDataType::Float2:		return 4 * 2;
			case ShaderDataType::Float3:		return 4 * 3;
			case ShaderDataType::Float4:		return 4 * 4;
			case ShaderDataType::Mat3:			return 4 * 3 * 3;
			case ShaderDataType::Mat4:			return 4 * 4 * 4;
			case ShaderDataType::Int:			return 4;
			case ShaderDataType::Int2:			return 4 * 2;
			case ShaderDataType::Int3:			return 4 * 3;
			case ShaderDataType::Int4:			return 4 * 4;
			case ShaderDataType::Bool:			return 1;
		}
		AM_CORE_ASSERT(false, "Unknow ShaderDataType!");
		return 0;
	}

	// 具体的缓冲区布局
	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			:Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		// 获得元素个数
		uint32_t GetComponentCount() const {
			switch (Type)
			{
				case ShaderDataType::Float:			return 1;
				case ShaderDataType::Float2:		return 2;
				case ShaderDataType::Float3:		return 3;
				case ShaderDataType::Float4:		return 4;
				case ShaderDataType::Mat3:			return 3 * 3;
				case ShaderDataType::Mat4:			return 4 * 4;
				case ShaderDataType::Int:			return 1;
				case ShaderDataType::Int2:			return 2;
				case ShaderDataType::Int3:			return 3;
				case ShaderDataType::Int4:			return 4;
				case ShaderDataType::Bool:			return 1;
			}
			AM_CORE_ASSERT(false, "Unknow ShaderDataType!");
			return 0;
		}

	};

	// 自定义缓冲区布局，实现类似
	/*	BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position"}		// 一个BufferElement
		};
	*/
	class BufferLayout {
	public:
		BufferLayout() {}

		// initializer_list用于支持列表初始化
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			:m_Element(elements) {
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Element; }

		std::vector<BufferElement>::iterator begin()				{ return m_Element.begin(); }
		std::vector<BufferElement>::iterator end()					{ return m_Element.end(); }				
		std::vector<BufferElement>::const_iterator begin() const	{ return m_Element.cbegin(); }				
		std::vector<BufferElement>::const_iterator end() const		{ return m_Element.cend(); }				
	private:
		// 计算offset 和 stride
		void CalculateOffsetsAndStride() {
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Element) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Element;		// 其中具体的BufferElement
		uint32_t m_Stride = 0;
	};

	// Vertex Buffer
	class VertexBuffer {
	public:
		VertexBuffer() = default;
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, unsigned int size);
	};

	// Index Buffer
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static IndexBuffer* Create(unsigned int* indices, unsigned int size);
	};

}