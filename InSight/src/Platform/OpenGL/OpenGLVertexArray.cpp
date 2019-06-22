#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace InSight
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType aType)
	{
		switch (aType)
		{
			case InSight::ShaderDataType::Float:	return GL_FLOAT;
			case InSight::ShaderDataType::Float2:	return GL_FLOAT;
			case InSight::ShaderDataType::Float3:	return GL_FLOAT;
			case InSight::ShaderDataType::Float4:	return GL_FLOAT;
			case InSight::ShaderDataType::Mat3:		return GL_FLOAT;
			case InSight::ShaderDataType::Mat4:		return GL_FLOAT;
			case InSight::ShaderDataType::Int:		return GL_INT;
			case InSight::ShaderDataType::Int2:		return GL_INT;
			case InSight::ShaderDataType::Int3:		return GL_INT;
			case InSight::ShaderDataType::Int4:		return GL_INT;
			case InSight::ShaderDataType::Bool:		return GL_BOOL;
		}
		EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// VERTEX ARRAY
	/////////////////////////////////////////////////////////////////////////

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &mRendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &mRendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(mRendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& aVertexBuffer)
	{

		EN_CORE_ASSERT(aVertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(mRendererID);
		aVertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = aVertexBuffer->GetLayout();
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		mVertexBuffers.push_back(aVertexBuffer);
	}

	void OpenGLVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& aIndexBuffer)
	{
		glBindVertexArray(mRendererID);
		aIndexBuffer->Bind();
		mIndexBuffer = aIndexBuffer;
	}
}